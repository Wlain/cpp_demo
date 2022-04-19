//
// Created by cwb on 2022/4/13.
//

#include "shadowMappingDepth.h"

#include "common/textureGL.h"

#include <glm/gtc/matrix_transform.hpp>
namespace graphicEngine::gl
{
ShadowMappingDepth::~ShadowMappingDepth()
{
    if (m_depthMapFbo != 0)
    {
        CHECK_GL(glDeleteFramebuffers(1, &m_depthMapFbo));
    }
    if (m_depthMapTexture != 0)
    {
        CHECK_GL(glDeleteTextures(1, &m_depthMapTexture));
    }
    if (m_quadTextureID != 0)
    {
        CHECK_GL(glDeleteTextures(1, &m_quadTextureID));
    }
    if (m_quadVao != 0)
    {
        CHECK_GL(glDeleteVertexArrays(1, &m_quadVao));
    }
    if (m_quadVbo != 0)
    {
        CHECK_GL(glDeleteBuffers(1, &m_quadVbo));
    }
}

void ShadowMappingDepth::initPrograms()
{
    m_program = MAKE_UNIQUE(m_program,
                            GET_CURRENT("/resources/shaders/LearnOpenGL/shadowMappingDepth.vert"),
                            GET_CURRENT("/resources/shaders/LearnOpenGL/shadowMappingDepth.frag"));
    m_quadProgram = MAKE_UNIQUE(m_quadProgram,
                                GET_CURRENT("/resources/shaders/LearnOpenGL/debugQuadDepth.vert"),
                                GET_CURRENT("/resources/shaders/LearnOpenGL/debugQuadDepth.frag"));
    m_quadProgram->use();
    m_quadProgram->setInt("depthMap", 0);
}

void ShadowMappingDepth::initVertices()
{
    DepthTest::initVertices();
    m_planeVertices = {
        // positions            // normals         // texcoords
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f
    };
    m_cubeVertices = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                            // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    };
    m_quadVertices = {
        // positions        // texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };
}

void ShadowMappingDepth::initPlaneVertexAttrib()
{
    // plane VAO
    CHECK_GL(glGenVertexArrays(1, &m_planeVao));
    CHECK_GL(glGenBuffers(1, &m_planeVbo));
    CHECK_GL(glBindVertexArray(m_planeVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_planeVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_planeVertices[0]) * m_planeVertices.size(), m_planeVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(2));
    CHECK_GL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
}

void ShadowMappingDepth::initFbo()
{
    CHECK_GL(glGenFramebuffers(1, &m_depthMapFbo));
    CHECK_GL(glGenTextures(1, &m_depthMapTexture));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_depthMapTexture));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, s_shadowWidth, s_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo));
    CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMapTexture, 0));
    CHECK_GL(glDrawBuffer(GL_NONE));
    CHECK_GL(glReadBuffer(GL_NONE));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    CHECK_GL(glGenVertexArrays(1, &m_quadVao));
    CHECK_GL(glGenBuffers(1, &m_quadVbo));
    CHECK_GL(glBindVertexArray(m_quadVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices[0]) * m_quadVertices.size(), m_quadVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glBindVertexArray(0));
}

void ShadowMappingDepth::initialize()
{
    DepthTest::initialize();
    initFbo();
}

void ShadowMappingDepth::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    m_lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_nearPlane, m_farPlane);
    m_lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    m_lightSpaceMatrix = m_lightProjection * m_lightView;
    m_program->use();
    m_program->setMatrix4("lightSpaceMatrix", m_lightSpaceMatrix);
}

void ShadowMappingDepth::render()
{
    renderDepthImage(m_program);
    // reset viewport
    CHECK_GL(glViewport(0, 0, m_width, m_height));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    // render Depth map to quad for visual debugging
    renderDebugQuad();
}

void ShadowMappingDepth::drawCubes(const std::unique_ptr<ProgramGL>& program)
{
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_cubeTexture->handle()));
    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 1.5f, 0.0));
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.5f));
    program->use();
    program->setMatrix4("model", m_modelMatrix);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(2.0f, 0.0f, 1.0));
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.5f));
    program->setMatrix4("model", m_modelMatrix);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(-1.0f, 0.0f, 2.0));
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.25f));
    program->setMatrix4("model", m_modelMatrix);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void ShadowMappingDepth::renderDebugQuad()
{
    m_quadProgram->use();
    m_quadProgram->setFloat("nearPlane", m_nearPlane);
    m_quadProgram->setFloat("farPlane", m_farPlane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_depthMapTexture);
    CHECK_GL(glBindVertexArray(m_quadVao));
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_depthMapTexture));
    CHECK_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
}

void ShadowMappingDepth::renderDepthImage(const std::unique_ptr<ProgramGL>& program)
{
    CHECK_GL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    CHECK_GL(glViewport(0, 0, s_shadowHeight, s_shadowHeight));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo));
    CHECK_GL(glClear(GL_DEPTH_BUFFER_BIT));
    drawFloor(program);
    drawCubes(program);
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ShadowMappingDepth::initCubesVertexAttrib()
{
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeVertices[0]) * m_cubeVertices.size(), m_cubeVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(2));
    CHECK_GL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
    CHECK_GL(glBindVertexArray(0));

}

} // namespace graphicEngine::gl
