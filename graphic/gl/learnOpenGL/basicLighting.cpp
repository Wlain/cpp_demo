//
// Created by william on 2022/4/7.
//

#include "basicLighting.h"

#include "camera.h"
namespace graphicEngine::gl
{

BasicLighting::~BasicLighting() = default;

void BasicLighting::initialize()
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
}
void BasicLighting::update(float elapseTime)
{
    m_lightPos.x = 1.0f + sin(elapseTime) * 2.0f;
    m_lightPos.y = sin(elapseTime / 2.0f) * 1.0f;
    Colors::update(elapseTime);
    m_lightingProgram->use();
    m_lightingProgram->setVector3("viewPos", m_camera->m_position);
    m_lightingProgram->setVector3("lightPos", m_lightPos);
}
void BasicLighting::resize(int width, int height)
{
    Colors::resize(width, height);
}

void BasicLighting::initLighting()
{
    /// 根据不同的shader切换不同的光照
    /// basicLighting:在世界空间计算phongs光照
    /// basicLighting:在观察空间中计算phongs光照
    /// basicLighting:在观察空间中计算gruraud光照
    m_lightingProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/basicLighting.vert"),
                                                  GET_CURRENT("/resources/shaders/LearnOpenGL/basicLighting.frag"));
    m_lightingProgram->use();
    m_lightingProgram->setVector3("objectColor", 1.0f, 0.5f, 0.3f);
    m_lightingProgram->setVector3("lightColor", 1.0f, 1.0f, 1.0f);
    glGenVertexArrays(1, &m_lightVao);
    glBindVertexArray(m_lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void BasicLighting::initCube()
{
    m_lightCubeProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/coordinateSystemsMultiple.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/cube.frag"));
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube[0]) * m_verticesCube.size(), m_verticesCube.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_vao);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void BasicLighting::render()
{
    Colors::render();
}

} // namespace graphicEngine::gl