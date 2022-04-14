//
// Created by cwb on 2022/4/14.
//

#include "msaaTest.h"
#include "textureGL.h"
namespace graphicEngine::gl
{
MsaaTest::MsaaTest()
{
    m_openMsaa = true;
}

void MsaaTest::render()
{
    glClearColor(0.f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCubes();
    glBindVertexArray(0);
}

void MsaaTest::drawCubes()
{
    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_cubeTexture->handle());
    m_modelMatrix = glm::mat4(1.0);
    m_program->setMatrix4("model", m_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MsaaTest::initPrograms()
{
    m_program = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/depthTest.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/msaa.frag"));
}

}