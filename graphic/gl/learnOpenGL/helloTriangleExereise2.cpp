//
// Created by william on 2022/4/5.
//

#include "helloTriangleExereise2.h"

#include "base.h"

namespace graphicEngine::gl
{
HelloTriangleExereise2::~HelloTriangleExereise2() = default;

void HelloTriangleExereise2::initialize()
{
    initWithProperty(std::make_tuple("HelloTriangleExereise2", GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.frag")));
    CHECK_GL(glGenVertexArrays(2, m_vaos));
    CHECK_GL(glGenBuffers(2, m_vbos));
    CHECK_GL(glBindVertexArray(m_vaos[0]));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesTriangle1), &m_verticesTriangle1, GL_STATIC_DRAW));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glBindVertexArray(m_vaos[1]));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesTriangle2), &m_verticesTriangle2, GL_STATIC_DRAW));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glBindVertexArray(0));
}

void HelloTriangleExereise2::update(float elapseTime)
{
}

void HelloTriangleExereise2::resize(int width, int height)
{
}

void HelloTriangleExereise2::render()
{
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glBindVertexArray(m_vaos[0]));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));
    CHECK_GL(glBindVertexArray(m_vaos[1]));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));
}
} // namespace graphicEngine::gl