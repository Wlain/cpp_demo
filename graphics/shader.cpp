//
// Created by william on 2021/10/24.
//

#include "shader.h"

namespace graphics
{
Shader::Shader() = default;
Shader::~Shader() = default;

VertexShader::VertexShader() = default;
VertexShader::~VertexShader() = default;

FragmentShader::FragmentShader() = default;

FragmentShader::FragmentShader(const Eigen::Matrix<float, 3, 1> color, const Eigen::Matrix<float, 3, 1> normal, const Eigen::Vector2f texCoords, std::shared_ptr<Texture> texture) :
    m_color(std::move(color)), m_normal(std::move(normal)), m_texCoords(std::move(texCoords)), m_texture(std::move(texture))
{
}

FragmentShader::~FragmentShader() = default;
} // namespace graphics