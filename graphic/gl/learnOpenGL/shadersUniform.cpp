//
// Created by william on 2022/4/5.
//

#include "shadersUniform.h"

#include "base.h"

namespace graphicEngine::gl
{
ShadersUniform::~ShadersUniform() = default;

void ShadersUniform::initialize()
{
    initWithProperty(std::make_tuple("ShadersUniform", GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/shadersUniform.frag")));
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void ShadersUniform::update(float elapseTime)
{
    auto greenValue = static_cast<float>(sin(elapseTime) / 2.0 + 0.5);
    m_program->setVector4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
}

void ShadersUniform::resize(int width, int height)
{
}

void ShadersUniform::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
} // namespace graphicEngine::gl