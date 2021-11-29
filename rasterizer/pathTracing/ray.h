//
// Created by william on 2021/11/24.
//

#ifndef CPP_DEMO_RAY_H
#define CPP_DEMO_RAY_H
#include "vector3.h"
namespace graphics::pathTracing
{
class Ray
{
public:
    Ray(const Vector3& p, const Vector3& i);
    virtual ~Ray();

private:
    Vector3 m_position;
    Vector3 m_intensity;
};
} // namespace graphics

#endif //CPP_DEMO_RAY_H
