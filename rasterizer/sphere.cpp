//
// Created by william on 2021/11/24.
//

#include "sphere.h"
namespace graphics::rayTracing
{

Sphere::Sphere(const Vector3& c, float r):
m_center(c),
m_radius(r)
{
}

Sphere::Sphere(const Sphere&)
{
   std::cout << "Sphere copy" << std::endl;
}
Sphere::Sphere(const Sphere&&)
{
    std::cout << "Sphere move" << std::endl;
}

Sphere::~Sphere() = default;

bool Sphere::intersect(const Vector3& originPoint, const Vector3& dir, float& timeNear, uint32_t& index, Vector2& uv)
{
    return false;
}

void Sphere::getSurfaceProperties(const Vector3& hitPoint, const Vector3& dir, const uint32_t& index, const Vector2& uv, Vector3& normal, Vector2& st) const
{
}
}