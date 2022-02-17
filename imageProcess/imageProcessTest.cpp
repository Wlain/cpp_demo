//
// Created by cwb on 2022/2/17.
//
#include "base.h"

#include <opencv2/opencv.hpp>

extern cv::Mat channelSwap(cv::Mat img);

void imageProcessTest()
{
    cv::Mat image = cv::imread(GET_CURRENT(/resources/test.jpg));
    if (!image.data)
    {
        printf("No m_image data \n");
        return;
    }
    cv::Mat out = channelSwap(image);
    imshow("Display result", out);
    cv::waitKey(0);
}