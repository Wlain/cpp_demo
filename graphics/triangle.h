//
// Created by william on 2021/10/18.
//

#ifndef CPP_DEMO_TRIANGLE_H
#define CPP_DEMO_TRIANGLE_H
#include <array>
#include <eigen3/Eigen/Eigen>
using namespace Eigen;
namespace rst
{
class Triangle
{
public:
    Triangle();
    ~Triangle();
    void setColor(int handle, float r, float g, float b);
    std::array<Vector4f, 3> toVector4() const;
    inline const Vector3f& x() const { return m_vertex[0]; }
    inline const Vector3f& y() const { return m_vertex[1]; }
    inline const Vector3f& z() const { return m_vertex[2]; }
    inline void setVertex(int handle, Vector3f v)
    {
        m_vertex[handle] = std::move(v);
    }
    inline void setTexCoord(int handle, float s, float t)
    {
        m_texCoords[handle] = Vector2f(s, t);
    }

    inline void setTexCoord(int handle, Vector2f f)
    {
        m_texCoords[handle] = std::move(f);
    }

    inline void setNormal(int handle, Vector3f n)
    {
        m_normal[handle] = std::move(n);
    }

private:
    Vector3f m_vertex[3];
    Vector3f m_color[3];
    Vector2f m_texCoords[2];
    Vector3f m_normal[3];
};
} // namespace rst

#endif //CPP_DEMO_TRIANGLE_H
