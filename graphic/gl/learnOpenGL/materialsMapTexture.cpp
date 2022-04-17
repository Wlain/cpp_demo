//
// Created by william on 2022/4/7.
//

#include "materialsMapTexture.h"

#include "common/camera.h"
#include "gl/common/textureGL.h"

namespace graphicEngine::gl
{

MaterialsMapTexture::~MaterialsMapTexture() = default;

void MaterialsMapTexture::initialize()
{
    m_verticesCube = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    CHECK_GL(glfwSetWindowTitle(m_window, "MaterialsMapTexture"));
    m_texture = std::make_unique<TextureGL>();
    m_texture1 = std::make_unique<TextureGL>();
    m_texture2 = std::make_unique<TextureGL>();
    m_texture->createByFile(GET_CURRENT("/resources/textures/container2.png"));
    m_texture1->createByFile(GET_CURRENT("/resources/textures/container2_specular.png"));
    m_texture1->createByFile(GET_CURRENT("/resources/textures/matrix.jpg")); // 自发光
    m_lightCubeProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/coordinateSystemsMultiple.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/cube.frag"));
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube[0]) * m_verticesCube.size(), m_verticesCube.data(), GL_STATIC_DRAW));
    CHECK_GL(glBindVertexArray(m_vao));
    // position attribute
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(0));

    m_lightingProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/materialsMapDiffuseSpecular.vert"),
                                                  GET_CURRENT("/resources/shaders/LearnOpenGL/materialsMapDiffuseSpecular.frag"));
    m_lightingProgram->use();
    m_lightingProgram->setVector3("objectColor", 1.0f, 0.5f, 0.3f);
    m_lightingProgram->setVector3("lightColor", 1.0f, 1.0f, 1.0f);
    m_lightingProgram->setFloat("material.shininess", 32.0f);
    m_lightingProgram->setInt("material.diffuse", 0);
    m_lightingProgram->setInt("material.specular", 1);
    m_lightingProgram->setInt("material.emission", 1);
    m_lightingProgram->setVector3("light.ambient", 0.2f, 0.2f, 0.2f);
    m_lightingProgram->setVector3("light.diffuse", 0.5f, 0.5f, 0.5f);
    m_lightingProgram->setVector3("light.specular", 1.0f, 1.0f, 1.0f);
    CHECK_GL(glGenVertexArrays(1, &m_lightVao));
    CHECK_GL(glBindVertexArray(m_lightVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(2));
}
void MaterialsMapTexture::update(float elapseTime)
{
    // light properties
    glm::vec3 lightColor;
    m_lightPos.x = 1.0f + sin(elapseTime) * 2.0f;
    m_lightPos.y = sin(elapseTime / 2.0f) * 1.0f;
    Colors::update(elapseTime);
    m_lightingProgram->use();
    m_lightingProgram->setVector3("viewPos", m_camera->m_position);
    m_lightingProgram->setVector3("light.position", m_lightPos);
}
void MaterialsMapTexture::resize(int width, int height)
{
    Colors::resize(width, height);
}

void MaterialsMapTexture::initLighting()
{
    m_lightingProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/MaterialsMapTexture.vert"),
                                                  GET_CURRENT("/resources/shaders/LearnOpenGL/MaterialsMapTexture.frag"));
    CHECK_GL(glGenVertexArrays(1, &m_lightVao));
    CHECK_GL(glBindVertexArray(m_lightVao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(1));
}

void MaterialsMapTexture::initCube()
{
    m_lightCubeProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/coordinateSystemsMultiple.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/cube.frag"));
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube[0]) * m_verticesCube.size(), m_verticesCube.data(), GL_STATIC_DRAW));
    CHECK_GL(glBindVertexArray(m_vao));
    // position attribute
    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    CHECK_GL(glEnableVertexAttribArray(0));
}

void MaterialsMapTexture::render()
{
    CHECK_GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    CHECK_GL(glEnable(GL_DEPTH_TEST));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    m_lightCubeProgram->use();
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    m_lightingProgram->use();
    // bind diffuse map
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture->handle()));
    // bind specular map
    CHECK_GL(glActiveTexture(GL_TEXTURE1));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture1->handle()));
    CHECK_GL(glActiveTexture(GL_TEXTURE2));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture2->handle()));
    CHECK_GL(glBindVertexArray(m_lightVao));
    CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

} // namespace graphicEngine::gl