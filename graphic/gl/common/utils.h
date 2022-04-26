//
// Created by cwb on 2022/2/7.
//

#ifndef CPP_DEMO_UTILS_H
#define CPP_DEMO_UTILS_H

#include "base.h"
#include "commonMacro.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace graphicEngine::gl
{
// GL_CHECK
#define GL_CHECK()                                                                                                        \
    do                                                                                                                    \
    {                                                                                                                     \
        GLenum glError = glGetError();                                                                                    \
        if (glError != GL_NO_ERROR)                                                                                       \
        {                                                                                                                 \
            LOG_ERROR("glGetError() = %i (0x%.8x) in filename = %s, line  = %i\n", glError, glError, __FILE__, __LINE__); \
        }                                                                                                                 \
    } while (0)

#define CHECK_GL(x)                                     \
    do                                                  \
    {                                                   \
        (x);                                            \
        checkGlError(TO_STRING(x), __FILE__, __LINE__); \
    } while (0)

#define CHECK_GL_CALLED(x)                         \
    do                                             \
    {                                              \
        CHECK_GL(x);                               \
        LOG_ERROR("func {} called", TO_STRING(x)); \
    } while (0)

static const char* getGLErrorMsg(GLenum err);
extern void checkGlError(const char* glFunc, const char* file, int line);
} // namespace graphicEngine::gl
#endif //CPP_DEMO_UTILS_H
