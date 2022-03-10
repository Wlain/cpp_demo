//
// Created by william on 2022/3/11.
//

#include <opencv2/opencv.hpp>

cv::Mat alphaBlend(const cv::Mat& src, const cv::Mat& dst, const cv::Mat& mask)
{
    // get height and width
    int width = src.cols;
    int height = src.rows;

    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);

    // each y, x
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            auto maskColor = mask.at<cv::Vec3b>(y, x)[0] / 255.0f;
            // R -> B
            out.at<cv::Vec3b>(y, x)[0] = (src.at<cv::Vec3b>(y, x)[0]) * maskColor + (1.0 - maskColor) * (dst.at<cv::Vec3b>(y, x)[0]);
            // B -> R
            out.at<cv::Vec3b>(y, x)[1] = (src.at<cv::Vec3b>(y, x)[1]) * maskColor + (1.0 - maskColor) * (dst.at<cv::Vec3b>(y, x)[1]);
            // G -> B
            out.at<cv::Vec3b>(y, x)[2] = (src.at<cv::Vec3b>(y, x)[2]) * maskColor + (1.0 - maskColor) * (dst.at<cv::Vec3b>(y, x)[2]);
        }
    }
    return out;
}