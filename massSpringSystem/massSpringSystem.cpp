//
// Created by william on 2022/3/25.
//

#include "application.h"

void massSpringSystem()
{
    Application::Config config;
    Application app(config);
    app.initialize();
    app.draw();
}