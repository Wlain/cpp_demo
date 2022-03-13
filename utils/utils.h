//
// Created by ytech on 2021/10/25.
//

#ifndef CPP_DEMO_UTILS_H
#define CPP_DEMO_UTILS_H

#include <opencv2/opencv.hpp>
#include <sys/stat.h>

constexpr const int width = 1080;
constexpr const int height = 720;

inline bool isFileExist(const std::string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

/// 展示面板
static void display(const char* title, void* data)
{
    cv::Mat image(height, width, CV_32FC3, data);
    cv::imshow(title, image);
    cv::waitKey();
}

#endif //CPP_DEMO_UTILS_H
