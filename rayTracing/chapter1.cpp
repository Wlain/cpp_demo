//
// Created by william on 2021/12/19.
//
#include <iostream>
#include <opencv2/opencv.hpp>

/// output an image
namespace rayTracing
{
void chapter1()
{
    int nx = 1280, ny = 720;
    int channels = 3;
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));
    float b = 0.2f;
    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            int ir = int(255.0 * r);
            int ig = int(255.0 * g);
            int ib = int(255.0 * b);
            // data[y*width*channels + x*channels + index]
            data[(ny - j - 1) * nx * 3 + 3 * i] = ir;
            data[(ny - j - 1) * nx * 3 + 3 * i + 1] = ig;
            data[(ny - j - 1) * nx * 3 + 3 * i + 2] = ib;
        }
    }
    cv::Mat image(ny, nx, CV_8UC3, (unsigned char*)data);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::imshow("chapter1", image);
    cv::waitKey();
    delete[] data;
}
} // namespace rayTracing