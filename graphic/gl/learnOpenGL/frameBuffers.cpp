//
// Created by william on 2022/4/9.
//

#include "frameBuffers.h"
#include "gl/common/utils.h"
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{

FrameBuffers::~FrameBuffers()
{
    if (m_fbo != 0)
        CHECK_GL(glDeleteFramebuffers(1, &m_fbo));
    if (m_rbo != 0)
        CHECK_GL(glDeleteFramebuffers(1, &m_rbo));
    if (m_textureID != 0)
        CHECK_GL(glDeleteTextures(1, &m_textureID));
    if (m_quadVao != 0)
        CHECK_GL(glDeleteVertexArrays(1, &m_quadVao));
    if (m_quadVbo != 0)
        CHECK_GL(glDeleteBuffers(1, &m_quadVbo));
}
void FrameBuffers::initialize()
{
    DepthTest::initialize();
    initFramebuffer();
}

void FrameBuffers::render()
{
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    CHECK_GL(glEnable(GL_DEPTH_TEST));
    DepthTest::render();
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    CHECK_GL(glDisable(GL_DEPTH_TEST));
    CHECK_GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f)); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    drawFrameBuffersScreen();
}

void FrameBuffers::initFramebuffer()
{
    // framebuffer configuration
    CHECK_GL(glGenFramebuffers(1, &m_fbo));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    CHECK_GL(glGenTextures(1, &m_textureID));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_textureID));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s_canvasWidth, s_canvasHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0));
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    CHECK_GL(glGenRenderbuffers(1, &m_rbo));
    CHECK_GL(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
    // 创建并初始化renderbuffer对象的数据存储,use a single renderbuffer object for both a depth AND stencil buffer.
    CHECK_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, s_canvasWidth, s_canvasHeight));
    // now actually attach it
    CHECK_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo));
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffers::initTextures()
{
    m_cubeTexture->createByFile(GET_CURRENT("/resources/textures/container.jpg"));
    m_floorTexture->createByFile(GET_CURRENT("/resources/textures/metal.png"));
}

void FrameBuffers::initPrograms()
{
    DepthTest::initPrograms();
    m_quadProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/textures.vert"),
                                                GET_CURRENT("/resources/shaders/LearnOpenGL/textures.frag"));
    m_quadProgram->use();
    m_quadProgram->setInt("inputTexture", 0);
}

void FrameBuffers::initVertices()
{
    DepthTest::initVertices();
    m_quadVertices = {
        // positions   // texCoords
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };
}

void FrameBuffers::initVertexAttrib()
{
    DepthTest::initVertexAttrib();
    CHECK_GL(glGenVertexArrays(1, &m_quadVao));
    CHECK_GL(glGenBuffers(1, &m_quadVbo));
    CHECK_GL(glBindVertexArray(m_quadVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices[0]) * m_quadVertices.size(), m_quadVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
}

void FrameBuffers::drawFrameBuffersScreen()
{
    m_quadProgram->use();
    CHECK_GL(glBindVertexArray(m_quadVao));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_textureID));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
}
} // namespace graphicEngine::gl