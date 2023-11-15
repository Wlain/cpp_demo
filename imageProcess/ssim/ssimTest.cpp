#include <opencv2/opencv.hpp>
#include <string_view>
///// http://mehdi.rabah.free.fr/SSIM/
static double calcSSIMValue(std::string_view path1, std::string_view path2)
{
    double C1 = 6.5025, C2 = 58.5225;

    cv::Mat img1 = cv::imread(path1.data(), cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread(path2.data(), cv::IMREAD_COLOR);

    if (img1.empty() || img2.empty())
        return -1;

    img1.convertTo(img1, CV_32F);
    img2.convertTo(img2, CV_32F);

    cv::Mat img1Sq, img2Sq, img1Img2;
    cv::pow(img1, 2, img1Sq);
    cv::pow(img2, 2, img2Sq);
    cv::multiply(img1, img2, img1Img2);

    cv::Mat mu1, mu2;
    cv::GaussianBlur(img1, mu1, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(img2, mu2, cv::Size(11, 11), 1.5);

    cv::Mat mu1Sq, mu2Sq, mu1Mu2;
    cv::pow(mu1, 2, mu1Sq);
    cv::pow(mu2, 2, mu2Sq);
    cv::multiply(mu1, mu2, mu1Mu2);

    cv::Mat sigma1Sq, sigma2Sq, sigma12;
    cv::GaussianBlur(img1Sq, sigma1Sq, cv::Size(11, 11), 1.5);
    cv::subtract(sigma1Sq, mu1Sq, sigma1Sq);
    cv::GaussianBlur(img2Sq, sigma2Sq, cv::Size(11, 11), 1.5);
    cv::subtract(sigma2Sq, mu2Sq, sigma2Sq);
    cv::GaussianBlur(img1Img2, sigma12, cv::Size(11, 11), 1.5);
    cv::subtract(sigma12, mu1Mu2, sigma12);

    cv::Mat temp1, temp2, temp3;
    cv::multiply(mu1Mu2, 2, temp1);
    cv::add(temp1, C1, temp1);
    cv::multiply(sigma12, 2, temp2);
    cv::add(temp2, C2, temp2);
    cv::multiply(temp1, temp2, temp3);

    cv::add(mu1Sq, mu2Sq, temp1);
    cv::add(temp1, C1, temp1);
    cv::add(sigma1Sq, sigma2Sq, temp2);
    cv::add(temp2, C2, temp2);
    cv::multiply(temp1, temp2, temp1);

    cv::Mat ssimMap;
    cv::divide(temp3, temp1, ssimMap);

    cv::Scalar indexScalar = cv::mean(ssimMap);

    return (indexScalar[0] + indexScalar[1] + indexScalar[2]) / 3.0;
}

bool calcSSIMResult(std::string_view path1, std::string_view path2)
{
    auto result = calcSSIMValue(path1, path2);
    return result > 0.99f;
}