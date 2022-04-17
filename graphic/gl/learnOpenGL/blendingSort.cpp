//
// Created by cwb on 2022/4/8.
//

#include "blendingSort.h"

#include "common/camera.h"
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{

BlendingSort::~BlendingSort()
{
    if (m_transparentVao != 0)
        CHECK_GL(glDeleteVertexArrays(1, &m_transparentVao));
    if (m_transparentVbo != 0)
        CHECK_GL(glDeleteBuffers(1, &m_transparentVbo));
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

void BlendingSort::render()
{
    DepthTest::render();
    // vegetation
    CHECK_GL(glBindVertexArray(m_transparentVao));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_transparentVbo));
    for (auto it = m_sorted.rbegin(); it != m_sorted.rend(); ++it)
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        m_program->setMatrix4("model", model);
        CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    }
}

void BlendingSort::initVertices()
{
    DepthTest::initVertices();
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
}

void BlendingSort::initVertexAttrib()
{
    DepthTest::initVertexAttrib();
    CHECK_GL(glGenVertexArrays(1, &m_transparentVao));
    CHECK_GL(glGenBuffers(1, &m_transparentVbo));
    CHECK_GL(glBindVertexArray(m_transparentVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_transparentVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_transparentVertices[0]) * m_transparentVertices.size(), m_cubeVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glBindVertexArray(0));
}

void BlendingSort::initTextures()
{
    DepthTest::initTextures();
    m_texture2 = std::make_unique<TextureGL>();
    m_texture2->createByFile(GET_CURRENT("/resources/textures/window.png"));
}

void BlendingSort::initGLStatus()
{
    DepthTest::initGLStatus();
    CHECK_GL(glEnable(GL_BLEND));
    CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
} // namespace graphicEngine::gl