//
// Created by william on 2022/3/18.
//
#include "vec3.h"

#include <opencv2/opencv.hpp>

cv::Mat computeGradientX(const cv::Mat& image)
{
    // get height and width
    int width = image.cols;
    int height = image.rows;

    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    cv::Mat kernel = cv::Mat::zeros(1, 3, CV_8S);
    kernel.at<char>(0, 2) = 1;
    kernel.at<char>(0, 1) = -1;
    for (int i = 0; i < height - 1; ++i)
    {
        for (int j = 0; j < width - 1; ++j)
        {
            auto gx = image.at<cv::Vec3b>(i, j + 1) - image.at<cv::Vec3b>(i, j);
            auto gy = image.at<cv::Vec3b>(i + 1, j) - image.at<cv::Vec3b>(i, j);
            auto gradient = gx + gy;
            out.at<cv::Vec3b>(i, j) = gradient;
        }
    }
    return out;
}