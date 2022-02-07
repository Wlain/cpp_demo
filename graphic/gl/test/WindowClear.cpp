//
// Created by cwb on 2022/2/7.
//

#include "WindowClear.h"
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
void WindowClear::display()
{
    Window::display();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
}