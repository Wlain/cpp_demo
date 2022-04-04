//
// Created by cwb on 2022/2/7.
//

#include "windowClear.h"
namespace graphicEngine::gl
{
WindowClear::~WindowClear() = default;

void WindowClear::initialize()
{
    Window::initialize();
}
void WindowClear::update(float elapseTime)
{
    Window::update(elapseTime);
}
void WindowClear::resize(int width, int height)
{
    Window::resize(width, height);
}
void WindowClear::render()
{
    Window::render();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
}