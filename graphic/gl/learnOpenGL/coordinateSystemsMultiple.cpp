//
// Created by cwb on 2022/4/7.
//

#include "coordinateSystemsMultiple.h"

#include "camera.h"
#include "textureGL.h"
namespace graphicEngine::gl
{
CoordinateSystemsMultiple::CoordinateSystemsMultiple()
{
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

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
    m_deltaTime = m_elapseTime - m_lastElapseTime;
    m_lastElapseTime = m_elapseTime;
    m_cameraSpeed = static_cast<float>(2.5 * m_deltaTime);
    m_program->use();
    m_program->setMatrix4("view", m_camera->viewMatrix());
    m_program->setMatrix4("projection", m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f));
    processInput();
}

void CoordinateSystemsMultiple::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void CoordinateSystemsMultiple::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

void CoordinateSystemsMultiple::processInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Forward, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Backward, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Left, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->processKeyboard(CameraMovement::Right, m_deltaTime);
}

void CoordinateSystemsMultiple::touchEvent(double xPos, double yPos)
{
    float xpos = static_cast<float>(xPos);
    float ypos = static_cast<float>(yPos);

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

void CoordinateSystemsMultiple::scrollEvent(double xOffset, double yOffset)
{
    m_camera->processMouseScroll((float)yOffset);
}
} // namespace graphicEngine::gl