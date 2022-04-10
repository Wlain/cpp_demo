//
// Created by cwb on 2022/4/8.
//

#include "stencilTest.h"

#include "base.h"
#include "camera.h"
#include "textureGL.h"
namespace graphicEngine::gl
{

StencilTest::~StencilTest()
{
}

void StencilTest::initialize()
{
    DepthTest::initialize();
}

void StencilTest::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    // set uniforms
    m_singleColorProgram->use();
    m_singleColorProgram->setMatrix4("view", m_camera->viewMatrix());
    m_singleColorProgram->setMatrix4("projection", m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f));
}
void StencilTest::resize(int width, int height)
{
    DepthTest::resize(width, height);
}
void StencilTest::render()
{
    glClearColor(0.f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
    glStencilMask(0x00); // 每一位在写入模板缓冲时都会变成0（禁用写入）
    // floor
    drawFloor();

    // 1st. render pass, draw objects as normal, writing to the stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
    // cubes
    drawCubes();

    // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
    // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
    // the objects' size differences, making it look like borders.
    // -----------------------------------------------------------------------------------------------------------------------------
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    m_singleColorProgram->use();
    float scale = 1.1f;
    // cubes
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, m_cubeTexture->handle());
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    m_singleColorProgram->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    m_singleColorProgram->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void StencilTest::initPrograms()
{
    DepthTest::initPrograms();
    m_singleColorProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/depthTest.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/stencilTestSingleColor.frag"));
}

void StencilTest::initGLStatus()
{
    DepthTest::initGLStatus();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
} // namespace graphicEngine::gl