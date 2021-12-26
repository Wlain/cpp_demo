//
// Created by william on 2021/12/20.
//

#include "camera.h"
#include "hitableList.h"
#include "material.h"
#include "sphere.h"

#include <iostream>
#include <opencv2/opencv.hpp>

/// 球心坐标：(0, 0, -1)

namespace rayTracing
{
Vec3f color(const Ray& r, HitAble* world)
{
    HitRecord record;
    if (world->hit(r, 0.001, FLT_MAX, record))
    {
        // 若 ray 交到场景中 diffuse 物体，trace 新的 ray
        Vec3 target = record.p + record.normal + randomInUnitSphere();
        /// 0.5 是 吸收率
        return 0.5 * color(Ray(record.p, target - record.p), world);
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

Vec3f color(const Ray& r, HitAble* world, int depth)
{
    HitRecord record;
    if (world->hit(r, 0.001, FLT_MAX, record))
    {
        Ray scattered;
        Vec3f attenuation;
        /// 最多弹射50次
        if (depth < 50 && record.material->scatter(r, record, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return Vec3f(0, 0, 0);
        }
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

/// 数据抽象
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
    list[0] = new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5);
    list[1] = new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f);
    HitAble* world = new HitAbleList(list, 2);

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
    delete[] * list;
    delete world;
}

/// 多重采样抗锯齿
void chapter6()
{
    int nx = 1280, ny = 640;
    int channels = 3;
    int ns = 100; // sample count
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));

    /// camera 相关
    Camera cam;

    /// 场景相关
    HitAble* list[2];
    list[0] = new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5);
    list[1] = new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f);
    HitAble* world = new HitAbleList(list, 2);
    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vec3f col(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + (rand() % 100 / 100.0f)) / float(nx);
                float v = float(j + (rand() % 100 / 100.0f)) / float(ny);
                /// 确定 ray r;
                Ray r = cam.getRay(u, v);
                /// 累加 ray r 射入场景 world 后，返回的颜色
                col += color(r, world);
            }
            col /= float(ns);
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());
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
    cv::imshow("chapter6", image);
    cv::waitKey();
    delete[] data;
    delete[] * list;
    delete world;
}

/// 漫反射魔表
void chapter7()
{
    int nx = 1280, ny = 640;
    int channels = 3;
    int ns = 100; // sample count
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));

    /// camera 相关
    Camera cam;

    /// 场景相关
    HitAble* list[2];
    list[0] = new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5);
    list[1] = new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f);
    HitAble* world = new HitAbleList(list, 2);
    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vec3f col(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + (rand() % 100 / 100.0f)) / float(nx);
                float v = float(j + (rand() % 100 / 100.0f)) / float(ny);
                /// 确定 ray r;
                Ray r = cam.getRay(u, v);
                /// 累加 ray r 射入场景 world 后，返回的颜色
                col += color(r, world);
            }
            col /= float(ns);
            // gammar 矫正
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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
    cv::imshow("chapter7", image);
    cv::waitKey();
    delete[] data;
    delete[] * list;
    delete world;
}

/// 材质
void chapter8()
{
    int nx = 1280, ny = 640;
    int channels = 3;
    int ns = 100; // sample count
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));

    /// camera 相关
    Camera cam;

    /// 场景相关
    HitAble* list[4];
    list[0] = new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5f, new Lambertian({ 0.8f, 0.3f, 0.3f }));
    list[1] = new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f, new Lambertian({ 0.8f, 0.8f, 0.3f }));
    list[2] = new Sphere({ 1.0f, 0.0f, -1.0f }, 0.5f, new Metal({ 0.8f, 0.6f, 0.2f }, 0.3f));
    list[3] = new Sphere({ -1.0f, 0.0f, -1.0f }, 0.5f, new Metal({ 0.8f, 0.8f, 0.8f }, 1.0f));
    HitAble* world = new HitAbleList(list, 4);
    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vec3f col(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + (rand() % 100 / 100.0f)) / float(nx);
                float v = float(j + (rand() % 100 / 100.0f)) / float(ny);
                /// 确定 ray r;
                Ray r = cam.getRay(u, v);
                /// 累加 ray r 射入场景 world 后，返回的颜色
                col += color(r, world, 0);
            }
            col /= float(ns);
            // gammar 矫正
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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
    cv::imshow("chapter8", image);
    cv::waitKey();
    delete[] data;
    delete[] * list;
    delete world;
}

/// 折射材质
void chapter9()
{
    int nx = 1280, ny = 640;
    int channels = 3;
    int ns = 100; // sample count
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));

    /// camera 相关
    Camera cam;

    /// 场景相关
    HitAble* list[5];
    list[0] = new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5f, new Lambertian({ 0.8f, 0.3f, 0.3f }));
    list[1] = new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f, new Lambertian({ 0.8f, 0.8f, 0.0f }));
    list[2] = new Sphere({ 1.0f, 0.0f, -1.0f }, 0.5f, new Metal({ 0.8f, 0.6f, 0.2f }, 0.3f));
    list[3] = new Sphere({ -1.0f, 0.0f, -1.0f }, 0.5f, new Dielectric(1.5f));
    list[4] = new Sphere({ -1.0f, 0.0f, -1.0f }, -0.45, new Dielectric(1.5));
    HitAble* world = new HitAbleList(list, 5);
    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vec3f col(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + (rand() % 100 / 100.0f)) / float(nx);
                float v = float(j + (rand() % 100 / 100.0f)) / float(ny);
                /// 确定 ray r;
                Ray r = cam.getRay(u, v);
                /// 累加 ray r 射入场景 world 后，返回的颜色
                col += color(r, world, 0);
            }
            col /= float(ns);
            // gammar 矫正
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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
    cv::imshow("chapter9", image);
    cv::waitKey();
    delete[] data;
    delete[] * list;
    delete world;
}

/// camera 模型
void chapter10()
{
    int nx = 1280, ny = 640;
    int channels = 3;
    int ns = 100; // sample count
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));

    /// camera 相关
    Camera cam({ -2.0f, 2.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, 90.0f, float(nx) / float(ny));

    /// 场景相关
    HitAble* list[5];
    list[0] = new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5f, new Lambertian({ 0.8f, 0.3f, 0.3f }));
    list[1] = new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f, new Lambertian({ 0.8f, 0.8f, 0.0f }));
    list[2] = new Sphere({ 1.0f, 0.0f, -1.0f }, 0.5f, new Metal({ 0.8f, 0.6f, 0.2f }, 0.3f));
    list[3] = new Sphere({ -1.0f, 0.0f, -1.0f }, 0.5f, new Dielectric(1.5f));
    list[4] = new Sphere({ -1.0f, 0.0f, -1.0f }, -0.45, new Dielectric(1.5));
    HitAble* world = new HitAbleList(list, 5);
    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vec3f col(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + (rand() % 100 / 100.0f)) / float(nx);
                float v = float(j + (rand() % 100 / 100.0f)) / float(ny);
                /// 确定 ray r;
                Ray r = cam.getRay(u, v);
                /// 累加 ray r 射入场景 world 后，返回的颜色
                col += color(r, world, 0);
            }
            col /= float(ns);
            // gammar 矫正
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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
    cv::imshow("chapter10", image);
    cv::waitKey();
    delete[] data;
    delete[] * list;
    delete world;
}

/// camera 模型
void chapter11()
{
    int nx = 1280, ny = 640;
    int channels = 3;
    int ns = 100; // sample count
    auto* data = new unsigned char[nx * ny * channels];
    std::memset(data, 0, nx * ny * channels * sizeof(unsigned char));

    /// camera 相关
    Vec3f lookFrom{ 3.0f, 3.0f, 2.0f };
    Vec3f lookAt{ 0.0f, 0.0f, -1.0f };
    float distToFocus = (lookFrom - lookAt).length();
    float aperture = 2.0f;
    Camera cam(lookFrom, lookAt, { 0.0f, 1.0f, 0.0f }, 20.0f, float(nx) / float(ny), aperture, distToFocus);

    /// 场景相关
    HitAble* list[5];
    list[0] = new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5f, new Lambertian({ 0.8f, 0.3f, 0.3f }));
    list[1] = new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f, new Lambertian({ 0.8f, 0.8f, 0.0f }));
    list[2] = new Sphere({ 1.0f, 0.0f, -1.0f }, 0.5f, new Metal({ 0.8f, 0.6f, 0.2f }, 0.3f));
    list[3] = new Sphere({ -1.0f, 0.0f, -1.0f }, 0.5f, new Dielectric(1.5f));
    list[4] = new Sphere({ -1.0f, 0.0f, -1.0f }, -0.45, new Dielectric(1.5));
    HitAble* world = new HitAbleList(list, 5);
    /// 遍历元素
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vec3f col(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + (rand() % 100 / 100.0f)) / float(nx);
                float v = float(j + (rand() % 100 / 100.0f)) / float(ny);
                /// 确定 ray r;
                Ray r = cam.getRay(u, v);
                /// 累加 ray r 射入场景 world 后，返回的颜色
                col += color(r, world, 0);
            }
            col /= float(ns);
            // gammar 矫正
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
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
    cv::imshow("chapter11", image);
    cv::waitKey();
    delete[] data;
    delete[] * list;
    delete world;
}

} // namespace rayTracing