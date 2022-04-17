//
// Created by cwb on 2022/4/8.
//

#include "depthTest.h"

#include "base.h"
#include "common/camera.h"
#include "common/utils.h"
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{
DepthTest::DepthTest()
{
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    m_cubeTexture = std::make_unique<TextureGL>();
    m_floorTexture = std::make_unique<TextureGL>();
    m_cubeTexture->setWarpingType(GL_REPEAT);
    m_floorTexture->setWarpingType(GL_REPEAT);
}

DepthTest::~DepthTest()
{
    if (m_planeVao != 0)
        CHECK_GL(glDeleteVertexArrays(1, &m_planeVao));
    if (m_planeVbo != 0)
        CHECK_GL(glDeleteBuffers(1, &m_planeVbo));
}

void DepthTest::initialize()
{
    initVertices();
    initModel();
    initPrograms();
    initVertexAttrib();
    initTextures();
    initGLStatus();
}

void DepthTest::update(float elapseTime)
{
    float currentTime = elapseTime;
    m_deltaTime = currentTime - m_lastTime;
    m_lastTime = currentTime;
    m_elapseTime = elapseTime;
    m_viewMatrix = m_camera->viewMatrix();
    m_program->use();
    m_program->setMatrix4("view", m_viewMatrix);
    m_projectionMatrix = m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f);
    m_program->setMatrix4("projection", m_projectionMatrix);
    processInput();
}

void DepthTest::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void DepthTest::render()
{
    CHECK_GL(glClearColor(0.f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    // cubes
    drawCubes();
    // floor
    drawFloor();
    CHECK_GL(glBindVertexArray(0));
}

void DepthTest::touchEvent(double xPos, double yPos)
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

void DepthTest::scrollEvent(double xOffset, double yOffset)
{
    m_camera->processMouseScroll((float)yOffset);
}

void DepthTest::processInput()
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

void DepthTest::initTextures()
{
    m_cubeTexture->createByFile(GET_CURRENT("/resources/textures/marble.jpg"));
    m_floorTexture->createByFile(GET_CURRENT("/resources/textures/metal.png"));
}

void DepthTest::initPrograms()
{
    m_program = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/depthTest.vert"),
                                            GET_CURRENT("/resources/shaders/LearnOpenGL/depthTest.frag"));
    m_program->use();
    m_program->setInt("inputTexture", 0);
}

void DepthTest::initVertices()
{
    m_planeVertices = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f
    };
    m_cubeVertices = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };
}
void DepthTest::initVertexAttrib()
{
    initCubesVertexAttrib();
    initPlaneVertexAttrib();
}

void DepthTest::initGLStatus()
{
    CHECK_GL(glEnable(GL_DEPTH_TEST));
    CHECK_GL(glDepthFunc(GL_LESS)); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
}

void DepthTest::drawCubes()
{
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_cubeTexture->handle()));
    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_program->use();
    m_program->setMatrix4("model", m_modelMatrix);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    m_modelMatrix = glm::mat4(1.0f);
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
    m_program->setMatrix4("model", m_modelMatrix);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void DepthTest::drawFloor()
{
    m_program->use();
    CHECK_GL(glBindVertexArray(m_planeVao));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_floorTexture->handle()));
    m_program->setMatrix4("model", glm::mat4(1.0f));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void DepthTest::initModel()
{
}

void DepthTest::initPlaneVertexAttrib()
{
    // plane VAO
    CHECK_GL(glGenVertexArrays(1, &m_planeVao));
    CHECK_GL(glGenBuffers(1, &m_planeVbo));
    CHECK_GL(glBindVertexArray(m_planeVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_planeVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_planeVertices[0]) * m_planeVertices.size(), m_planeVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glBindVertexArray(0));
}

void DepthTest::initCubesVertexAttrib()
{
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeVertices[0]) * m_cubeVertices.size(), m_cubeVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glBindVertexArray(0));
}

} // namespace graphicEngine::gl