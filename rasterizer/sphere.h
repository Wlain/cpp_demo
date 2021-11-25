//
// Created by william on 2021/11/24.
//

#ifndef CPP_DEMO_SPHERE_H
#define CPP_DEMO_SPHERE_H
#include "object.h"
namespace graphics::rayTracing
{
class Sphere : public Object
{
public:
    Sphere(const Vector3& c, float r);
    Sphere(const Sphere&);
    Sphere(const Sphere&&);
    ~Sphere() override;
    bool intersect(const Vector3& originPoint, const Vector3& dir, float& timeNear, uint32_t& index, Vector2& uv) override;
    void getSurfaceProperties(const Vector3& hitPoint, const Vector3& dir, const uint32_t& index, const Vector2& uv, Vector3& normal, Vector2& st) const override;

private:
    Vector3 m_center;
    float m_radius;
};
} // namespace graphics::rayTracing

#endif //CPP_DEMO_SPHERE_H
