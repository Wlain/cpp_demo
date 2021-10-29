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
    Vector3f kd = fragShader.texture()->colorBilinear(fragShader.texCoords().x(), fragShader.texCoords().y()) / 255.0f;
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
    Vector3f kd = fragShader.texture()->colorBilinear(fragShader.texCoords().x(), fragShader.texCoords().y()) / 255.0f;
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

    auto n = fragShader.normal().normalized();
    auto x = n.x();
    auto y = n.y();
    auto z = n.z();
    Vector3f t = {
        x * y / std::sqrt(x * x + z * z),
        std::sqrt(x * x + z * z),
        z * y / std::sqrt(x * x + z * z)
    };
    t.normalized();
    auto b = n.cross(t);
    b.normalized();
    Matrix3f tBN;
    tBN << t.x(), b.x(), n.x(),
        t.y(), b.y(), n.y(),
        t.z(), b.z(), n.z();
    float u = fragShader.texCoords().x();
    float v = fragShader.texCoords().y();
    float w = fragShader.texture()->width();
    float h = fragShader.texture()->height();
    float uc = fragShader.texture()->colorBilinear(u + 1.0f / w, v).norm() - fragShader.texture()->colorBilinear(u, v).norm();
    float vc = fragShader.texture()->colorBilinear(u, v + 1.0f / h).norm() - fragShader.texture()->colorBilinear(u, v).norm();
    float dU = kh * kn * uc;
    float dV = kh * kn * vc;
    Vector3f ln = { -dU, -dV, 1 };
    ln.normalize();
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
}

Vector3f normalFragmentShader(const FragmentShader& fragShader)
{
}

} // namespace graphics