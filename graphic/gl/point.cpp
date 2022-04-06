//
// Created by william on 2021/6/23.
//

#include "point.h"
namespace graphicEngine::gl
{
Point::~Point() = default;

void Point::initialize()
{
    initWithProperty(std::make_tuple("point", GET_CURRENT("/resources/shaders/point.gl.vert"), GET_CURRENT("/resources/shaders/point.gl.frag")));
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void Point::update(float elapseTime)
{
}

void Point::resize(int width, int height)
{
}

void Point::render()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindVertexArray(m_vao);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

} // namespace graphicEngine::gl