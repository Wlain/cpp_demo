//
// Created by cwb on 2022/2/7.
//

#include "window.h"
namespace graphicEngine::gl
{
Window::~Window() = default;

void Window::initialize()
{
    initWithProperty("Window");
}
void Window::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}
void Window::resize(int width, int height)
{
    CommonInterface::resize(width, height);
}
void Window::display()
{
    CommonInterface::display();
}
}
