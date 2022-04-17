//
// Created by william on 2021/10/24.
//

#include "shader.h"
#include "commonMacro.h"
using namespace Eigen;
namespace graphics
{
Shader::Shader()
{
    LOG_INFO("Shader constructor called");
}

Shader::~Shader()
{
    LOG_INFO("Shader destructor called");
}

Shader::Shader(const Shader&)
{
    LOG_INFO("Shader copy destructor called");
}

Shader::Shader(Shader&&)
{
    LOG_INFO("Shader move destructor called");
}

VertexShader::VertexShader() = default;

VertexShader::~VertexShader() = default;

void VertexShader::geometricProcessing()
{
}

FragmentShader::FragmentShader() = default;

FragmentShader::FragmentShader(VertexShader* vertexShader, Vector2f texCoords) :
    m_vertexShader(vertexShader), m_texCoords(std::move(texCoords))
{
}

FragmentShader::~FragmentShader() = default;
} // namespace graphics