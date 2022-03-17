//
// Created by cwb on 2022/3/17.
//

#include "idwWarping.h"

#include <opencv2/opencv.hpp>

static std::vector<Vector2> start = { { 155, 11.5 },
                                      { 196, 178.5 },
                                      { 201, 183.5 } };

static std::vector<Vector2> end = { { 141, 190.5 },
                                    { 205, 184.5 },
                                    { 211, 188.5 } };

cv::Mat idwTest(const cv::Mat& img)
{
    // get height and width
    int width = img.cols;
    int height = img.rows;
    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    IdwWarping warping(start, end);
    warping.resize(width, height);
    warping.resetFilledStatus();
    warping.getWarpingResult(img, out);
    return out;
}