//
// Created by william on 2022/4/9.
//

#include "frameBuffersExercise1.h"
namespace graphicEngine::gl
{

FrameBuffersExercise1::~FrameBuffersExercise1() = default;

void FrameBuffersExercise1::initialize()
{
    FrameBuffers::initialize();
}

void FrameBuffersExercise1::update(float elapseTime)
{
    FrameBuffers::update(elapseTime);
}

void FrameBuffersExercise1::resize(int width, int height)
{
    FrameBuffers::resize(width, height);
}

void FrameBuffersExercise1::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glEnable(GL_DEPTH_TEST);
    DepthTest::render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCubes();
    drawFloor();
    glDisable(GL_DEPTH_TEST);
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