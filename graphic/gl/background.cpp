//
// Created by william on 2021/6/23.
//

#include "background.h"

namespace graphicEngine::gl
{
Background::~Background() = default;

void Background::initialize()
{
    m_title = "Background";
}

void Background::update(float elapseTime)
{
}

void Background::resize(int width, int height)
{
}

void Background::render()
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
} // namespace graphicEngine::gl