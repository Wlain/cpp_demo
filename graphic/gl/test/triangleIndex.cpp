//
// Created by william on 2021/4/5.
//

#include "triangleIndex.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace graphicEngine;

namespace graphicEngine::gl
{
TriangleIndex::~TriangleIndex()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void TriangleIndex::initialize()
{
    initWithProperty(std::make_tuple("OpenGL Triangle", GET_CURRENT("/resources/shaders/triangle.gl.vert"), GET_CURRENT("/resources/shaders/triangle.gl.frag")));
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void TriangleIndex::update(float elapseTime)
{
    glm::mat4 m =glm::mat4(1.0f), p;
    glm::rotate(m, elapseTime, glm::vec3(0.0f));
    p = glm::ortho(-m_ratio, m_ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    m_mvpMatrix = p * m;
}

void TriangleIndex::resize(int width, int height)
{
    m_ratio = (float)width / (float)height;
    glViewport(0.0f, 0.0f, width, height);
}

void TriangleIndex::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glUniformMatrix4fv(glGetUniformLocation(m_program->getProgram(), "MVP"), 1, GL_FALSE, (const GLfloat*)&m_mvpMatrix);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace graphicEngine::gl