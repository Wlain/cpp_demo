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
    Shader(const Shader&);
    Shader(Shader&&);
};

class VertexShader : public Shader
{
public:
    VertexShader();
    ~VertexShader() override;
    void geometricProcessing();
    inline const Eigen::Vector4f& position() const { return m_position; }
    inline Eigen::Vector4f& position() { return m_position; }
    inline void setPosition(Eigen::Vector4f position) { m_position = position; }
    inline Eigen::Vector4f& interpolatePos() { return m_interpolatePos; }
    inline std::map<std::string, Eigen::Vector4f*>& attrVec4Array() { return m_attrVec4Array; }
    inline std::map<std::string, Eigen::Matrix4f>& uniformMatrix() { return m_uniformMatrix; }

private:
    std::map<std::string, Eigen::Vector4f*> m_attrVec4Array;
    std::map<std::string, Eigen::Matrix4f> m_uniformMatrix;
    Eigen::Vector4f m_interpolatePos;
    Eigen::Vector4f m_position;
};

class FragmentShader : public Shader
{
public:
    FragmentShader();
    FragmentShader(VertexShader* vertexShader, Eigen::Vector2f texCoords);
    ~FragmentShader() override;
    inline const auto& viewPosition() const { return m_viewPosition; }
    inline auto& viewPosition() { return m_viewPosition; }
    inline const auto& color() const { return m_color; }
    inline auto& color() { return m_color; }
    inline const auto& normal() const { return m_normal; }
    inline auto& normal() { return m_normal; }
    inline const auto& texCoords() const { return m_texCoords; }
    inline auto& texCoords() { return m_texCoords; }
    inline const auto& texture() const { return m_texture; }
    inline auto& texture() { return m_texture; }
    inline void setUniformFloat(float value) { m_uniformFloat = value; }
    inline void setVertexShader(const std::shared_ptr<VertexShader>& vertexShader) { m_vertexShader = vertexShader; }

public:
    std::shared_ptr<VertexShader> m_vertexShader;
    std::shared_ptr<Texture> m_texture;
    Eigen::Vector4f m_viewPosition;
    Eigen::Vector3f m_color;
    Eigen::Vector3f m_normal;
    Eigen::Vector2f m_texCoords;
    float m_uniformFloat = 0.0f;
};

} // namespace graphics

#endif //CPP_DEMO_SHADER_H
