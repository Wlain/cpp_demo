//
// Created by william on 2021/12/20.
//

#ifndef CPP_DEMO_CAMERA_H
#define CPP_DEMO_CAMERA_H

#include "ray.h"

namespace rayTracing
{
Vec3f randomInUnitDist()
{
    Vec3f p;
    do
    {
        p = 2.0f * Vec3f(rand() % 100 / 100.0f, rand() % 100 / 100.0f, 0.0f) - Vec3f(1.0f, 1.0f, 0.0f);
    } while (p.dotProduct(p) >= 1.0f);
    return p;
}

class Camera
{
public:
    Camera() = default;
    Camera(Vec3f eyePos, Vec3f target, Vec3f up, float fov, float aspect)
    {
        Vec3f u, v, w;
        float theta = fov * M_PI / 180.0f;
        float halfHeight = tan(theta / 2);
        float halfWidth = aspect * halfHeight;
        origin = eyePos;
        w = (eyePos - target).normalize();
        u = up.crossProduct(w).normalize();
        v = w.crossProduct(u);
        lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w; // 左下角
        horizontal = 2.0f * halfWidth * u;                             // 横轴
        vertical = 2.0f * halfHeight * v;                              // 竖轴
    }

    /// aperture:光圈，focusDist:焦距
    Camera(Vec3f eyePos, Vec3f target, Vec3f up, float fov, float aspect, float aperture, float focusDist)
    {
        lensRadius = aperture / 2.0f;
        float theta = fov * M_PI / 180.0f;
        float halfHeight = tan(theta / 2);
        float halfWidth = aspect * halfHeight;
        origin = eyePos;
        w = (eyePos - target).normalize();
        u = up.crossProduct(w).normalize();
        v = w.crossProduct(u);
        lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w; // 左下角
        horizontal = 2.0f * halfWidth * focusDist * u;                                                     // 横轴
        vertical = 2.0f * halfHeight * focusDist * v;                                                      // 竖轴
    }

    Ray getRay(float s, float t)
    {
        Vec3f rd = lensRadius * randomInUnitDist();
        Vec3f offset = u * rd.x + v * rd.y;
        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
    }

public:
    /// camera 相关
    Vec3f lowerLeftCorner{ -2.0f, -1.0f, -1.0f }; // 左下角
    Vec3f horizontal{ 4.0f, 0.0f, 0.0f };         // 横轴
    Vec3f vertical{ 0.0f, 2.0f, 0.0f };           // 竖轴
    Vec3f origin{ 0.0f, 0.0f, 0.0f };             // eye
    Vec3f u, v, w;
    float lensRadius;
};
} // namespace rayTracing

#endif //CPP_DEMO_CAMERA_H
