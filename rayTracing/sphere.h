//
// Created by william on 2021/12/20.
//

#ifndef CPP_DEMO_SPHERE_H
#define CPP_DEMO_SPHERE_H
#include "hitable.h"
#include "ray.h"
namespace rayTracing
{
class Sphere : public HitAble
{
public:
    Sphere() = default;
    Sphere(Vec3f _center, float _radius) :
        center(_center), radius(_radius) {}

    Sphere(Vec3f _center, float _radius, Material *m) :
        center(_center), radius(_radius), material(m) {}

    ~Sphere()
    {
        if (material)
        {
            delete material;
        }
    }
    bool hit(const Ray& r, float timeMin, float timeMax, HitRecord& record) const override
    {
        Vec3f A_C = r.origin() - center;
        Vec3f B = r.direction();
        float a = B.dotProduct(B);
        float b = 2.0f * A_C.dotProduct(B);
        float c = A_C.dotProduct(A_C) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant > 0)
        {
            float temp = (-b - std::sqrt(discriminant)) / (2.0f * a);
            if (temp < timeMax && temp > timeMin)
            {
                record.t = temp;
                record.p = r.pointAtParameter(temp);
                record.normal = (record.p - center) / radius;
                record.material = material;
                return true;
            }
            temp = (-b + std::sqrt(discriminant)) / (2.0f * a);
            if (temp < timeMax && temp > timeMin)
            {
                record.t = temp;
                record.p = r.pointAtParameter(temp);
                record.normal = (record.p - center) / radius;
                record.material = material;
                return true;
            }
        }
        return false;
    }

public:
    Vec3f center;
    float radius;
    Material *material;
};
} // namespace rayTracing

#endif //CPP_DEMO_SPHERE_H
