//
// Created by william on 2022/4/9.
//

#include "frameBuffers.h"

#include "textureGL.h"
namespace graphicEngine::gl
{

FrameBuffers::~FrameBuffers()
{
    if (m_fbo != 0)
        glDeleteFramebuffers(1, &m_fbo);
    if (m_rbo != 0)
        glDeleteFramebuffers(1, &m_rbo);
    if (m_textureID != 0)
        glDeleteTextures(1, &m_textureID);
    if (m_quadVao != 0)
        glDeleteVertexArrays(1, &m_quadVao);
    if (m_quadVbo != 0)
        glDeleteBuffers(1, &m_quadVbo);
}
void FrameBuffers::initialize()
{
    DepthTest::initialize();
    initFramebuffer();
}

void FrameBuffers::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glEnable(GL_DEPTH_TEST);
    DepthTest::render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    drawFrameBuffersScreen();
}

void FrameBuffers::initFramebuffer()
{
    // framebuffer configuration
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s_canvasWidth, s_canvasHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    // 创建并初始化renderbuffer对象的数据存储,use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, s_canvasWidth, s_canvasHeight);
    // now actually attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffers::initTextures()
{
    m_cubeTexture->createByFile(GET_CURRENT("/resources/textures/container.jpg"));
    m_floorTexture->createByFile(GET_CURRENT("/resources/textures/metal.png"));
}

void FrameBuffers::initPrograms()
{
    DepthTest::initPrograms();
    m_quadProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/textures.vert"),
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
    glGenVertexArrays(1, &m_quadVao);
    glGenBuffers(1, &m_quadVbo);
    glBindVertexArray(m_quadVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices[0]) * m_quadVertices.size(), m_quadVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void FrameBuffers::drawFrameBuffersScreen()
{
    m_quadProgram->use();
    glBindVertexArray(m_quadVao);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
} // namespace graphicEngine::gl