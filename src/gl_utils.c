#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

#include "gl_utils.h"

void
setUniform1i(const GLuint program, const char *name, const GLint value)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniform1i(loc, value);
}

void
setUniform1ui(const GLuint program, const char *name, const GLint value)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniform1ui(loc, value);
}

void
setUniform1f(const GLuint program, const char *name, const GLint value)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniform1f(loc, value);
}

void
setUniform2f(
    const GLuint program,
    const char *name,
    const float a,
    const float b)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniform2f(loc, a, b);
}

void
setUniform3f(
    const GLuint program,
    const char *name,
    const float a,
    const float b,
    const float c)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniform3f(loc, a, b, c);
}

void
setUniform4f(
    const GLuint program,
    const char *name,
    const float a,
    const float b,
    const float c,
    const float d)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniform4f(loc, a, b, c, d);
}

void
setUniform4fv(const GLuint program, const char *name, const GLfloat *value)
{
	GLuint loc = glGetUniformLocation(program, name);
	glUniform4fv(loc, 1, value);
}

void
setUniformMatrix4fv(const GLuint program, const char *name,
		    const GLfloat *value)
{
    GLuint loc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

GLuint
gl_texture_2d_create(const char *path, int *width, int *height, int *channel_count)
{
	GLuint id;
	unsigned char *data;
	stbi_set_flip_vertically_on_load(1);

	data = stbi_load(path, width, height, channel_count, 0);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	return id;
}
