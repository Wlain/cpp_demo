//
// Created by william on 2022/4/6.
//

#include "textureExercise2.h"
#include "textureGL.h"
namespace graphicEngine::gl
{
TextureExercise2::~TextureExercise2() = default;
void TextureExercise2::initialize()
{
    m_vertices = {
        // positions         // texture coords
        0.5f,  0.5f, 0.0f,   2.0f, 2.0f, // top right
        0.5f, -0.5f, 0.0f,   2.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 2.0f  // top left
    };
    m_texture1->setWarpingType(GL_NEAREST);
    TexturesCombined::initialize();
}
void TextureExercise2::update(float elapseTime)
{
}
void TextureExercise2::resize(int width, int height)
{
}
void TextureExercise2::render()
{
    TexturesCombined::render();
}
}