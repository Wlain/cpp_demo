//
// Created by william on 2021/11/24.
//

#ifndef CPP_DEMO_OBJECT_H
#define CPP_DEMO_OBJECT_H
#include "base.h"
#include "vector2.h"
#include "vector3.h"

namespace graphics::rayTracing
{
class Object
{
public:
    enum class MaterialType
    {
        DIFFUSE_AND_GLOSSY,
        REFLECTION_AND_REFRACTION,
        REFLECTION
    };

public:
    Object();
    virtual ~Object();
    /// 判断是否相交
    virtual bool intersect(const Vector3& originPoint, const Vector3& dir, float& timeNear, uint32_t& index, Vector2& uv) = 0;
    virtual void getSurfaceProperties(const Vector3& hitPoint, const Vector3& dir, const uint32_t& index, const Vector2& uv, Vector3& normal, Vector2& st) const = 0;
    // 查询环境光颜色
    virtual Vector3 evalDiffuseColor(const Vector2&) const;

public:
    MaterialType m_materialType;
    Vector3 m_diffuseColor;
    float m_ior;
    float m_kd, m_ks;
    float m_specularColorExponent;
};
} // namespace graphics::rayTracing

#endif //CPP_DEMO_OBJECT_H
