//
// Created by cwb on 2022/4/14.
//

#include "msaaTestOffscreen.h"
#include "commonMacro.h"
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{
MsaaOffscreen::~MsaaOffscreen()
{
    if (m_quadFbo != 0)
    {
        CHECK_GL(glDeleteFramebuffers(1, &m_quadFbo));
    }
    if (m_multiFbo != 0)
    {
        CHECK_GL(glDeleteFramebuffers(1, &m_multiFbo));
    }
    if (m_multiRbo != 0)
    {
        CHECK_GL(glDeleteRenderbuffers(1, &m_multiRbo));
    }
    if (m_quadTextureID != 0)
    {
        CHECK_GL(glDeleteTextures(1, &m_quadTextureID));
    }
    if (m_multiTextureID != 0)
    {
        CHECK_GL(glDeleteTextures(1, &m_multiTextureID));
    }
    if (m_quadVao != 0)
    {
        CHECK_GL(glDeleteVertexArrays(1, &m_quadVao));
    }
    if (m_quadVao != 0)
    {
        CHECK_GL(glDeleteBuffers(1, &m_quadVbo));
    }
}

void MsaaOffscreen::initialize()
{
    DepthTest::initialize();
    initFbo();
}

void MsaaOffscreen::initFbo()
{
    //  configure MSAA framebuffer
    CHECK_GL(glGenFramebuffers(1, &m_multiFbo));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_multiFbo));
    // create a multisampled color attachment texture
    CHECK_GL(glGenTextures(1, &m_multiTextureID));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_multiTextureID));
    CHECK_GL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, s_canvasWidth, s_canvasHeight, GL_TRUE));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_multiTextureID, 0));
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    CHECK_GL(glGenRenderbuffers(1, &m_multiRbo));
    CHECK_GL(glBindRenderbuffer(GL_RENDERBUFFER, m_multiRbo));
    CHECK_GL(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, s_canvasWidth, s_canvasHeight));
    CHECK_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    CHECK_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_multiRbo));
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // configure second post-processing framebuffer
    CHECK_GL(glGenFramebuffers(1, &m_quadFbo));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_quadFbo));
    CHECK_GL(glGenTextures(1, &m_quadTextureID));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_quadTextureID));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s_canvasWidth, s_canvasHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_quadTextureID, 0));
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
    }
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void MsaaOffscreen::render()
{
    // 1. draw scene as normal in multisampled buffers
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_multiFbo));
    CHECK_GL(glEnable(GL_DEPTH_TEST));
    MsaaTest::render();
    // 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
    CHECK_GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multiFbo));
    CHECK_GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_quadFbo));
    CHECK_GL(glBlitFramebuffer(0, 0, s_canvasWidth, s_canvasHeight, 0, 0, s_canvasWidth, s_canvasHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR));
    // 3. now render quad with scene's visuals as its texture image
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    CHECK_GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    CHECK_GL(glDisable(GL_DEPTH_TEST));
    // 4.draw Screen quad
    m_quadProgram->use();
    CHECK_GL(glBindVertexArray(m_quadVao));
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_quadTextureID));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void MsaaOffscreen::initVertices()
{
    DepthTest::initVertices();
    m_quadVertices = {
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
}

void MsaaOffscreen::initVertexAttrib()
{
    DepthTest::initVertexAttrib();
    CHECK_GL(glGenVertexArrays(1, &m_quadVao));
    CHECK_GL(glGenBuffers(1, &m_quadVbo));
    CHECK_GL(glBindVertexArray(m_quadVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices[0]) * m_quadVertices.size(), m_quadVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    CHECK_GL(glBindVertexArray(0));
}

void MsaaOffscreen::initPrograms()
{
    MsaaTest::initPrograms();
    m_quadProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/quad.vert"),
                                              GET_CURRENT("/resources/shaders/LearnOpenGL/quad.frag"));
    m_quadProgram->use();
    m_quadProgram->setInt("inputTexture", 0);
}

void MsaaOffscreen::initTextures()
{
    DepthTest::initTextures();
}
} // namespace graphicEngine::gl