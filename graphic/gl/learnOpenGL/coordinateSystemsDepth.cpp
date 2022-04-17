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
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube), &m_verticesCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->handle());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture1->handle());
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
} // namespace graphicEngine::gl