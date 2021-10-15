//
// Created by william on 2021/10/15.
//

#ifndef CPP_DEMO_OPENCVTEST_H
#define CPP_DEMO_OPENCVTEST_H
#include <opencv2/opencv.hpp>

class OpencvTest
{
public:
    OpencvTest();
    ~OpencvTest();
    void show();

private:
    cv::Mat m_image;
};

#endif //CPP_DEMO_OPENCVTEST_H
