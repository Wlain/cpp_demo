//
// Created by cwb on 2022/4/15.
//

#include "advancedLighting.h"

#include "camera.h"
#include "textureGL.h"
namespace graphicEngine::gl
{
void AdvancedLighting::render()
{
    glClearColor(0.f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // floor
    drawFloor();
    glBindVertexArray(0);
}

void AdvancedLighting::initTextures()
{
    m_floorTexture->createByFile(GET_CURRENT("/resources/textures/wood.png"));
}

void AdvancedLighting::processInput()
{
    DepthTest::processInput();
    if (glfwGetKey(m_window, GLFW_KEY_B) == GLFW_PRESS && !m_blinnKeyPressed)
    {
        m_blinn = !m_blinn;
        m_blinnKeyPressed = true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        m_blinnKeyPressed = false;
    }
}

void AdvancedLighting::initPrograms()
{
    m_program = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/advancedLighting.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/advancedLighting.frag"));
    m_program->use();
    m_program->setInt("inputTexture", 0);
}

void AdvancedLighting::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    m_program->setVector3("viewPos", m_camera->m_position);
    m_program->setVector3("lightPos", m_lightPos);
    m_program->setBool("blinn", m_blinn);
}

void AdvancedLighting::initVertices()
{
    m_planeVertices = {
        // positions            // normals         // texcoords
        10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
        -10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

        10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
        -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
        10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f
    };
}

void AdvancedLighting::initVertexAttrib()
{
    initPlaneVertexAttrib();
}

void AdvancedLighting::initPlaneVertexAttrib()
{
    // plane VAO
    glGenVertexArrays(1, &m_planeVao);
    glGenBuffers(1, &m_planeVbo);
    glBindVertexArray(m_planeVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_planeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_planeVertices[0]) * m_planeVertices.size(), m_planeVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
}
} // namespace graphicEngine::gl