//
// Created by cwb on 2022/4/7.
//

#include "coordinateSystemsMultiple.h"

#include "textureGL.h"
namespace graphicEngine::gl
{
CoordinateSystemsMultiple::~CoordinateSystemsMultiple() = default;

void CoordinateSystemsMultiple::initialize()
{
    m_title = "CoordinateSystemsDepth";
    m_vertPath = GET_CURRENT("/resources/shaders/LearnOpenGL/coordinateSystemsMultiple.vert");
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

void CoordinateSystemsMultiple::update(float elapseTime)
{
    m_elapseTime = elapseTime;
    auto view = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);
    m_program->use();
    m_program->setMatrix4("view", view);
    m_program->setMatrix4("projection", projection);
}

void CoordinateSystemsMultiple::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void CoordinateSystemsMultiple::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->handle());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture1->handle());
    m_program->use();
    glBindVertexArray(m_vao);
    auto model = glm::mat4(1.0f);
    for (int i = 0; i < 10; ++i)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, m_cubePositions[i]);
        float angle = 20.0f * i;
        if (i % 3 == 0)
            angle = m_elapseTime * 25.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_program->setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
} // namespace graphicEngine::gl