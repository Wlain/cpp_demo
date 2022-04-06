//
// Created by cwb on 2022/2/7.
//

#ifndef CPP_DEMO_UTILS_H
#define CPP_DEMO_UTILS_H

// GL_CHECK
#define GL_CHECK() \
{ \
GLenum glError = glGetError(); \
if (glError != GL_NO_ERROR) {\
printf("glGetError() = %i (0x%.8x) in filename = %s, line  = %i\n", glError, glError, __FILE__ , __LINE__); \
} \
}

#include <string>

std::string getGLExtensions();

#endif //CPP_DEMO_UTILS_H
