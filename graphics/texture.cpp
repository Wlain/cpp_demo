//
// Created by william on 2021/10/24.
//

#include "texture.h"
using namespace Eigen;
namespace graphics
{
Texture::Texture(const char* name)
{
    m_imageData = cv::imread(name);
    cv::cvtColor(m_imageData, m_imageData, cv::COLOR_RGB2BGR);
    m_width = m_imageData.cols;
    m_height = m_imageData.rows;
}

Texture::~Texture() = default;

Vector3f Texture::color(float u, float v)
{
    u = std::clamp(u, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);
    v = !std::isnan(v) ? v : 1.0f;
    u = !std::isnan(u) ? u : 1.0f;
    auto uImg = u * (float)width();
    auto vImg = (1.0f - v) * (float)height();
    auto color = m_imageData.at<cv::Vec3b>((int)vImg, (int)uImg);
    return { color[0], color[1], color[2] };
}

Vector3f Texture::colorBilinear(float u, float v)
{
    u = std::clamp(u, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);
    v = !std::isnan(v) ? v : 1.0f;
    u = !std::isnan(u) ? u : 1.0f;
    auto uImg = u * (float)width();
    auto vImg = (1.0f - v) * (float)height();
    auto uMin = std::floor(uImg);
    auto uMax = std::ceil(uImg);
    auto vMin = std::floor(vImg);
    auto vMax = std::ceil(vImg);
    auto color11 = m_imageData.at<cv::Vec3b>((int)vMin, (int)uMin);
    auto color12 = m_imageData.at<cv::Vec3b>((int)vMin, (int)uMax);
    auto color21 = m_imageData.at<cv::Vec3b>((int)vMax, (int)uMin);
    auto color22 = m_imageData.at<cv::Vec3b>((int)vMax, (int)uMax);

    auto color1 = (uImg - uMin) * color11 + (uMax - uImg) * color12;
    auto color2 = (uImg - uMin) * color21 + (uMax - uImg) * color22;
    auto color = (vImg - vMin) * color1 + (vMax - vImg) * color2;
    return { color[0], color[1], color[2] };
}

int Texture::width() const
{
    return m_width;
}

int Texture::height() const
{
    return m_height;
}
} // namespace graphics
