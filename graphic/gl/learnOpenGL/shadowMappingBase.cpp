//
// Created by cwb on 2022/4/19.
//

#include "shadowMappingBase.h"

#include "common/camera.h"
#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{
ShadowMappingBase::~ShadowMappingBase()
{
    if (m_lightVao != 0)
    {
        CHECK_GL(glDeleteVertexArrays(1, &m_lightVao));
    }
    if (m_lightVbo != 0)
    {
        CHECK_GL(glDeleteBuffers(1, &m_lightVbo));
    }
}

void ShadowMappingBase::initPrograms()
{
    ShadowMappingDepth::initPrograms();
    m_programShadowBase = MAKE_UNIQUE(m_programShadowBase,
                                      GET_CURRENT("/resources/shaders/LearnOpenGL/shadowMapping.vert"),
                                      GET_CURRENT("/resources/shaders/LearnOpenGL/shadowMapping.frag"));
    m_programShadowBase->use();
    m_programShadowBase->setInt("diffuseTexture", 0);
    m_programShadowBase->setInt("shadowMap", 1);
    m_programShadowBase->setVector3("lightColor", m_lightColor);

    m_lightingProgram = MAKE_UNIQUE(m_lightingProgram,
                                    GET_CURRENT("/resources/shaders/LearnOpenGL/singleLight.vert"),
                                    GET_CURRENT("/resources/shaders/LearnOpenGL/singleLight.frag"));
    m_lightingProgram->use();
    m_lightingProgram->setVector3("lightColor", m_lightColor);
}

void ShadowMappingBase::update(float elapseTime)
{
    ShadowMappingDepth::update(elapseTime);
    m_programShadowBase->use();
    // set light uniforms
    m_programShadowBase->setVector3("viewPos", m_camera->m_position);
    m_programShadowBase->setVector3("lightPos", m_lightPos);
    m_programShadowBase->setMatrix4("view", m_viewMatrix);
    m_programShadowBase->setMatrix4("projection", m_projectionMatrix);
    m_programShadowBase->setMatrix4("lightSpaceMatrix", m_lightSpaceMatrix);

    m_lightPos.x = 1.0f + sin(elapseTime) * 2.0f;
    m_lightPos.y = 3.0 + sin(elapseTime / 2.0f) * 1.0f;
    m_lightingProgram->use();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
    m_lightingProgram->setMatrix4("view",m_viewMatrix);
    m_lightingProgram->setMatrix4("projection", m_projectionMatrix);
    m_lightingProgram->setMatrix4("model", model);
    m_lightingProgram->setVector3("lightPos", m_lightPos);
}

void ShadowMappingBase::render()
{
    // 1. render depth of scene to texture (from light's perspective)
    renderDepthImage(m_program);
    // 2. render scene as normal using the generated depth/shadow map
    CHECK_GL(glViewport(0, 0, m_width, m_height));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    renderScene(m_programShadowBase);
    drawLight();
    // render Depth map to quad for visual debugging
    //    renderDebugQuad();
}

void ShadowMappingBase::initTextures()
{
    m_woodTexture = std::make_unique<TextureGL>();
    m_woodTexture->setWarpingType(GL_REPEAT);
    m_woodTexture->createByFile(GET_CURRENT("/resources/textures/wood.png"));
}

void ShadowMappingBase::renderScene(const std::unique_ptr<ProgramGL>& program)
{
    CHECK_GL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    CHECK_GL(glClear(GL_DEPTH_BUFFER_BIT));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_woodTexture->handle());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_depthMapTexture);
    /// floor
    program->use();
    CHECK_GL(glBindVertexArray(m_planeVao));
    program->setMatrix4("model", glm::mat4(1.0f));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    /// cubes
    CHECK_GL(glBindVertexArray(m_vao));
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
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ShadowMappingBase::initVertices()
{
    ShadowMappingDepth::initVertices();
    m_lightCubeVertices = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f
    };
}

void ShadowMappingBase::initPlaneVertexAttrib()
{
    ShadowMappingDepth::initPlaneVertexAttrib();
    CHECK_GL(glGenVertexArrays(1, &m_lightVao));
    CHECK_GL(glGenBuffers(1, &m_lightVbo));
    CHECK_GL(glBindVertexArray(m_lightVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_lightVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_lightCubeVertices[0]) * m_lightCubeVertices.size(), m_lightCubeVertices.data(), GL_STATIC_DRAW));
    // position attribute
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
}

void ShadowMappingBase::drawLight()
{
    m_lightingProgram->use();
    CHECK_GL(glBindVertexArray(m_lightVao));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
}
} // namespace graphicEngine::gl