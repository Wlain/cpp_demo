//
// Created by cwb on 2022/2/17.
//
#include "base.h"

#include <opencv2/opencv.hpp>

extern cv::Mat channelSwap(const cv::Mat& img);
extern cv::Mat colorTransferBetweenImages();
extern cv::Mat mirrorTest(const cv::Mat& img, bool horizontal, bool vertical);

void imageProcessTest()
{
    cv::Mat image = cv::imread(GET_CURRENT(/resources/test.jpg));
    if (!image.data)
    {
        printf("No m_image data \n");
        return;
    }
//    cv::Mat out = channelSwap(image);
    auto out = mirrorTest(image, false, true);
    imshow("Display result", out);
    cv::waitKey(0);
}