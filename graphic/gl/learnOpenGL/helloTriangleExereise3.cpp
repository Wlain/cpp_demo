//
// Created by william on 2022/4/5.
//

#include "helloTriangleExereise3.h"
#include "base.h"

namespace graphicEngine::gl
{

HelloTriangleExereise3::~HelloTriangleExereise3()
{
    CHECK_GL(glDeleteBuffers(2, m_vbos));
    CHECK_GL(glDeleteVertexArrays(2, m_vaos));
}

void HelloTriangleExereise3::initialize()
{
    initWithProperty(std::make_tuple("HelloTriangleExereise3", GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.frag")));
    // init program
    m_program2 = std::make_unique<graphicEngine::ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle2.frag"));
    CHECK_GL(glGenVertexArrays(2, m_vbos));
    CHECK_GL(glGenBuffers(2, m_vaos));
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

void HelloTriangleExereise3::update(float elapseTime)
{
}

void HelloTriangleExereise3::resize(int width, int height)
{
}

void HelloTriangleExereise3::render()
{
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glBindVertexArray(m_vaos[0]));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));
    CHECK_GL(glUseProgram(m_program2->getProgram()));
    CHECK_GL(glBindVertexArray(m_vaos[1]));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));
}
} // namespace graphicEngine::gl