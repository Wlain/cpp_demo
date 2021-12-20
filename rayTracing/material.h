//
// Created by william on 2021/12/20.
//

#ifndef CPP_DEMO_MATERIAL_H
#define CPP_DEMO_MATERIAL_H
#include "hitable.h"

namespace rayTracing
{
Vec3f randomInUnitSphere()
{
    Vec3f p;
    do
    {
        p = 2.0f * Vec3((rand() % 100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - Vec3(1.0f, 1.0f, 1.0f);
    } while (p.norm() >= 1.0f);
    return p;
}

class Material
{
public:
    /// 分散
    virtual bool scatter(const Ray& r, const HitRecord& record, Vec3f& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Vec3f& a) :
        albedo(a) {}
    bool scatter(const Ray& r, const HitRecord& record, Vec3f& attenuation, Ray& scattered) const override
    {
        Vec3f target = record.p + record.normal + randomInUnitSphere();
        scattered = Ray(record.p, target - record.p);
        attenuation = albedo;
        return true;
    }

public:
    Vec3f albedo; /// 反射率
};

class Metal : public Material
{
public:
    Metal(const Vec3f& a, float f) :
        albedo(a), fuzz(f) {}
    bool scatter(const Ray& r, const HitRecord& record, Vec3f& attenuation, Ray& scattered) const override
    {
        Vec3f reflected = reflect(r.direction().normalize(), record.normal);
        /// 在单位球内，随机取一点S进行扰动
        scattered = Ray(record.p, reflected + fuzz * randomInUnitSphere());
        attenuation = albedo;
        /// 反射出的光线若与 normal 夹角大于90度，则不正确
        return (scattered.direction().dotProduct(record.normal) > 0);
    }

public:
    Vec3f albedo; /// 反射率
    float fuzz;   /// 模糊
};
} // namespace rayTracing

#endif //CPP_DEMO_MATERIAL_H
