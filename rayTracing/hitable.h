//
// Created by william on 2021/12/20.
//

#ifndef CPP_DEMO_HITABLE_H
#define CPP_DEMO_HITABLE_H
#include "ray.h"

namespace rayTracing
{
class Material;

// 记录交点信息
struct HitRecord
{
    float t;
    Vec3f p;
    Vec3f normal;
    Material *material;
};

class HitAble
{
public:
    HitAble() = default;
    virtual ~HitAble() = default;

    virtual bool hit(const Ray& r, float timeMin, float timeMax, HitRecord& record) const = 0;
};
} // namespace rayTracing

#endif //CPP_DEMO_HITABLE_H
