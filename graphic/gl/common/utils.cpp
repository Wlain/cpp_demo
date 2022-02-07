//
// Created by cwb on 2022/2/7.
//

#include "utils.h"
#include "base.h"

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