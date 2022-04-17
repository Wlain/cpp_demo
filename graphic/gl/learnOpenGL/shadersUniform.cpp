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
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glBindVertexArray(0));
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
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));
}
} // namespace graphicEngine::gl