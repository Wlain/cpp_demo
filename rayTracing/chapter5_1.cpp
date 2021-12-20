//
// Created by william on 2021/12/20.
//

#include "hitableList.h"
#include "sphere.h"

#include <iostream>
#include <opencv2/opencv.hpp>

/// 球心坐标：(0, 0, -1)

namespace rayTracing
{
Vec3f color(const Ray& r, HitAble* world)
{
    HitRecord record;
    if (world->hit(r, 0.0, FLT_MAX, record))
    {
        // 若 ray 交到场景中任意 object，计算其交点法线并可视化
        /// 归一化
        return 0.5 * (record.normal + 1.0f);
    }
    else
    {
        /// 否则返回背景色
        Vec3f unitDirection = r.direction().normalize();
        float t = (0.5f * unitDirection.y + 1.0f); // [0, 2]-> [-1, 1]
        auto startColor = Vec3f(1.0f, 1.0f, 1.0f);
        auto endColor = Vec3f(0.5f, 0.7f, 1.0f);
        return (1.0f - t) * startColor + t * endColor;
    }
}

void chapter5_1()
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

    /// 场景相关
    HitAble* list[2];
    list[0] = new Sphere({0.0f, 0.0f, -1.0f}, 0.5);
    list[1] = new Sphere({0.0f, -100.5f, -1.0f}, 100.0f);
    HitAble *world = new HitAbleList(list, 2);

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
            Vec3f col = color(r, world);
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
    cv::imshow("chapter5", image);
    cv::waitKey();
    delete[] data;
    delete[] *list;
    delete world;
}
} // namespace rayTracing