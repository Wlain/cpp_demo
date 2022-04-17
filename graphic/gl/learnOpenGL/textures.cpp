//
// Created by cwb on 2022/4/6.
//

#include "textures.h"

namespace graphicEngine::gl
{

Textures::Textures()
{
    m_texture = std::make_unique<TextureGL>();
}

Textures::~Textures() = default;

void Textures::initialize()
{
    m_texture->createByFile(GET_CURRENT("/resources/test.jpg"));
    initWithProperty(std::make_tuple("Textures", GET_CURRENT("/resources/shaders/LearnOpenGL/textures.vert"), GET_CURRENT("/resources/shaders/LearnOpenGL/textures.frag")));
    CHECK_GL(glGenVertexArrays(1, &m_vao));
    CHECK_GL(glGenBuffers(1, &m_vbo));
    CHECK_GL(glGenBuffers(1, &m_ebo));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW));
    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    CHECK_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), &m_indices, GL_STATIC_DRAW));

    CHECK_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr));
    CHECK_GL(glEnableVertexAttribArray(0));
    CHECK_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    CHECK_GL(glEnableVertexAttribArray(1));
    CHECK_GL(glBindVertexArray(0));
}

void Textures::update(float elapseTime)
{
}

void Textures::resize(int width, int height)
{
}

void Textures::render()
{
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture->handle()));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
} // namespace graphicEngine::gl