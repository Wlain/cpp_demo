//
// Created by william on 2021/10/18.
//

#include "triangle.h"
namespace graphics
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
    if ((r < 0.0f) || (r > 1.0f) || (g < 0.0f) || (g > 1.0f) || (b < 0.0f) || (b > 1.f))
    {
        throw std::runtime_error("Invalid color values");
    }
    m_color[handle] = Vector3f(r, g, b);
}

std::array<Vector4f, 3> Triangle::toVector4() const
{
    std::array<Vector4f, 3> res;
    std::transform(std::begin(m_vertex), std::end(m_vertex), res.begin(), [](auto& vec) {
        return Vector4f(vec.x(), vec.y(), vec.z(), 1.0f);
    });
    return res;
}

void Triangle::setNormals(const std::array<Vector3f, 3>& normals)
{
    m_normal[0] = normals[0];
    m_normal[1] = normals[1];
    m_normal[2] = normals[2];
}

void Triangle::setColors(const std::array<Vector3f, 3>& colors)
{
    setColor(0, colors[0][0], colors[0][1], colors[0][2]);
    setColor(1, colors[1][0], colors[1][1], colors[1][2]);
    setColor(2, colors[2][0], colors[2][1], colors[2][2]);
}

} // namespace graphics