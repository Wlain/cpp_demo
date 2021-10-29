//
// Created by william on 2021/10/24.
//

#include "shader.h"
using namespace Eigen;
namespace graphics
{
Shader::Shader() = default;
Shader::~Shader() = default;

VertexShader::VertexShader() = default;
VertexShader::~VertexShader() = default;

FragmentShader::FragmentShader() = default;

FragmentShader::FragmentShader(const Vector3f colors, const Vector3f normals, const Vector2f texCoords, std::shared_ptr<Texture> texture) :
    m_color(std::move(colors)), m_normal(std::move(normals)), m_texCoords(std::move(texCoords)), m_texture(std::move(texture))
{
}

FragmentShader::~FragmentShader() = default;
} // namespace graphics