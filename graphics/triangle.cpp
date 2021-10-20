//
// Created by william on 2021/10/18.
//

#include "triangle.h"
namespace rst
{
Triangle::Triangle()
{
    memset(m_vertex, 0, sizeof(m_vertex));
    memset(m_color, 0, sizeof(m_color));
    memset(m_texCoords, 0, sizeof(m_texCoords));
    memset(m_normal, 0, sizeof(m_normal));
}
Triangle::~Triangle() = default;

void Triangle::setColor(int handle, float r, float g, float b)
{
    if ((r < 0.0f) || (r > 255.0f) || (g < 0.0f) || (g > 255.0f) || (b < 0.0f) || (b > 255.f))
    {
        throw std::runtime_error("Invalid color values");
    }
    m_color[handle] = Vector3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

std::array<Vector4f, 3> Triangle::toVector4() const
{
    std::array<Vector4f, 3> res;
    std::transform(std::begin(m_vertex), std::end(m_vertex), res.begin(), [](auto& vec) {
        return Vector4f(vec.x(), vec.y(), vec.z(), 1.0f);
    });
    return res;
}

} // namespace rst