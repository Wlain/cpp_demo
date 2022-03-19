//
// Created by william on 2022/3/18.
//
#include <opencv2/opencv.hpp>

/// 求导数
cv::Mat computeGradient(const cv::Mat& image)
{
    // get height and width
    int width = image.cols;
    int height = image.rows;

    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for (int i = 1; i < height-1; ++i)
    {
        for (int j = 1; j < width-1; ++j)
        {
            auto delta = 1;
            auto gx = (image.at<cv::Vec3b>(i, j + 1) - image.at<cv::Vec3b>(i, j)) / delta;
            auto gy = (image.at<cv::Vec3b>(i + 1, j) - image.at<cv::Vec3b>(i, j)) / delta;
            auto gradient = gx + gy;
            out.at<cv::Vec3b>(i, j) = gradient;
        }
    }
    return out;
}