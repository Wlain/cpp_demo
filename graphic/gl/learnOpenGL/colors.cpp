//
// Created by cwb on 2022/4/7.
//

#include "colors.h"

#include "common/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace graphicEngine::gl
{
Colors::Colors()
{
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

Colors::~Colors()
{
    if (m_lightVao != 0)
        glDeleteVertexArrays(1, &m_lightVao);
}

void Colors::initialize()
{
    m_title = "Colors";
    glfwSetWindowTitle(m_window, m_title.c_str());
    initCube();
    initLighting();
}

void Colors::update(float elapseTime)
{
    float currentTime = elapseTime;
    m_deltaTime = currentTime - m_lastTime;
    m_lastTime = currentTime;
    auto model = glm::mat4(1.0f);
    m_lightingProgram->use();
    m_lightingProgram->setMatrix4("view", m_camera->viewMatrix());
    m_lightingProgram->setMatrix4("projection", m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f));
    m_lightingProgram->setMatrix4("model", model);

    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    m_lightCubeProgram->use();
    m_lightCubeProgram->setMatrix4("view", m_camera->viewMatrix());
    m_lightCubeProgram->setMatrix4("projection", m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f));
    m_lightCubeProgram->setMatrix4("model", model);
    processInput();
}

void Colors::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void Colors::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_lightCubeProgram->use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_lightingProgram->use();
    glBindVertexArray(m_lightVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Colors::processInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Forward, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Backward, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Left, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Right, m_deltaTime);
}

void Colors::touchEvent(double xPos, double yPos)
{
    auto xpos = static_cast<float>(xPos);
    auto ypos = static_cast<float>(yPos);

    if (m_firstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float xOffset = xpos - m_lastX;
    float yOffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top
    m_lastX = xpos;
    m_lastY = ypos;
    m_camera->processMouseMovement(xOffset, yOffset);
}

void Colors::scrollEvent(double xOffset, double yOffset)
{
    m_camera->processMouseScroll((float)yOffset);
}

void Colors::initLighting()
{
    m_lightingProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/coordinateSystemsMultiple.vert"),
                                                  GET_CURRENT("/resources/shaders/LearnOpenGL/colors.frag"));
    m_lightingProgram->use();
    m_lightingProgram->setVector3("objectColor", 1.0f, 0.5f, 0.3f);
    m_lightingProgram->setVector3("lightColor", 1.0f, 1.0f, 1.0f);
    glGenVertexArrays(1, &m_lightVao);
    glBindVertexArray(m_lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Colors::initCube()
{
    m_lightCubeProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/coordinateSystemsMultiple.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/cube.frag"));
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube[0]) * m_verticesCube.size(), m_verticesCube.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_vao);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

} // namespace graphicEngine::gl