//
// Created by cwb on 2022/3/17.
//

#include "idwWarping.h"
#include "rbfWarping.h"

#include <opencv2/opencv.hpp>

static std::vector<Vector2> start = { { 155, 180.5 },
                                      { 196, 178.5 },
                                      { 201, 183.5 } };

static std::vector<Vector2> end = { { 141, 190.5 },
                                    { 205, 184.5 },
                                    { 211, 188.5 } };

void fillWhite(cv::Mat& img)
{
    int width = img.cols;
    int height = img.rows;
    /// 清屏
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            img.at<cv::Vec3b>(i, j) = { 255, 255, 255 };
        }
    }
}

cv::Mat idwTest(const cv::Mat& img)
{
    // get height and width
    int width = img.cols;
    int height = img.rows;
    // prepare output∂
    cv::Mat out = cv::Mat::ones(height, width, CV_8UC3);
    fillWhite(out);
    IdwWarping warping(start, end);
    warping.resize(width, height);
    warping.resetFilledStatus();
    warping.getWarpingResult(img, out);
    return out;
}

cv::Mat rbfTest(const cv::Mat& img)
{
    // get height and width
    int width = img.cols;
    int height = img.rows;
    // prepare output∂
    cv::Mat out = cv::Mat::ones(height, width, CV_8UC3);
    fillWhite(out);
    RbfWarping warping(start, end);
    warping.resize(width, height);
    warping.resetFilledStatus();
    warping.getWarpingResult(img, out);
    return out;
}