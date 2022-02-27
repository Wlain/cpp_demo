//
// Created by cwb on 2022/2/17.
//
#include <opencv2/opencv.hpp>

cv::Mat channelSwap(const cv::Mat& img)
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
            // R -> B
            out.at<cv::Vec3b>(y, x)[0] = img.at<cv::Vec3b>(y, x)[2];
            // B -> R
            out.at<cv::Vec3b>(y, x)[2] = img.at<cv::Vec3b>(y, x)[0];
            // G -> B
            out.at<cv::Vec3b>(y, x)[1] = img.at<cv::Vec3b>(y, x)[1];
        }
    }
    return out;
}
