//
// Created by cwb on 2022/4/14.
//

#include "msaaTest.h"
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{
MsaaTest::MsaaTest()
{
    m_openMsaa = true;
}

void MsaaTest::render()
{
    CHECK_GL(glClearColor(0.f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    drawCubes(m_program.get());
    CHECK_GL(glBindVertexArray(0));
}

void MsaaTest::drawCubes(const ProgramGL* program)
{
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_cubeTexture->handle()));
    m_modelMatrix = glm::mat4(1.0);
    m_program->setMatrix4("model", m_modelMatrix);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void MsaaTest::initPrograms()
{
    m_program = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/depthTest.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/msaa.frag"));
}

}