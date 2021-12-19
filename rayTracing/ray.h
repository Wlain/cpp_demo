//
// Created by william on 2021/12/19.
//

#ifndef CPP_DEMO_RAY_H
#define CPP_DEMO_RAY_H
#include "vec3.h"
namespace rayTracing
{
class Ray
{
public:
    Ray() {}
    Ray(const Vec3f& a, const Vec3f& b) :
        A(a), B(b) {}
    Vec3f origin() const { return A; }
    Vec3f direction() const { return B; }
    Vec3f pointAtParameter(float t) const { return A + t * B; }
    Vec3f A;
    Vec3f B;
};
} // namespace rayTracing
#endif //CPP_DEMO_RAY_H
