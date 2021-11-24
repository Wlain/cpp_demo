//
// Created by william on 2021/11/18.
//
#include "../triangle.h"
#include "animation.h"
#include "background.h"
#include "cube.h"
#include "instancing.h"
#include "point.h"
#include "sharing.h"
#include "showGLInfo.h"

using namespace graphicEngine::gl;

std::string getGLExtensions()
{
    int NumberOfExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
    std::string extensions;
    for (int i = 0; i < NumberOfExtensions; i++)
    {
        const char* e = (const char*)glGetStringi(GL_EXTENSIONS, i);
        extensions += e;
        extensions += '\n';
    }
    std::cout << "Extensions: " << extensions << std::endl;
    return extensions;
}

void glTest()
{
    Animation obj;
    obj.initialize();
    obj.draw();
}