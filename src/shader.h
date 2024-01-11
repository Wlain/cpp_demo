#ifndef _Shader
#define _Shader
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "file.h"

GLuint
create_shader_program(const char *vert_path, const char *frag_path, const char *geom_path)
{
	GLuint program;
	GLuint vert_shader;
	GLuint frag_shader;
	GLuint geom_shader;
	char *vert_shader_source;
	char *frag_shader_source;
	char *geom_shader_source;
	int success;
	char info_log[512];
	char ls[45] = "Creating Vertex Shader ";

	read_file_into_buffer(&vert_shader_source, vert_path);

//	strcat(ls, vert_path);
//	strcat(ls, "...");
//	printf("%-45s", ls);

	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, (const char * const *)&vert_shader_source, NULL);
	free(vert_shader_source);
	glCompileShader(vert_shader);

	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
		printf("\033[0;31m%5s\033[0m\n", "FAIL!");
		printf("[RENDERING] Error compiling vertex shader\n%s\n", info_log);
		return -1;
	}

	printf("\033[0;32m%5s\033[0m\n", "OK!");

	read_file_into_buffer(&frag_shader_source, frag_path);

//	strcpy(ls, "Creating Fragment Shader ");
//	strcat(ls, frag_path);
//	strcat(ls, "...");
//	printf("%-45s", ls);

	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, (const char * const *)&frag_shader_source, NULL);
	free(frag_shader_source);
	glCompileShader(frag_shader);

	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
		printf("\033[0;31m%5s\033[0m\n", "FAIL!");
		printf("[RENDERING] Error compiling fragment shader\n%s\n", info_log);
		return -1;
	}

	printf("\033[0;32m%5s\033[0m\n", "OK!");

	if (geom_path != NULL)
	{
		read_file_into_buffer(&geom_shader_source, geom_path);

		strcpy(ls, "Creating Geometry Shader ");
		strcat(ls, geom_path);
		strcat(ls, "...");
		printf("%-45s", ls);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, (const char * const *)&geom_shader_source, NULL);
		free(geom_shader_source);
		glCompileShader(geom_shader);

		glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
		    glGetShaderInfoLog(geom_shader, 512, NULL, info_log);
			printf("\033[0;31m%5s\033[0m\n", "FAIL!");
		    printf("[RENDERING] Error compiling geometry shader\n%s\n", info_log);
		    return -1;
		}

		printf("\033[0;32m%5s\033[0m\n", "OK!");
	}

	strcpy(ls, "Creating Shader Program...");
	printf("%-45s", ls);

	program = glCreateProgram();

	printf("\033[0;32m%5s\033[0m\n", "OK!");

	sprintf(ls, "Linking Shader Program %d...", program);
	printf("%-45s", ls);

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	if (geom_path != NULL) 
		glAttachShader(program, geom_shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, info_log);
		printf("\033[0;31m%5s\033[0m\n", "FAIL!");
		printf("[RENDERING] Error linking shader\n%s\n", info_log);
		return -1;
	}

	printf("\033[0;32m%5s\033[0m\n", "OK!");

	return program;
}
#endif
