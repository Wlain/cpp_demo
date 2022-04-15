//
// Created by cwb on 2022/4/15.
//

#include "gammaCorrection.h"

#include "camera.h"
namespace graphicEngine::gl
{

void GammaCorrection::initPrograms()
{
    m_program = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/advancedLightingGammaCorrect.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/advancedLightingGammaCorrect.frag"));
    m_program->use();
    m_program->setInt("inputTexture", 0);
}

void GammaCorrection::initVertices()
{
    AdvancedLighting::initVertices();
    m_lightPositions = {
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f)
    };
    m_lightColors = {
        glm::vec3(0.25),
        glm::vec3(0.50),
        glm::vec3(0.75),
        glm::vec3(1.00)
    };
}

void GammaCorrection::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    m_program->setVector3("viewPos", m_camera->m_position);
    m_program->setVector3Arrays("lightPos", m_lightPositions);
    m_program->setVector3Arrays("lightColors", m_lightColors);
    m_program->setBool("gamma", m_gammaEnabled);
}

void GammaCorrection::processInput()
{
    AdvancedLighting::processInput();
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS && !m_gammaKeyPressed)
    {
        m_gammaEnabled = !m_gammaEnabled;
        m_gammaKeyPressed = true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        m_gammaKeyPressed = false;
    }
}
} // namespace graphicEngine::gl