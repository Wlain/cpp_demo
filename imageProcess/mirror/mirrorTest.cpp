//
// Created by cwb on 2022/3/2.
//
#include <opencv2/opencv.hpp>

cv::Mat mirrorTest(const cv::Mat& img, bool horizontal, bool vertical)
{
    // get height and width
    int width = img.cols;
    int height = img.rows;
    // prepare output
    cv::Mat out = img.clone();
    if (horizontal)
    {
        if (vertical)
        {
            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    out.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(width - 1 - i, height - 1 - j);
                }
            }
        }
        else
        {
            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    out.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i, height - 1 - j);
                }
            }
        }
    }
    else
    {
        if (vertical)
        {
            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    out.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(width - 1 - i, j);
                }
            }
        }
    }
    return out;
}