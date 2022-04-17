//
// Created by william on 2022/4/6.
//

#include "textureExercise3.h"

#include "gl/common/textureGL.h"
namespace graphicEngine::gl
{
TextureExercise3::~TextureExercise3() = default;
void TextureExercise3::initialize()
{
    m_vertices = {
        // positions         // texture coords
        0.5f, 0.5f, 0.0f, 0.55f, 0.55f,   // top right
        0.5f, -0.5f, 0.0f, 0.55f, 0.45f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.45f, 0.45f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.45f, 0.55f   // top left
    };
    m_texture1->setWarpingType(GL_NEAREST);
    m_texture->setFilteringType(GL_NEAREST);
    m_texture1->setFilteringType(GL_NEAREST);
    m_title = "TextureExercise3";
    TexturesCombined::initialize();
}
void TextureExercise3::update(float elapseTime)
{
}
void TextureExercise3::resize(int width, int height)
{
}
void TextureExercise3::render()
{
    TexturesCombined::render();
}
} // namespace graphicEngine::gl