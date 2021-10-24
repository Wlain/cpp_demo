//
// Created by william on 2021/10/24.
//

#ifndef CPP_DEMO_SHADER_H
#define CPP_DEMO_SHADER_H

#include "texture.h"

#include <eigen3/Eigen/Eigen>

namespace graphics
{
class Shader
{
public:
    Shader();
    virtual ~Shader();
};

class VertexShader : public Shader
{
public:
    VertexShader();
    ~VertexShader() override;
    inline const Eigen::Vector3f& position() const { return m_position; }
    inline Eigen::Vector3f& position() { return m_position; }
    inline void setPosition(const Eigen::Vector3f& position) { m_position = position; }

private:
    Eigen::Vector3f m_position;
};

class FragmentShader : public Shader
{
public:
    FragmentShader();
    FragmentShader(const Eigen::Vector3f colors, const Eigen::Vector3f normals, const Eigen::Vector2f texCoords, Texture* texture);
    ~FragmentShader() override;

public:
    Texture* m_texture = nullptr;
    Eigen::Vector3f m_viewPosition;
    Eigen::Vector3f m_color;
    Eigen::Vector3f m_normal;
    Eigen::Vector2f m_texCoords;
};

} // namespace graphics

#endif //CPP_DEMO_SHADER_H
