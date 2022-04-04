//
// Created by cwb on 2022/2/7.
//

#include "window.h"
namespace graphicEngine::gl
{
Window::~Window() = default;

void Window::initialize()
{
    m_title = "Window";
}

void Window::update(float elapseTime)
{
}

void Window::resize(int width, int height)
{
}

void Window::render()
{
}
}
