//
// Created by william on 2021/12/20.
//

#ifndef CPP_DEMO_CAMERA_H
#define CPP_DEMO_CAMERA_H

#include "ray.h"

namespace rayTracing
{
class Camera
{
public:
    Camera() = default;

    Ray getRay(float u, float v)
    {
        return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
    }

public:
    /// camera 相关
    Vec3f lowerLeftCorner{ -2.0f, -1.0f, -1.0f }; // 左下角
    Vec3f horizontal{ 4.0f, 0.0f, 0.0f };         // 横轴
    Vec3f vertical{ 0.0f, 2.0f, 0.0f };           // 竖轴
    Vec3f origin{ 0.0f, 0.0f, 0.0f };             // eye
};
} // namespace rayTracing

#endif //CPP_DEMO_CAMERA_H
