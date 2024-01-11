#ifndef GL_UTILS_H_INCLUDED
#define GL_UTILS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

void setUniform1i(const GLuint program, const char *name, const GLint value);
void setUniform1ui(const GLuint program, const char *name, const GLint value);
void setUniform1f(const GLuint program, const char *name, const GLint value);
void setUniform2f( const GLuint program, const char *name, const float a, const float b);
void setUniform3f( const GLuint program, const char *name, const float a, const float b, const float c);
void setUniform4f( const GLuint program, const char *name, const float a, const float b, const float c, const float d);
void setUniform4fv(const GLuint program, const char *name, const GLfloat *value);
void setUniformMatrix4fv(const GLuint program, const char *name, const GLfloat *value);
GLuint gl_texture_2d_create(const char *path, int *width, int *height, int *channel_count);

#endif
