//
// Created by william on 2022/11/4.
//
#include "commonMacro.h"

#include <opencv2/opencv.hpp>

void opencvMP4Test()
{
    cv::VideoCapture videoCapture;
    videoCapture.open(GET_CURRENT("/resources/test.mp4"));
    // check if we succeeded
    if (!videoCapture.isOpened()) {
        return;
    }
    auto fps = videoCapture.get(cv::CAP_PROP_FPS);
    LOG_INFO("the fps is {}", fps);
    cv::Mat frame;
    //--- GRAB AND WRITE LOOP
    int frameCount = 0;
    while (true)
    {
        //  wait for a new frame from camera and store it into 'frame'
        videoCapture.read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            LOG_INFO("ERROR! blank frame grabbed\n");
            return;
        }
        frameCount++;
        if (frameCount == int(videoCapture.get(cv::CAP_PROP_FRAME_COUNT))){
            frameCount = 0;
            videoCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
        }
        imshow("Live", frame);
        if (cv::waitKey(5) >= 0)
            break;
    }
    cv::waitKey(0);
}
