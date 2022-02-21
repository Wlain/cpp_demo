//
// Created by william on 2022/2/19.
//
#include "base.h"

#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat rGBToLab(const cv::Mat_<cv::Vec3f>& m)
{
    auto result = m;
    for (int i = 0; i < m.rows; ++i)
    {
        for (int j = 0; j < m.cols; ++j)
        {
            // RGB to LMS
            auto L = 0.3811 * m(i, j)[0] + 0.5783 * m(i, j)[1] + 0.0402 * m(i, j)[2];
            auto M = 0.1967 * m(i, j)[0] + 0.7244 * m(i, j)[1] + 0.0782 * m(i, j)[2];
            auto S = 0.0241 * m(i, j)[0] + 0.1288 * m(i, j)[1] + 0.8444 * m(i, j)[2];
            // gamma 矫正, 注意需要避免log(0)的情况
            if (L != 0)
            {
                L = std::log(L);
            }
            if (M != 0)
            {
                M = std::log(M);
            }
            if (S != 0)
            {
                S = std::log(S);
            }

            // LMS to Lab
            result.at<cv::Vec3f>(i, j)[0] = (L + M + S) / sqrt(3.0);
            result.at<cv::Vec3f>(i, j)[1] = (L + M - 2 * S) / sqrt(6.0);
            result.at<cv::Vec3f>(i, j)[2] = (L - M) / sqrt(2.0);
        }
    }
    return result;
}

cv::Mat labToRGB(const cv::Mat& m)
{
    cv::Mat result = m;
    for (int i = 0; i < m.rows; ++i)
    {
        for (int j = 0; j < m.cols; ++j)
        {
            // Lab to LMS
            double L = result.at<cv::Vec3f>(i, j)[0] / sqrt(3.0) + result.at<cv::Vec3f>(i, j)[1] / sqrt(6.0) + result.at<cv::Vec3f>(i, j)[2] / sqrt(2.0);
            double M = result.at<cv::Vec3f>(i, j)[0] / sqrt(3.0) + result.at<cv::Vec3f>(i, j)[1] / sqrt(6.0) - result.at<cv::Vec3f>(i, j)[2] / sqrt(2.0);
            double S = result.at<cv::Vec3f>(i, j)[0] / sqrt(3.0) - 2 * result.at<cv::Vec3f>(i, j)[1] / sqrt(6.0);
            // gamma 矫正, 逆运算
            L = exp(L);
            M = exp(M);
            S = exp(S);
            // LMS to RGB
            result.at<cv::Vec3f>(i, j)[0] = std::clamp(4.4679 * L - 3.5873 * M + 0.1193 * S, 0.0, 1.0);
            result.at<cv::Vec3f>(i, j)[1] = std::clamp(-1.2186 * L + 2.3809 * M - 0.1624 * S, 0.0, 1.0);
            result.at<cv::Vec3f>(i, j)[2] = std::clamp(0.0497 * L - 0.2439 * M + 1.2045 * S, 0.0, 1.0);
        }
    }
    return result;
}

cv::Vec3f computeMeans(const cv::Mat& image)
{
    cv::Vec3f result;
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            result += image.at<cv::Vec3f>(i, j);
        }
    }
    return result / (image.rows * image.cols);
}

cv::Vec3f computeStandardDeviations(const cv::Mat& image, const cv::Vec3f& means)
{
    cv::Vec3f result;
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            const cv::Vec3f& tmp = image.at<cv::Vec3f>(i, j) - means;
            result += cv::Vec3f(std::abs(tmp[0]), std::abs(tmp[1]), std::abs(tmp[2]));
        }
    }
    for (int i = 0; i < 3; ++i)
    {
        result[i] = std::sqrt(result[i] / (image.rows * image.cols));
    }
    return result;
}

void colorTransferBetweenImages()
{
    // format off
    cv::Mat inputSrc = cv::imread(GET_CURRENT(/resources/input1.jpg));
    cv::Mat inputDst = cv::imread(GET_CURRENT(/resources/input2.jpg));
    cv::Mat srcImg32F;
    cv::Mat targetImg32F;
    inputSrc.convertTo(srcImg32F, CV_32FC3, 1.0f / 255.0f);
    inputDst.convertTo(targetImg32F, CV_32FC3, 1.0f / 255.0f);
    cv::Mat inputSrcLab = rGBToLab(srcImg32F);
    cv::Mat inputDstLab = rGBToLab(targetImg32F);
    auto inputSrcMeans = computeMeans(inputSrcLab);
    auto inputDstMeans = computeMeans(inputDstLab);
    auto inputSrcStandardDeviations = computeStandardDeviations(inputSrcLab, inputSrcMeans);
    auto inputDstStandardDeviations = computeStandardDeviations(inputDstLab, inputDstMeans);
    cv::Vec3f coef;
    for (int i = 0; i < 3; ++i)
    {
        coef[i] = inputDstStandardDeviations[i] / inputSrcStandardDeviations[i];
    }
    cv::Mat resultNormal = srcImg32F;
    for (int i = 0; i < inputSrc.rows; ++i)
    {
        for (int j = 0; j < inputSrc.cols; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                resultNormal.at<cv::Vec3f>(i, j)[k] = coef[k] * (srcImg32F.at<cv::Vec3f>(i, j)[k] - inputSrcMeans[k]) + inputDstMeans[k];
            }
        }
    }
    resultNormal = labToRGB(resultNormal);
    auto result = resultNormal;
    resultNormal.convertTo(result, CV_8U, 255.0f, 1.0f / 255.0f);
    imshow("inputSrc", inputSrc);
    imshow("inputDst", inputDst);
    imshow("result", result);
    cv::waitKey(0);
}
