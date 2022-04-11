//
// Created by william on 2022/4/11.
//

#include "advancedUbo.h"

#include "camera.h"
#include "common/utils.h"

namespace graphicEngine::gl
{
AdvancedUbo::~AdvancedUbo()
{
    if (m_uboMatrices != 0)
        glDeleteBuffers(1, &m_uboMatrices);
}

void AdvancedUbo::initPrograms()
{
    DepthTest::initPrograms();
    m_redProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/advancedUbo.vert"),
                                             GET_CURRENT("/resources/shaders/LearnOpenGL/uboRed.frag"));
    m_blueProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/advancedUbo.vert"),
                                              GET_CURRENT("/resources/shaders/LearnOpenGL/uboBlue.frag"));
    m_greenProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/advancedUbo.vert"),
                                               GET_CURRENT("/resources/shaders/LearnOpenGL/uboGreen.frag"));
    m_yellowProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/advancedUbo.vert"),
                                                GET_CURRENT("/resources/shaders/LearnOpenGL/uboYellow.frag"));
}

void AdvancedUbo::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // cubes
    drawCubes();
    glBindVertexArray(0);
}

void AdvancedUbo::drawCubes()
{
    glBindVertexArray(m_vao);
    // red
    m_redProgram->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
    m_redProgram->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // green
    m_greenProgram->use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
    m_redProgram->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // yellow
    m_yellowProgram->use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
    m_yellowProgram->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // blue
    m_blueProgram->use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
    m_blueProgram->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// configure a uniform buffer object
void AdvancedUbo::initUbo()
{
    // first. We get the relevant block indices
    GLuint uniformBlockIndexRed = glGetUniformBlockIndex(m_redProgram->getProgram(), "Matrices");
    GLuint uniformBlockIndexGreen = glGetUniformBlockIndex(m_greenProgram->getProgram(), "Matrices");
    GLuint uniformBlockIndexBlue = glGetUniformBlockIndex(m_blueProgram->getProgram(), "Matrices");
    GLuint uniformBlockIndexYellow = glGetUniformBlockIndex(m_yellowProgram->getProgram(), "Matrices");
    // then we link each shader's uniform block to this uniform binding point
    glUniformBlockBinding(m_redProgram->getProgram(), uniformBlockIndexRed, 0);
    glUniformBlockBinding(m_greenProgram->getProgram(), uniformBlockIndexGreen, 0);
    glUniformBlockBinding(m_blueProgram->getProgram(), uniformBlockIndexBlue, 0);
    glUniformBlockBinding(m_yellowProgram->getProgram(), uniformBlockIndexYellow, 0);
    // Now actually create the buffer
    glGenBuffers(1, &m_uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(glm::mat4));
    // store the projection matrix (we only do this once now) (note: we're not using zoom any more by changing the FoV)
    auto projection = glm::perspective(45.0f, (float)s_canvasWidth / (float)s_canvasHeight, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AdvancedUbo::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    auto view = m_camera->viewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    /// 第二个参数代表偏移
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AdvancedUbo::initialize()
{
    DepthTest::initialize();
    initUbo();
}
} // namespace graphicEngine::gl