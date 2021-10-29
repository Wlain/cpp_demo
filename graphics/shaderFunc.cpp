//
// Created by ytech on 2021/10/25.
//

#include "shaderFunc.h"
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
    if (fragShader.m_texture)
    {
        fragmentColor = fragShader.m_texture->colorBilinear(fragShader.m_texCoords.x(), fragShader.m_texCoords.y());
    }
    return fragmentColor;
}

Vector4f groundVertexShader(const VertexShader& vertShader)
{
}

Vector3f blinnPhongFragmentShader(const FragmentShader& fragShader)
{
    Vector3f ka = { 0.005f, 0.005f, 0.005f };
    Vector3f kd = fragShader.m_texture->colorBilinear(fragShader.m_texCoords.x(), fragShader.m_texCoords.y()) / 255.0f;
    Vector3f ks = { 0.7937, 0.7937, 0.7937 };
    std::vector<Light> lights = {
        { { 20, 20, 20 }, { 500, 500, 500 } },
        { { -20, 20, 0 }, { 500, 500, 500 } }
    };
    Vector3f ambientLightIntensity = { 10.0f, 10.0f, 10.0f };
    Vector3f eyePos = { 0, 0, 10 };
    float p = 150.0f;

    auto target = fragShader.viewPosition().head<3>();
    auto normal = fragShader.normal();

    Vector3f fragColor = { 0, 0, 0 };
    for (auto& light : lights)
    {
        auto distanceSquared = (light.position - target).squaredNorm(); //范数（模）的平方，也就是距离平方
        auto lightDir = (light.position - target).normalized();
        auto viewDir = (eyePos - target).normalized();
        auto halfDir = (lightDir + viewDir).normalized();

        auto ambientColor = ka.cwiseProduct(ambientLightIntensity);
        auto diffuseColor = kd.cwiseProduct(light.intensity / distanceSquared * std::max(0.0f, normal.dot(lightDir)));
        auto specularColor = ks.cwiseProduct(light.intensity / distanceSquared * std::pow(std::max(0.0f, normal.dot(halfDir)), p));
        fragColor = ambientColor + diffuseColor + specularColor;
    }
    return fragColor * 255.0f;
}

Vector3f bumpFragmentShader(const FragmentShader& fragShader)
{
}

Vector3f displacementFragmentShader(const FragmentShader& fragShader)
{
}

Vector3f normalFragmentShader(const FragmentShader& fragShader)
{
}

} // namespace graphics