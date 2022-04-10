//
// Created by william on 2022/4/9.
//

#include "skybox.h"

#include "camera.h"
#include "textureGL.h"

#include <SOIL2/stb_image.h>
namespace graphicEngine::gl
{

Skybox::~Skybox()
{
    if (m_skyboxVao != 0)
        glDeleteVertexArrays(1, &m_skyboxVao);
    if (m_skyboxVbo != 0)
        glDeleteBuffers(1, &m_skyboxVbo);
    if (m_cubeMapTexture != 0)
        glDeleteTextures(1, &m_cubeMapTexture);
}

void Skybox::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    m_skyboxProgram->use();
    auto view = glm::mat4(glm::mat3(m_camera->viewMatrix())); // remove translation from the view matrix
    m_skyboxProgram->setMatrix4("view", view);
    m_skyboxProgram->setMatrix4("projection", m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f));
}

void Skybox::render()
{
    glClearColor(0.f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // cubes
    drawCubes();
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);
    // change depth function so depth test passes when values are equal to depth buffer's content
    m_skyboxProgram->use();
    glBindVertexArray(m_skyboxVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Skybox::initVertices()
{
    DepthTest::initVertices();
    m_skyboxVertices = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint Skybox::loadCubeMaps(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, channels;
    for (int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            LOG_ERROR("CubeMap texture failed to load at path:%s", faces[i].c_str());
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

void Skybox::initPrograms()
{
    DepthTest::initPrograms();
    m_skyboxProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/skybox.vert"),
                                                GET_CURRENT("/resources/shaders/LearnOpenGL/skybox.frag"));
    m_skyboxProgram->use();
    m_skyboxProgram->setInt("skybox", 0);
}

void Skybox::initVertexAttrib()
{
    DepthTest::initVertexAttrib();
    // skybox VAO
    glGenVertexArrays(1, &m_skyboxVao);
    glGenBuffers(1, &m_skyboxVbo);
    glBindVertexArray(m_skyboxVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices[0]) * m_skyboxVertices.size(), m_skyboxVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void Skybox::initTextures()
{
    m_cubeTexture->createByFile(GET_CURRENT("/resources/textures/container.jpg"));
    m_floorTexture->createByFile(GET_CURRENT("/resources/textures/metal.png"));
    std::vector<std::string> skyboxFace = {
        GET_CURRENT("/resources/textures/skybox/right.jpg"),
        GET_CURRENT("/resources/textures/skybox/left.jpg"),
        GET_CURRENT("/resources/textures/skybox/top.jpg"),
        GET_CURRENT("/resources/textures/skybox/bottom.jpg"),
        GET_CURRENT("/resources/textures/skybox/front.jpg"),
        GET_CURRENT("/resources/textures/skybox/back.jpg")
    };
    m_cubeMapTexture = loadCubeMaps(skyboxFace);
}
} // namespace graphicEngine::gl