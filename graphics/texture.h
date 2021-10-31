//
// Created by william on 2021/10/24.
//

#ifndef CPP_DEMO_TEXTURE_H
#define CPP_DEMO_TEXTURE_H
#include "base.h"

#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>

namespace graphics
{
class Texture
{
public:
    Texture(const char* name);
    ~Texture();
    Eigen::Vector3f color(float u, float v);
    Eigen::Vector3f colorBilinear(float u, float v);
    int width() const;
    int height() const;

private:
    cv::Mat m_imageData;
    int m_width;
    int m_height;
};
} // namespace graphics

#endif //CPP_DEMO_TEXTURE_H
