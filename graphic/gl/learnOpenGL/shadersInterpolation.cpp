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
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void ShadersInterpolation::update(float elapseTime)
{
}

void ShadersInterpolation::resize(int width, int height)
{
}

void ShadersInterpolation::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
} // namespace graphicEngine::gl