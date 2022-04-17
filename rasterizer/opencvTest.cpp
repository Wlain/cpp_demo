//
// Created by william on 2021/10/15.
//
#include "commonMacro.h"

#include <opencv2/opencv.hpp>

void opencvTest()
{
    cv::Mat image = cv::imread(GET_CURRENT("/resources/test.jpg"));
    if (!image.data)
    {
        printf("No m_image data \n");
        return;
    }
    imshow("Display Image", image);
    cv::waitKey(0);
}
