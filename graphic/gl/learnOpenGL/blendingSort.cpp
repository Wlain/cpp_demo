//
// Created by cwb on 2022/4/8.
//

#include "blendingSort.h"

#include "camera.h"
#include "textureGL.h"
namespace graphicEngine::gl
{

BlendingSort::~BlendingSort()
{
    if (m_transparentVao != 0)
        glDeleteVertexArrays(1, &m_transparentVao);
    if (m_transparentVbo != 0)
        glDeleteBuffers(1, &m_transparentVbo);
}

void BlendingSort::initialize()
{
    m_transparentVertices = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f, 0.0f
    };
    m_vegetation = {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };
    DepthTest::initialize();
    glGenVertexArrays(1, &m_transparentVao);
    glGenBuffers(1, &m_transparentVbo);
    glBindVertexArray(m_transparentVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_transparentVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_transparentVertices[0]) * m_transparentVertices.size(), m_cubeVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    m_texture2 = std::make_unique<TextureGL>();
    m_texture2->createByFile(GET_CURRENT("/resources/textures/window.png"));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void BlendingSort::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    // sort the transparent windows before rendering
    for (const auto& window : m_vegetation)
    {
        float distance = glm::length(m_camera->m_position - window);
        m_sorted[distance] = window;
    }
}
void BlendingSort::resize(int width, int height)
{
    DepthTest::resize(width, height);
}
void BlendingSort::render()
{
    DepthTest::render();
    // vegetation
    glBindVertexArray(m_transparentVao);
    glBindTexture(GL_TEXTURE_2D, m_transparentVbo);
    for (auto it = m_sorted.rbegin(); it != m_sorted.rend(); ++it)
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        m_program->setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
} // namespace graphicEngine::gl