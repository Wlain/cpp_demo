//
// Created by william on 2021/12/19.
//

//
// Created by william on 2021/12/19.
//

#include "ray.h"

#include <iostream>
#include <opencv2/opencv.hpp>

/// output an image
namespace rayTracing
{
bool hitSphere(const Vec3f& center, float radius, const Ray& r)
{
    Vec3f A_C = r.origin() - center;
    Vec3f B = r.direction();
    float a = B.dotProduct(B);
    float b = 2.0f * A_C.dotProduct(B);
    float c = A_C.dotProduct(A_C) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

Vec3f colorHitSphere(const Ray& r)
{
    /// 如果 ray 击中球体的话，返回球体的颜色
    if(hitSphere({0.0f, 0.0f, -1.0f}, 0.5, r))
        return {1.0, 0.0, 0.0};
    /// 否则返回背景色
    Vec3f unitDirection = r.direction().normalize();
    float t = (0.5f * unitDirection.y + 1.0f); // [0, 2]-> [-1, 1]
    auto startColor = Vec3f(1.0f, 1.0f, 1.0f);
    auto endColor = Vec3f(0.5f, 0.7f, 1.0f);
    return (1.0f - t) * startColor + t * endColor;
}

void chapter4()
{
    int nx = 1280, ny = 640;
    int channels = 3;
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));

    /// camera 相关
    Vec3f lowerLeftCorner{ -2.0f, -1.0f, -1.0f }; // 左下角
    Vec3f horizontal{ 4.0f, 0.0f, 0.0f };         // 横轴
    Vec3f vertical{ 0.0f, 2.0f, 0.0f };           // 竖轴
    Vec3f origin{ 0.0f, 0.0f, 0.0f };             // eye

    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            /// 确定 ray r;
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

            /// 计算ray r返回的颜色
            Vec3f col = colorHitSphere(r);
            int ir = int(255.0 * col.r());
            int ig = int(255.0 * col.g());
            int ib = int(255.0 * col.b());
            // data[y*width*channels + x*channels + index]
            data[(ny - j - 1) * nx * 3 + 3 * i] = ir;
            data[(ny - j - 1) * nx * 3 + 3 * i + 1] = ig;
            data[(ny - j - 1) * nx * 3 + 3 * i + 2] = ib;
        }
        // print渲染进度
        std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
    }
    cv::Mat image(ny, nx, CV_8UC3, (unsigned char*)data);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::imshow("chapter4", image);
    cv::waitKey();
    delete[] data;
}
} // namespace rayTracing