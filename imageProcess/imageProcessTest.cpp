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

// laplacian filter
cv::Mat laplacian_filter(cv::Mat img, int kernel_size)
{
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();

    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);

    // prepare kernel
    double kernel[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};

    int pad = floor(kernel_size / 2);
    double v = 0;
    // filtering
    for (int y = 1; y < height-1; ++y)
    {
        for (int x = 1; x < width-1; ++x)
        {
            v = 0;
            for (int dy = -pad; dy < pad + 1; ++dy)
            {
                for (int dx = -pad; dx < pad + 1; ++dx)
                {
                    if (((dy + y) >= 0) && ((dx + x) >= 0) && ((dy + y) <= height) && ((dx + x) <= width))
                    {
                        v += img.at<uchar>(y + dy, x + dx) * kernel[dy + pad][dx + pad];
                    }
                }
            }
            v = fmax(v, 0);
            v = fmin(v, 255);
            out.at<uchar>(y, x) = (uchar)v;
        }
    }
    return out;
}

void imageProcessTest()
{
    cv::Mat srcImage = cv::imread(GET_CURRENT(/resources/1/fg.jpg));
    cv::Mat targetImage = cv::imread(GET_CURRENT(/resources/1/bg.jpg));
    cv::Mat maskImage = cv::imread(GET_CURRENT(/resources/1/mask.jpg));
    cv::Mat monaLisa = cv::imread(GET_CURRENT(/resources/monaLisa.bmp));
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