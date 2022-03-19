//
// Created by william on 2022/3/18.
//

#include <opencv2/opencv.hpp>

/// 求微分
cv::Mat computeLaplace(const cv::Mat& image)
{
    // get height and width
    int width = image.cols;
    int height = image.rows;

    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            auto delta = 1;
            auto fx = (image.at<cv::Vec3b>(i, j + 1) - image.at<cv::Vec3b>(i, j) - (image.at<cv::Vec3b>(i, j) - image.at<cv::Vec3b>(i, j - 1))) / std::pow(delta, 2);
            auto fy = (image.at<cv::Vec3b>(i + 1, j) - image.at<cv::Vec3b>(i, j) - (image.at<cv::Vec3b>(i, j) - image.at<cv::Vec3b>(i - 1, j))) / std::pow(delta, 2);
            out.at<cv::Vec3b>(i, j) = (fx + fy);
        }
    }
    return out;
}