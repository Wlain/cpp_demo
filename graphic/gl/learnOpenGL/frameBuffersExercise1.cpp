//
// Created by william on 2022/4/9.
//

#include "frameBuffersExercise1.h"
namespace graphicEngine::gl
{

FrameBuffersExercise1::~FrameBuffersExercise1() = default;

void FrameBuffersExercise1::render()
{
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    CHECK_GL(glEnable(GL_DEPTH_TEST));
    DepthTest::render();
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    CHECK_GL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    drawCubes();
    drawFloor();
    CHECK_GL(glDisable(GL_DEPTH_TEST));
    drawFrameBuffersScreen();
}

void FrameBuffersExercise1::initVertices()
{
    DepthTest::initVertices();
    m_quadVertices = {
        // positions   // texCoords
        -0.3f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.7f, 0.0f, 0.0f, 0.0f,
        0.3f, 0.7f, 0.0f, 1.0f, 0.0f,

        -0.3f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.3f, 0.7f, 0.0f, 1.0f, 0.0f,
        0.3f, 1.0f, 0.0f, 1.0f, 1.0f
    };
}
} // namespace graphicEngine::gl