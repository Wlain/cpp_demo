//
// Created by cwb on 2022/4/13.
//

#include "instanceQuads.h"

#include "program.h"
namespace graphicEngine::gl
{
InstanceQuads::~InstanceQuads()
{
    if (m_instanceVbo != 0)
    {
        CHECK_GL(glDeleteBuffers(1, &m_instanceVbo));
    }
}

void InstanceQuads::initialize()
{
    m_program = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/instancing.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/instancing.frag"));
    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    CHECK_GL(glGenBuffers(1, &m_instanceVbo));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_instanceVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations, GL_STATIC_DRAW));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    m_quadVertices = {
        // positions     // colors
        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        0.05f, 0.05f, 0.0f, 1.0f, 1.0f
    };

    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_quadVertices.size(), m_quadVertices.data(), GL_STATIC_DRAW));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(2));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_instanceVbo)); // this attribute comes from a different vertex buffer
    CHECK_GL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    /// 第二个参数：除数，索引：在 slot 索引中指定将在通用属性的更新之间传递的数量。
    CHECK_GL(glVertexAttribDivisor(2, 1)); // tell OpenGL this is an instanced vertex attribute.
    CHECK_GL(glBindVertexArray(0));
}

void InstanceQuads::update(float elapseTime)
{
}

void InstanceQuads::resize(int width, int height)
{
}

void InstanceQuads::render()
{
    CHECK_GL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100)); // 100 triangles of 6 vertices each
    CHECK_GL(glBindVertexArray(0));
}
} // namespace graphicEngine::gl