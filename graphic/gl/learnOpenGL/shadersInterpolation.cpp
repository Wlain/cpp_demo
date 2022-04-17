//
// Created by william on 2022/4/5.
//

#include "shadersInterpolation.h"

#include "base.h"

namespace graphicEngine::gl
{

ShadersInterpolation::~ShadersInterpolation() = default;

void ShadersInterpolation::initialize()
{
    initWithProperty(std::make_tuple("ShadersInterpolation", GET_CURRENT("/resources/shaders/LearnOpenGL/shadersInterpolation.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/shadersInterpolation.frag")));
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW));
    // position attribute
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(0));
    // color attribute
    CHECK_GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glBindVertexArray(0));
}

void ShadersInterpolation::update(float elapseTime)
{
}

void ShadersInterpolation::resize(int width, int height)
{
}

void ShadersInterpolation::render()
{
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));
}
} // namespace graphicEngine::gl