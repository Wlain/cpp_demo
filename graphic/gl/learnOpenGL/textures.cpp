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
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), &m_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Textures::update(float elapseTime)
{
}

void Textures::resize(int width, int height)
{
}

void Textures::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindTexture(GL_TEXTURE_2D, m_texture->handle());
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
} // namespace graphicEngine::gl