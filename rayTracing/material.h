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

/// 理想反射
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

/// 金属
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

/// 折射：折射定律
bool refract(const Vec3f& v, const Vec3f& n, float niOverNt, Vec3f& refracted)
{
    Vec3f uv = v;
    uv.normalize();
    float dt = uv.dotProduct(n);
    /// niOverNt：ni/nt
    float discriminant = 1.0f - niOverNt * niOverNt * (1 - dt * dt);
    // 根据：0 << sin(a) << 1推导
    /// 折射存在
    if (discriminant >= 0)
    {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

float schlick(float cosine, float refIndex)
{
    float r0 = (1.0f - refIndex) / (1.0f + refIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}

/// 电介质
class Dielectric : public Material
{
public:
    Dielectric(float ri) :
        refIndex(ri) {}
    bool scatter(const Ray& r, const HitRecord& record, Vec3f& attenuation, Ray& scattered) const override
    {
        Vec3f outwardNormal;
        Vec3f reflected = reflect(r.direction(), record.normal);
        float niOverNt;
        attenuation = Vec3f(1.0f, 1.0f, 1.0f);
        Vec3f refracted;
        float reflectProb;
        float cosine;
        if (r.direction().dotProduct(record.normal) > 0)
        {
            outwardNormal = -record.normal;
            niOverNt = refIndex;
            cosine = refIndex * r.direction().dotProduct(record.normal) / r.direction().length();
        }
        else
        {
            outwardNormal = record.normal;
            niOverNt = 1.0f / refIndex;
            cosine = -r.direction().dotProduct(record.normal) / r.direction().length();
        }
        if (refract(r.direction(), outwardNormal, niOverNt, refracted))
        {
            reflectProb = schlick(cosine, refIndex);
        }
        else
        {
            scattered = Ray(record.p, reflected);
            reflectProb = 1.0;
        }

        if ((rand() % 100 / float(100)) < reflectProb)
        {
            scattered = Ray(record.p, reflected);
        }
        else
        {
            scattered = Ray(record.p, refracted);
        }
        return true;
    }

public:
    float refIndex;
};

} // namespace rayTracing

#endif //CPP_DEMO_MATERIAL_H
