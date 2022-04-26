//
// Created by cwb on 2022/4/20.
//

#include "pointShadowMapping.h"

#include <glm/gtc/matrix_transform.hpp>

namespace graphicEngine::gl
{
void PointShadowMapping::initDepthFbo()
{
    CHECK_GL(glGenFramebuffers(1, &m_depthMapFbo));
    CHECK_GL(glGenTextures(1, &m_depthMapTexture));
    CHECK_GL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthMapTexture));
    for (int i = 0; i < 6; ++i)
    {
        CHECK_GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, s_shadowWidth, s_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
    }
    CHECK_GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
    CHECK_GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    CHECK_GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo));
    CHECK_GL(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthMapTexture, 0));
    CHECK_GL(glDrawBuffer(GL_NONE));
    CHECK_GL(glReadBuffer(GL_NONE));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void PointShadowMapping::initPrograms()
{
    ShadowMappingBase::initPrograms();
    m_program = MAKE_UNIQUE(m_program,
                            GET_CURRENT("/resources/shaders/LearnOpenGL/pointShadowDepth.vert"),
                            GET_CURRENT("/resources/shaders/LearnOpenGL/pointShadowDepth.frag"),
                            GET_CURRENT("/resources/shaders/LearnOpenGL/pointShadowDepth.gs.glsl"));

    m_programShadowBase = MAKE_UNIQUE(m_programShadowBase,
                                      GET_CURRENT("/resources/shaders/LearnOpenGL/pointShadows.vert"),
                                      GET_CURRENT("/resources/shaders/LearnOpenGL/pointShadows.frag"));
    m_programShadowBase->use();
    m_programShadowBase->setInt("diffuseTexture", 0);
    m_programShadowBase->setInt("shadowMap", 1);
    m_programShadowBase->setVector3("lightColor", m_lightColor);
}

void PointShadowMapping::render()
{
    ShadowMappingBase::render();
}

void PointShadowMapping::update(float elapseTime)
{
    ShadowMappingBase::update(elapseTime);
    // 0. create depth cubemap transformation matrices
    float nearPlane = 1.0f;
    float farPlane = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)s_shadowWidth / (float)s_shadowHeight, nearPlane, farPlane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_program->use();
    for (int i = 0; i <6; ++i)
    {
        m_program->setMatrix4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    m_program->setFloat("farPlane", farPlane);
    m_program->setVector3("lightPos", m_lightPos);
    m_programShadowBase->use();
    // set light uniforms
    m_programShadowBase->setFloat("shadows", m_shadows);
    m_programShadowBase->setFloat("farPlane", farPlane);
}

void PointShadowMapping::renderDepthImage(const ProgramGL* program)
{
    CHECK_GL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    CHECK_GL(glViewport(0, 0, s_shadowHeight, s_shadowHeight));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFbo));
    CHECK_GL(glClear(GL_DEPTH_BUFFER_BIT));
    drawCubes(program);
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void PointShadowMapping::drawCubes(const ProgramGL* program)
{
    CHECK_GL(glBindVertexArray(m_vao));
    program->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(5.0f));
    program->setMatrix4("model", model);
    glDisable(GL_CULL_FACE);
    m_program->setInt("reverseNormals", 1);
    m_program->setInt("reverseNormals", 0); // and of course disable it
    program->setMatrix4("model", m_modelMatrix);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    glEnable(GL_CULL_FACE);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    program->setMatrix4("model", model);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    program->setMatrix4("model", model);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    program->setMatrix4("model", model);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    program->setMatrix4("model", model);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    program->setMatrix4("model", model);
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void PointShadowMapping::processInput()
{
    DepthTest::processInput();
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS && !m_shadowsKeyPressed)
    {
        m_shadows = !m_shadows;
        m_shadowsKeyPressed = true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        m_shadowsKeyPressed = false;
    }
}
} // namespace graphicEngine::gl