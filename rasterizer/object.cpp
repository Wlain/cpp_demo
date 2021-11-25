//
// Created by william on 2021/11/24.
//

#include "object.h"
namespace graphics::rayTracing
{
Object::Object() :
    m_materialType(MaterialType::DIFFUSE_AND_GLOSSY),
    m_ior(1.3f),
    m_kd(0.8f),
    m_ks(0.2f),
    m_diffuseColor(0.2),
    m_specularColorExponent(25)
{
}

Object::~Object() = default;

Vector3 Object::evalDiffuseColor(const Vector2&) const
{
    return m_diffuseColor;
}
} // namespace graphics