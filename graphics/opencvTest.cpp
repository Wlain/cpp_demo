//
// Created by william on 2021/10/15.
//
#include "opencvTest.h"
#include "base.h"

OpencvTest::OpencvTest()
{
    m_image = cv::imread(GET_CURRENT(/resources/test.jpg));
}

OpencvTest::~OpencvTest() = default;

void OpencvTest::show()
{
    if (!m_image.data)
    {
        printf("No m_image data \n");
        return;
    }
    imshow("Display Image", m_image);
    cv::waitKey(0);
}