//
// Created by william on 2022/4/6.
//

#include "texturesCombined.h"
#include "gl/common/textureGL.h"

namespace graphicEngine::gl
{
TexturesCombined::TexturesCombined()
{
    m_texture = std::make_unique<TextureGL>();
    m_texture1 = std::make_unique<TextureGL>();
    m_title = "TexturesCombined";
    m_vertPath = GET_CURRENT("/resources/shaders/LearnOpenGL/textures.vert");
    m_fragPath = GET_CURRENT("/resources/shaders/LearnOpenGL/texturesCombined.frag");
}

TexturesCombined::~TexturesCombined() = default;

void TexturesCombined::initialize()
{
    m_texture->createByFile(GET_CURRENT("/resources/textures/container.jpg"));
    m_texture1->createByFile(GET_CURRENT("/resources/textures/awesomeface.png"));
    initWithProperty();
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
    m_program->use();
    m_program->setInt("inputTexture", 0);
    m_program->setInt("inputTexture1", 1);
    m_program->setFloat("mixValue", 0.2f);
}

void TexturesCombined::update(float elapseTime)
{
    m_elapseTime = elapseTime;
}

void TexturesCombined::resize(int width, int height)
{
}

void TexturesCombined::render()
{
    CHECK_GL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    m_program->use();
    CHECK_GL(glActiveTexture(GL_TEXTURE0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture->handle()));
    CHECK_GL(glActiveTexture(GL_TEXTURE1));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_texture1->handle()));
    CHECK_GL(glBindVertexArray(m_vao));
    CHECK_GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
} // namespace graphicEngine::gl