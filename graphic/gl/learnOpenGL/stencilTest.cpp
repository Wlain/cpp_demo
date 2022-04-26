//
// Created by cwb on 2022/4/8.
//

#include "stencilTest.h"

#include "base.h"
#include "common/camera.h"
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{

StencilTest::~StencilTest() = default;

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
    CHECK_GL(glClearColor(0.f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
    CHECK_GL(glStencilMask(0x00)); // 每一位在写入模板缓冲时都会变成0（禁用写入）
    // floor
    drawFloor(m_program.get());

    // 1st. render pass, draw objects as normal, writing to the stencil buffer
    CHECK_GL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
    CHECK_GL(glStencilMask(0xFF)); // 每一位写入模板缓冲时都保持原样
    // cubes
    drawCubes(m_program.get());

    // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
    // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
    // the objects' size differences, making it look like borders.
    // -----------------------------------------------------------------------------------------------------------------------------
    CHECK_GL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    CHECK_GL(glStencilMask(0x00));
    CHECK_GL(glDisable(GL_DEPTH_TEST));
    m_singleColorProgram->use();
    float scale = 1.1f;
    // cubes
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_cubeTexture->handle()));
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    m_singleColorProgram->setMatrix4("model", model);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    m_singleColorProgram->setMatrix4("model", model);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    CHECK_GL(glBindVertexArray(0));
    CHECK_GL(glStencilMask(0xFF)); // 每一位写入模板缓冲时都保持原样
    CHECK_GL(glStencilFunc(GL_ALWAYS, 0, 0xFF));
    CHECK_GL(glEnable(GL_DEPTH_TEST));
}

void StencilTest::initPrograms()
{
    DepthTest::initPrograms();
    m_singleColorProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/depthTest.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/stencilTestSingleColor.frag"));
}

void StencilTest::initGLStatus()
{
    DepthTest::initGLStatus();
    CHECK_GL(glEnable(GL_DEPTH_TEST));
    CHECK_GL(glDepthFunc(GL_LESS));
    CHECK_GL(glEnable(GL_STENCIL_TEST));
    CHECK_GL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    CHECK_GL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
}
} // namespace graphicEngine::gl