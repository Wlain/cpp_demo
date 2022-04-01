//
// Created by william on 2022/3/25.
//

#include "massSpringSystem.h"

using namespace graphicEngine::gl;

void massSpringSystemTest()
{
    massSpringSystem::Config config;
    massSpringSystem test(config);
    test.initialize();
    test.draw();
}