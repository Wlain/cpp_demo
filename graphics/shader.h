//
// Created by william on 2021/10/24.
//

#ifndef CPP_DEMO_SHADER_H
#define CPP_DEMO_SHADER_H

#include "texture.h"

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
    inline const Eigen::Vector4f& position() const { return m_position; }
    inline Eigen::Vector4f& position() { return m_position; }
    inline void setPosition(const Eigen::Vector4f& position) { m_position = position; }

private:
    Eigen::Vector4f m_position;
};

class FragmentShader : public Shader
{
public:
    FragmentShader();
    FragmentShader(const Eigen::Vector3f colors, const Eigen::Vector3f normals, const Eigen::Vector2f texCoords, std::shared_ptr<Texture> texture);
    ~FragmentShader() override;
    inline auto& viewPosition() { return m_viewPosition; }
    inline const auto& color() const { return m_color; }
    inline const auto& normal() const { return m_normal; }
    inline const auto& texCoords() const { return m_texCoords; }

public:
    std::shared_ptr<Texture> m_texture;
    Eigen::Vector4f m_viewPosition;
    Eigen::Vector3f m_color;
    Eigen::Vector3f m_normal;
    Eigen::Vector2f m_texCoords;
};

} // namespace graphics

#endif //CPP_DEMO_SHADER_H
