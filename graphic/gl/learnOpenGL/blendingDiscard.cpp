//
// Created by cwb on 2022/4/8.
//

#include "blendingDiscard.h"
#include <glm/gtc/matrix_transform.hpp>
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{

BlendingDiscard::~BlendingDiscard()
{
    if (m_transparentVao != 0)
        glDeleteVertexArrays(1, &m_transparentVao);
    if (m_transparentVbo != 0)
        glDeleteBuffers(1, &m_transparentVbo);
}

void BlendingDiscard::initialize()
{
    DepthTest::initialize();
}

void BlendingDiscard::render()
{
    DepthTest::render();
    // vegetation
    glBindVertexArray(m_transparentVao);
    glBindTexture(GL_TEXTURE_2D, m_transparentVbo);
    for (auto & i : m_vegetation)
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, i);
        m_program->setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
void BlendingDiscard::initVertices()
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
void BlendingDiscard::initVertexAttrib()
{
    DepthTest::initVertexAttrib();
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
}
void BlendingDiscard::initTextures()
{
    DepthTest::initTextures();
    m_texture2 = std::make_unique<TextureGL>();
    m_texture2->createByFile(GET_CURRENT("/resources/textures/grass.png"));
}
} // namespace graphicEngine::gl