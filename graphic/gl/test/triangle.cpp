//
// Created by william on 2021/4/5.
//

#include "triangle.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace graphicEngine;

namespace graphicEngine::gl
{
void Triangle::initialize()
{
    initWithProperty(std::make_tuple("OpenGL Triangle", GET_CURRENT("/resources/shaders/triangle.gl.vert"), GET_CURRENT("/resources/shaders/triangle.gl.frag")));
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);
}

void Triangle::update(float elapseTime)
{
    glm::mat4 m = glm::mat4(1.0), p;
    glm::translate(m, glm::vec3(elapseTime));
    p = glm::ortho(-m_ratio, m_ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    m_mvpMatrix = p * m;
}

void Triangle::resize(int width, int height)
{
    m_ratio = (float)width / (float)height;
    glViewport(0.0f, 0.0f, width, height);
}

void Triangle::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glUniformMatrix4fv(glGetUniformLocation(m_program->getProgram(), "MVP"), 1, GL_FALSE, (const GLfloat*)&m_mvpMatrix);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Triangle::~Triangle() = default;

} // namespace graphicEngine::gl