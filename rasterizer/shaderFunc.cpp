//
// Created by ytech on 2021/10/25.
//

#include "shaderFunc.h"

#include <cmath>
using namespace Eigen;
using namespace graphics;

namespace graphics
{
Vector4f baseVertShader(const VertexShader& vertShader)
{
    return vertShader.position();
}

Vector3f baseFragShader(const FragmentShader& fragShader)
{
    return { 148.0f, 121.0f, 92.0f };
}

Eigen::Vector3f baseFragTriangleShader(const FragmentShader& fragShader)
{
    return fragShader.color();
}

Vector3f normalFragShader(const FragmentShader& fragShader)
{
    Vector3f returnColor = (fragShader.normal().normalized() + Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    return { returnColor.x() * 255, returnColor.y() * 255, returnColor.z() * 255 };
}

Vector3f textureFragmentShader(const FragmentShader& fragShader)
{
    Vector3f fragmentColor;
    fragmentColor.setOnes();
    if (fragShader.texture())
    {
        fragmentColor = fragShader.texture()->colorBilinear(fragShader.texCoords().x(), fragShader.texCoords().y());
    }
    return fragmentColor;
}

Vector4f groundVertexShader(const VertexShader& vertShader)
{
}

Vector3f blinnPhongFragmentShader(const FragmentShader& fragShader)
{
    Vector3f ka = { 0.005f, 0.005f, 0.005f };
    Vector3f kd = fragShader.texture()->color(fragShader.texCoords().x(), fragShader.texCoords().y()) / 255.0f;
    Vector3f ks = { 0.7937, 0.7937, 0.7937 };
    std::vector<Light> lights = {
        { { 20, 20, 20 }, { 500, 500, 500 } },
        { { -20, 20, 0 }, { 500, 500, 500 } }
    };
    Vector3f ambientLightIntensity = { 10.0f, 10.0f, 10.0f };
    Vector3f eyePos = { 0.0f, 0.0f, 10.0f };
    float p = 150.0f;

    const auto& target = fragShader.viewPosition().head<3>();
    const auto& normal = fragShader.normal().normalized();

    Vector3f fragColor = { 0, 0, 0 };
    for (auto& light : lights)
    {
        auto distanceSquared = (light.position - target).squaredNorm(); //范数（模）的平方，也就是距离平方
        auto lightDir = (light.position - target).normalized();
        auto viewDir = (eyePos - target).normalized();
        auto halfDir = (lightDir + viewDir).normalized();
        auto ambientColor = ka.cwiseProduct(ambientLightIntensity);
        auto diffuseColor = kd.cwiseProduct(light.intensity / distanceSquared) * std::max(0.0f, normal.dot(lightDir));
        Vector3f specularColor = ks * std::pow(std::max(0.0f, normal.dot(halfDir)), p);
        fragColor += (ambientColor + diffuseColor + specularColor);
    }
    return fragColor * 255.0f;
}

Vector3f bumpFragmentShader(const FragmentShader& fragShader)
{
    Vector3f ka = { 0.005, 0.005, 0.005 };
    Vector3f kd = fragShader.texture()->color(fragShader.texCoords().x(), fragShader.texCoords().y()) / 255.0f;
    Vector3f ks = { 0.7937, 0.7937, 0.7937 };

    std::vector<Light> lights = {
        { { 20, 20, 20 }, { 500, 500, 500 } },
        { { -20, 20, 0 }, { 500, 500, 500 } }
    };
    Vector3f ambientLightIntensity = { 10.0f, 10.0f, 10.0f };
    Vector3f eyePos = { 0, 0, 10 };
    float p = 150.0f;

    const auto& target = fragShader.viewPosition().head<3>();
    const auto& normal = fragShader.normal().normalized();

    float kh = 0.2f, kn = 0.1f;
    /// 这两个是一个系数，为了控制凹凸贴图对原图的影响
    auto n = fragShader.normal().normalized();
    auto x = n.x();
    auto y = n.y();
    auto z = n.z();
    // x'，z'是根据相似求出来的
    // t 是切线
    Vector3f t = {
        x * y / std::sqrt(x * x + z * z),
        std::sqrt(x * x + z * z),
        z * y / std::sqrt(x * x + z * z)
    };
    t.normalized();
    auto b = n.cross(t);
    b.normalized();
    /// 空间上的转化矩阵TBN
    Matrix3f tBN;
    tBN << t.x(), b.x(), n.x(),
        t.y(), b.y(), n.y(),
        t.z(), b.z(), n.z();
    float u = fragShader.texCoords().x();
    float v = fragShader.texCoords().y();
    float w = fragShader.texture()->width();
    float h = fragShader.texture()->height();
    // 梯度：方向变换求出来的导数
    // uc 指代的是 dp / du
    float uc = fragShader.texture()->colorBilinear(u + 1.0f / w, v).norm() - fragShader.texture()->colorBilinear(u, v).norm();
    // vc 指代的是 dp / dv
    float vc = fragShader.texture()->colorBilinear(u, v + 1.0f / h).norm() - fragShader.texture()->colorBilinear(u, v).norm();
    float dPU = kh * kn * uc;
    float dPV = kh * kn * vc;
    Vector3f lt = { dPU, dPV, 1 };
    /// x方向的切线：dPU
    /// y方向的切线：dPV
    /// 在每个点假设每个点法向量n是（0， 0， 1），这个是局部三维坐标系，求出一个
    /// 梯度：dPU / dPV
    /// 求法线：逆时针旋转90度(实际上就是乘以一个旋转矩阵)，得到转转之后的坐标（-dP.x,-dP.y, 1），这个坐标也就是根据uv图计算出来的法线;
    Matrix3f rotateMatrix;
    /// 绕某个轴轴逆时针旋转90度，得到法线向量
    Vector3f ln = { -dPU, -dPV, 1.0f };
    ln.normalize();
    /// 转换到三维空间中
    n = (tBN * ln).normalized();
    Vector3f fragColor = { 0, 0, 0 };
    for (auto& light : lights)
    {
        auto distanceSquared = (light.position - target).squaredNorm(); //范数（模）的平方，也就是距离平方
        auto lightDir = (light.position - target).normalized();
        auto viewDir = (eyePos - target).normalized();
        auto halfDir = (lightDir + viewDir).normalized();
        auto ambientColor = ka.cwiseProduct(ambientLightIntensity);
        auto diffuseColor = kd.cwiseProduct(light.intensity / distanceSquared) * std::max(0.0f, normal.dot(lightDir));
        Vector3f specularColor = ks * std::pow(std::max(0.0f, normal.dot(halfDir)), p);
        fragColor += (ambientColor + diffuseColor + specularColor);
    }
    fragColor += n;
    return fragColor * 255.f;
}

Vector3f displacementFragmentShader(const FragmentShader& fragShader)
{
    Vector3f ka = { 0.005, 0.005, 0.005 };
    Vector3f kd = fragShader.texture()->color(fragShader.texCoords().x(), fragShader.texCoords().y()) / 255.0f;
    Vector3f ks = { 0.7937, 0.7937, 0.7937 };

    std::vector<Light> lights = {
        { { 20, 20, 20 }, { 500, 500, 500 } },
        { { -20, 20, 0 }, { 500, 500, 500 } }
    };
    Vector3f ambientLightIntensity = { 10.0f, 10.0f, 10.0f };
    Vector3f eyePos = { 0, 0, 10 };
    float p = 150.0f;

    Vector3f target = fragShader.viewPosition().head<3>();
    auto normal = fragShader.normal().normalized();

    float kh = 0.2, kn = 0.1;

    /// 这两个是一个系数，为了控制凹凸贴图对原图的影响
    auto n = fragShader.normal().normalized();
    auto x = n.x();
    auto y = n.y();
    auto z = n.z();
    // x'，z'是根据相似求出来的
    // t 是切线
    Vector3f t = {
        x * y / std::sqrt(x * x + z * z),
        std::sqrt(x * x + z * z),
        z * y / std::sqrt(x * x + z * z)
    };
    t.normalized();
    auto b = n.cross(t);
    b.normalized();
    /// 空间上的转化矩阵TBN
    Matrix3f tBN;
    tBN << t.x(), b.x(), n.x(),
        t.y(), b.y(), n.y(),
        t.z(), b.z(), n.z();
    float u = fragShader.texCoords().x();
    float v = fragShader.texCoords().y();
    float w = fragShader.texture()->width();
    float h = fragShader.texture()->height();
    // 梯度：方向变换求出来的导数
    // uc 指代的是 dp / du
    float uc = fragShader.texture()->colorBilinear(u + 1.0f / w, v).norm() - fragShader.texture()->colorBilinear(u, v).norm();
    // vc 指代的是 dp / dv
    float vc = fragShader.texture()->colorBilinear(u, v + 1.0f / h).norm() - fragShader.texture()->colorBilinear(u, v).norm();
    float dPU = kh * kn * uc;
    float dPV = kh * kn * vc;
    Vector3f lt = { dPU, dPV, 1 };
    /// x方向的切线：dPU
    /// y方向的切线：dPV
    /// 在每个点假设每个点法向量n是（0， 0， 1），这个是局部三维坐标系，求出一个
    /// 梯度：dPU / dPV
    /// 求法线：逆时针旋转90度(实际上就是乘以一个旋转矩阵)，得到转转之后的坐标（-dP.x,-dP.y, 1），这个坐标也就是根据uv图计算出来的法线;
    Matrix3f rotateMatrix;
    /// 绕某个轴轴逆时针旋转90度，得到法线向量
    Vector3f ln = { -dPU, -dPV, 1.0f };
    ln.normalize();
    /// 转换到三维空间中
    target = target + kn * normal * fragShader.texture()->colorBilinear(u, v).norm();
    normal = (tBN * ln).normalized();
    Vector3f fragColor = { 0, 0, 0 };
    for (auto& light : lights)
    {
        auto distanceSquared = (light.position - target).squaredNorm(); //范数（模）的平方，也就是距离平方
        auto lightDir = (light.position - target).normalized();
        auto viewDir = (eyePos - target).normalized();
        auto halfDir = (lightDir + viewDir).normalized();
        auto ambientColor = ka.cwiseProduct(ambientLightIntensity);
        auto diffuseColor = kd.cwiseProduct(light.intensity / distanceSquared) * std::max(0.0f, normal.dot(lightDir));
        Vector3f specularColor = ks * std::pow(std::max(0.0f, normal.dot(halfDir)), p);
        fragColor += (ambientColor + diffuseColor + specularColor);
    }
    return fragColor * 255.f;
}

Vector3f normalFragmentShader(const FragmentShader& fragShader)
{
}

} // namespace graphics