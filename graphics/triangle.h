//
// Created by william on 2021/10/18.
//

#ifndef CPP_DEMO_TRIANGLE_H
#define CPP_DEMO_TRIANGLE_H
#include <array>
#include <eigen3/Eigen/Eigen>
using namespace Eigen;
namespace graphics
{
class Triangle
{
public:
    Triangle();
    ~Triangle();
    std::array<Vector4f, 3> toVector4() const;
    void setColor(int handle, float r, float g, float b);
    inline const Vector3f* color() const { return m_color; }
    inline const Vector2f* texCoords() const { return m_texCoords; }
    inline const Vector3f& x() const { return m_vertex[0]; }
    inline const Vector3f& y() const { return m_vertex[1]; }
    inline const Vector3f& z() const { return m_vertex[2]; }
    inline void setVertex(int handle, Vector3f v) { m_vertex[handle] = std::move(v); }
    inline const Vector3f* vertex() const  { return m_vertex; }
    inline const Vector3f* normal() const  { return m_normal; }
    inline void setTexCoord(int handle, float s, float t) { m_texCoords[handle] = Vector2f(s, t); }
    inline void setTexCoord(int handle, Vector2f f) { m_texCoords[handle] = std::move(f); }
    inline void setNormal(int handle, Vector3f n) { m_normal[handle] = std::move(n); }

    void setNormals(const std::array<Vector3f, 3>& normals);
    void setColors(const std::array<Vector3f, 3>& colors);


private:
    Vector3f m_vertex[3];
    Vector3f m_color[3];
    Vector2f m_texCoords[2];
    Vector3f m_normal[3];
};
} // namespace graphics

#endif //CPP_DEMO_TRIANGLE_H
