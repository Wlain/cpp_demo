//
// Created by cwb on 2022/2/17.
//
#include "base.h"

#include <opencv2/opencv.hpp>

extern cv::Mat channelSwap(const cv::Mat& img);
extern cv::Mat colorTransferBetweenImages(const cv::Mat& inputSrc, const cv::Mat& inputDst);
extern cv::Mat mirrorTest(const cv::Mat& img, bool horizontal, bool vertical);
extern cv::Mat alphaBlend(const cv::Mat& src, const cv::Mat& dst, const cv::Mat& mask);
extern cv::Mat grayTest(const cv::Mat& img);
extern cv::Mat idwTest(const cv::Mat& img);
extern cv::Mat rbfTest(const cv::Mat& img);
extern cv::Mat computeGradient(const cv::Mat& image);
extern cv::Mat computeLaplace(const cv::Mat& image);
extern cv::Mat possionBlend(cv::InputArray _src, cv::InputArray _target, cv::InputArray _mask);

void imageProcessTest()
{
    cv::Mat srcImage = cv::imread(GET_CURRENT("/resources/1/fg.jpg"));
    cv::Mat targetImage = cv::imread(GET_CURRENT("/resources/1/bg.jpg"));
    cv::Mat maskImage = cv::imread(GET_CURRENT("/resources/1/mask.jpg"));
    cv::Mat monaLisa = cv::imread(GET_CURRENT("/resources/monaLisa.bmp"));
    if (!srcImage.data || !targetImage.data)
    {
        printf("No srcImage data or No targetImage data \n");
        return;
    }
    //    cv::Mat out = channelSwap(srcImage);
    //    auto out = mirrorTest(srcImage, false, false);
    //    auto out = colorTransferBetweenImages(srcImage, targetImage);
    //    auto out = alphaBlend(srcImage, targetImage, maskImage);
    //    auto out = grayTest(srcImage);
    //    auto idw = idwTest(monaLisa);
    //    auto rbf = rbfTest(monaLisa);
    //    imshow("IDW", idw);
    //    imshow("RBF", rbf);
    //    auto out = computeGradient(srcImage);
    auto out = computeLaplace(grayTest(srcImage));
    imshow("result image", out);
    cv::waitKey(0);
}