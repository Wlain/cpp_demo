//
// Created by william on 2021/10/24.
//

#include "texture.h"

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

Eigen::Vector3f Texture::color(float u, float v)
{
    auto uImg = u * m_width;
    auto vImg = v * (1.0 - v) * m_height;
    auto color = m_imageData.at<cv::Vec3b>(uImg, vImg);
    return Eigen::Vector3f(color[0], color[1], color[2]);
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
