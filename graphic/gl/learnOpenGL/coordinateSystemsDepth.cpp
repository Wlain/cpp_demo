//
// Created by cwb on 2022/4/7.
//

#include "coordinateSystemsDepth.h"

#include "gl/common/textureGL.h"

#include <glm/gtc/matrix_transform.hpp>
namespace graphicEngine::gl
{
CoordinateSystemsDepth::~CoordinateSystemsDepth() = default;
void CoordinateSystemsDepth::initialize()
{
    m_title = "CoordinateSystemsDepth";
    m_vertPath = GET_CURRENT("/resources/shaders/LearnOpenGL/transformations.vert");
    m_texture->createByFile(GET_CURRENT("/resources/textures/container.jpg"));
    m_texture1->createByFile(GET_CURRENT("/resources/textures/awesomeface.png"));
    initWithProperty();
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube), &m_verticesCube, GL_STATIC_DRAW));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glBindVertexArray(0));
    m_program->use();
    m_program->setInt("inputTexture", 0);
    m_program->setInt("inputTexture1", 1);
    m_program->setFloat("mixValue", 0.2f);
}

void CoordinateSystemsDepth::update(float elapseTime)
{
    auto model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    auto view = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);
    model = glm::rotate(model, elapseTime, glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);
    m_program->use();
    m_program->setMatrix4("transform", projection * view * model);
}

void CoordinateSystemsDepth::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void CoordinateSystemsDepth::render()
{
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glEnable(GL_DEPTH_TEST));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture->handle()));
    CHECK_GL(glActiveTexture(GL_TEXTURE1));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture1->handle()));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
}
} // namespace graphicEngine::gl