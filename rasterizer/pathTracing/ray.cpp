//
// Created by william on 2021/11/24.
//

#include "ray.h"
namespace graphics::pathTracing
{
Ray::Ray(const Vector3& p, const Vector3& i) :
    m_position(p), m_intensity(i)
{
}

Ray::~Ray() = default;
} // namespace graphics
