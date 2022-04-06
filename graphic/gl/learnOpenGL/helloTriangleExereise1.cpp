//
// Created by william on 2022/4/5.
//

#include "helloTriangleExereise1.h"

#include "base.h"

namespace graphicEngine::gl
{

HelloTriangleExereise1::~HelloTriangleExereise1() = default;

void HelloTriangleExereise1::initialize()
{
    initWithProperty(std::make_tuple("HelloTriangleExereise1", GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/helloTriangle.frag")));
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void HelloTriangleExereise1::update(float elapseTime)
{
}

void HelloTriangleExereise1::resize(int width, int height)
{
}

void HelloTriangleExereise1::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
} // namespace graphicEngine::gl