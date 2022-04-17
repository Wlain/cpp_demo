//
// Created by cwb on 2022/4/11.
//

#include "geometryShaderHouse.h"
namespace graphicEngine::gl
{

GeometryShaderHouse::~GeometryShaderHouse() = default;

void GeometryShaderHouse::initialize()
{
    m_points = {
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f // bottom-left
    };
    m_program = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/geometry.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/geometry.frag"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/geometry.gs.glsl"));
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_points[0]) * m_points.size(), m_points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}

void GeometryShaderHouse::resize(int width, int height)
{
}

void GeometryShaderHouse::update(float elapseTime)
{
}

void GeometryShaderHouse::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, 4);
}

} // namespace graphicEngine::gl