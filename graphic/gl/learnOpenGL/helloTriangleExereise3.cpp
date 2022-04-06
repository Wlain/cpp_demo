//
// Created by william on 2022/4/5.
//

#include "helloTriangleExereise3.h"
#include "base.h"

namespace graphicEngine::gl
{

HelloTriangleExereise3::~HelloTriangleExereise3()
{
    glDeleteBuffers(2, m_vbos);
    glDeleteVertexArrays(2, m_vaos);
}

void HelloTriangleExereise3::initialize()
{
    initWithProperty(std::make_tuple("HelloTriangleExereise3", GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.frag")));
    // init program
    m_program2 = std::make_unique<graphicEngine::Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle2.frag"));
    glGenVertexArrays(2, m_vbos);
    glGenBuffers(2, m_vaos);
    glBindVertexArray(m_vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesTriangle1), &m_verticesTriangle1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(m_vaos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesTriangle2), &m_verticesTriangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void HelloTriangleExereise3::update(float elapseTime)
{
}

void HelloTriangleExereise3::resize(int width, int height)
{
}

void HelloTriangleExereise3::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindVertexArray(m_vaos[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(m_program2->getProgram());
    glBindVertexArray(m_vaos[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
} // namespace graphicEngine::gl