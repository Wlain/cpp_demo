//
// Created by william on 2022/3/25.
//

#include "application.h"

#include <utility>
Application::~Application() = default;

Application::Application(Config config) :
    m_config(std::move(config))
{
}

void Application::initialize()
{
    initWithProperty(std::make_tuple("OpenGL Application", GET_CURRENT("/resources/shaders/triangle.gl.vert"), GET_CURRENT("/resources/shaders/triangle.gl.frag")));
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glPointSize(8);
    glLineWidth(4);
    glColor3f(1.0, 1.0, 1.0);
    // Create two ropes
    int numNodes = 32;
}

void Application::update(float elapseTime)
{
    CommonInterface::update(elapseTime);
}

void Application::resize(int width, int height)
{
    CommonInterface::resize(width, height);
}

void Application::display()
{
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Application::draw()
{
    CommonInterface::draw();
}
