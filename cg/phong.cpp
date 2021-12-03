//
// Created by william on 2021/11/29.
//

#include "mathDefine.h"
#include <opencv2/opencv.hpp>

constexpr const int width = 1080;
constexpr const int height = 720;

namespace cg
{
void phongTest()
{
    cv::Mat image(height, width, CV_32FC3, cv::Scalar(0));
    cv::flip(image, image, -1);
    cv::imshow("phongTest", image);
    cv::waitKey();
}
} // namespace cg