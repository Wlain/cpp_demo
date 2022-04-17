//
// Created by cwb on 2022/2/7.
//

#include "utils.h"

#include "base.h"
namespace graphicEngine::gl
{
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

const char* getGLErrorMsg(GLenum err)
{
    const char* msg;
    switch (err)
    {
    case GL_INVALID_ENUM:
        return "invalid enum";
    case GL_INVALID_VALUE:
        return "invalid value";
    case GL_INVALID_OPERATION:
        return "invalid operations";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "invalid framebuffer operation";
    case GL_OUT_OF_MEMORY:
        return "out of memory";
    default:
        static char tmp[256];
        sprintf(tmp, "%d\n", err);
        return tmp;
    }
}

void checkGlError(const char* glFunc)
{
    auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        auto msg = getGLErrorMsg(err);
        LOG_ERROR("`{} generated error!`: msg:{}", glFunc, msg);
    }
}
} // namespace graphicEngine::gl