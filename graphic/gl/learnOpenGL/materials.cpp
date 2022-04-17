//
// Created by william on 2022/4/7.
//

#include "materials.h"

#include "common/camera.h"
namespace graphicEngine::gl
{

Materials::~Materials() = default;

void Materials::initialize()
{
    m_verticesCube = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };
    Colors::initialize();
    CHECK_GL(glfwSetWindowTitle(m_window, "Materials"));
    m_lightingProgram->use();
    // material properties
    m_lightingProgram->setVector3("material.ambient", 1.0f, 0.5f, 0.31f);
    m_lightingProgram->setVector3("material.diffuse", 1.0f, 0.5f, 0.31f);
    m_lightingProgram->setVector3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    m_lightingProgram->setFloat("material.shininess", 32.0f);
}
void Materials::update(float elapseTime)
{
    // light properties
    glm::vec3 lightColor;
    lightColor.x = static_cast<float>(sin(elapseTime * 2.0));
    lightColor.y = static_cast<float>(sin(elapseTime * 0.7));
    lightColor.z = static_cast<float>(sin(elapseTime * 1.3));
    auto diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
    auto ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

    m_lightPos.x = 1.0f + sin(elapseTime) * 2.0f;
    m_lightPos.y = sin(elapseTime / 2.0f) * 1.0f;
    Colors::update(elapseTime);
    m_lightingProgram->use();
    m_lightingProgram->setVector3("viewPos", m_camera->m_position);
    m_lightingProgram->setVector3("light.position", m_lightPos);
    m_lightingProgram->setVector3("light.ambient", ambientColor);
    m_lightingProgram->setVector3("light.diffuse", diffuseColor);
    m_lightingProgram->setVector3("light.specular", 1.0f, 1.0f, 1.0f);
}
void Materials::resize(int width, int height)
{
    Colors::resize(width, height);
}

void Materials::initLighting()
{
    m_lightingProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/materials.vert"),
                                                  GET_CURRENT("/resources/shaders/LearnOpenGL/materials.frag"));
    CHECK_GL(glGenVertexArrays(1, &m_lightVao));
    CHECK_GL(glBindVertexArray(m_lightVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(1));
}

void Materials::initCube()
{
    m_lightCubeProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/coordinateSystemsMultiple.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/cube.frag"));
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube[0]) * m_verticesCube.size(), m_verticesCube.data(), GL_STATIC_DRAW));
    CHECK_GL(glBindVertexArray(m_vao));
    // position attribute
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(0));
}

void Materials::render()
{
    Colors::render();
}

} // namespace graphicEngine::gl