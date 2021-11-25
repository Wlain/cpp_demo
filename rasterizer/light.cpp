//
// Created by william on 2021/11/24.
//

#include "light.h"
namespace graphics::pathTracing
{
Light::Light(const Vector3& p, const Vector3& i) :
    m_position(p), m_intensity(i)
{
}

Light::~Light() = default;
} // namespace graphics
