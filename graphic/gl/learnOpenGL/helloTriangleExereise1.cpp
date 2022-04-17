//
// Created by william on 2022/4/5.
//

#include "helloTriangleExereise1.h"
#include "commonMacro.h"
#include "base.h"

namespace graphicEngine::gl
{

HelloTriangleExereise1::~HelloTriangleExereise1() = default;

void HelloTriangleExereise1::initialize()
{
    initWithProperty(std::make_tuple("HelloTriangleExereise1", GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.frag")));
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glBindVertexArray(0));
}

void HelloTriangleExereise1::update(float elapseTime)
{
}

void HelloTriangleExereise1::resize(int width, int height)
{
}

void HelloTriangleExereise1::render()
{
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
}
} // namespace graphicEngine::gl