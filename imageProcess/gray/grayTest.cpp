//
// Created by william on 2022/2/26.
//
#include <opencv2/opencv.hpp>
#include "vector3.h"

cv::Mat grayTest(const cv::Mat& img)
{
    // get height and width
    int width = img.cols;
    int height = img.rows;
    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    // each y, x
    for(int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Vector3 weight{ 0.2125, 0.7154, 0.0721 };
            Vector3 color{ (float )img.at<cv::Vec3b>(y, x)[0], (float )img.at<cv::Vec3b>(y, x)[1], (float )img.at<cv::Vec3b>(y, x)[2] };
            auto gray = weight.dot(color);
            out.at<cv::Vec3b>(y, x)[0] = (int)gray;
            out.at<cv::Vec3b>(y, x)[2] = (int)gray;
            out.at<cv::Vec3b>(y, x)[1] = (int)gray;
        }
    }
    return out;
}
