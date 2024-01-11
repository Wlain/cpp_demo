#include <assert.h>

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "gl_utils.h"
#include "math_linear.h"

#include "render.h"
#include "shader.h"

#define MAX_QUAD_COUNT 100000
#define MAX_VERTEX_COUNT MAX_QUAD_COUNT * 4
#define MAX_INDEX_COUNT MAX_QUAD_COUNT * 6
#define MAX_TEXTURE_COUNT 32

typedef struct {
	vec3 position;
	vec4 colour;
	vec2 tex_coords;
	int tex_index;
} Vertex;

typedef struct {
	GLFWwindow *window;
	uint32_t quad_shader;
	uint32_t quad_vao;
	uint32_t quad_vbo;
	uint32_t quad_ebo;
	uint32_t quad_count;
	uint32_t white_texture;
	uint32_t white_texture_slot;
	uint32_t index_count;
	Vertex *quad_buffer;
	Vertex *quad_buffer_ptr;
	uint32_t texture_slots[MAX_TEXTURE_COUNT];
	uint32_t texture_slot_index;
	uint32_t text_shader;
	uint32_t text_vao;
	uint32_t text_vbo;
} State;

static State state = {0};

void
render_init(GLFWwindow *window, uint32_t render_width, uint32_t render_height)
{
	int i;
	uint32_t *indices = calloc(MAX_INDEX_COUNT, sizeof(uint32_t));
	uint32_t offset = 0;
	uint32_t white = 0xffffffff;
	uint32_t loc;
	mat4x4 projection;
	mat4x4 transform;
	mat4x4 view = {0};

	state.window = window;

	state.texture_slot_index = 1;
	state.quad_buffer = malloc(MAX_VERTEX_COUNT * sizeof(Vertex));

	glGenVertexArrays(1, &state.quad_vao);
	glBindVertexArray(state.quad_vao);

	glGenBuffers(1, &state.quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, state.quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, colour)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, tex_coords)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, tex_index)));
	glEnableVertexAttribArray(3);

	for (i = 0; i < MAX_INDEX_COUNT; i += 6, offset += 4) {
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 3 + offset;
		indices[i + 3] = 1 + offset;
		indices[i + 4] = 2 + offset;
		indices[i + 5] = 3 + offset;
	}

	glGenBuffers(1, &state.quad_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.quad_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MAX_INDEX_COUNT, indices, GL_STATIC_DRAW);

	glGenTextures(1, &state.white_texture);
	glBindTexture(GL_TEXTURE_2D, state.white_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);

	state.texture_slots[0] = state.white_texture;

	/* create quad program */
	state.quad_shader = create_shader_program("assets/shaders/quad.vert", "assets/shaders/quad.frag", NULL);
	glUseProgram(state.quad_shader);

	loc = glGetUniformLocation(state.quad_shader, "u_textures");

	for (i = 0; i < 32; ++i) {
		char str[16];
		sprintf(str, "u_sampler%d", i);
		loc = glGetUniformLocation(state.quad_shader, str);
		glUniform1i(loc, GL_TEXTURE0 + i);
	}

	mat4x4_ortho(projection, 0, render_width, 0, render_height, -100, 100);
	setUniformMatrix4fv(state.quad_shader, "u_projection", &projection[0][0]);

	mat4x4_identity(transform);
	setUniformMatrix4fv(state.quad_shader, "u_transform", &transform[0][0]);

	mat4x4_identity(view);
	setUniformMatrix4fv(state.quad_shader, "u_view", &view[0][0]);

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	free(indices);
}

void
render_exit()
{
	free(state.quad_buffer);
}

void
render_begin_batch()
{
	state.quad_buffer_ptr = state.quad_buffer;
}

void
render_end_batch()
{
	GLsizeiptr size = (uint8_t*)state.quad_buffer_ptr - (uint8_t*)state.quad_buffer;
	glBindBuffer(GL_ARRAY_BUFFER, state.quad_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, state.quad_buffer);
}

void
render_flush()
{
	uint32_t i;

	for (i = 0; i < state.texture_slot_index; ++i)
		glBindTexture(GL_TEXTURE_2D, state.texture_slots[i]);

	glUseProgram(state.quad_shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(state.quad_vao);
	glDrawElements(GL_TRIANGLES, state.index_count, GL_UNSIGNED_INT, NULL);

	state.index_count = 0;
	state.texture_slot_index = 1;
	state.quad_count = 0;
}

void
render_quad_subtexture(
	const uint32_t texture_id,
	const float texture_width,
	const float texture_height,
	const float sprite_width,
	const float sprite_height,
	const float sprite_x,
	const float sprite_y,
	const float x,
	const float y)
{

	int i, texture_index = 0;
	vec4 colour = { 1, 1, 1, 1 };
	vec2 min, max;
	min[0] = (sprite_x * sprite_width) / texture_width;
	min[1] = (sprite_y * sprite_height) / texture_height;
	max[0] = ((sprite_x + 1) * sprite_width) / texture_width;
	max[1] = ((sprite_y + 1) * sprite_height) / texture_height;

	if (state.index_count >= MAX_INDEX_COUNT || state.texture_slot_index > 31) {
		render_end_batch();
		render_flush();
		render_begin_batch();
	}

	for (i = 1; i < state.texture_slot_index; ++i)
		if (state.texture_slots[i] == texture_id) {
			texture_index = i;
			break;
		}

	if (0 == texture_index) {
		texture_index = state.texture_slot_index;
		state.texture_slots[state.texture_slot_index] = texture_id;
		state.texture_slot_index++;
	}

	state.quad_buffer_ptr->position[0] = x;
	state.quad_buffer_ptr->position[1] = y;
	state.quad_buffer_ptr->position[2] = 0;
	memcpy(state.quad_buffer_ptr->colour, colour, sizeof(vec4));
	state.quad_buffer_ptr->tex_coords[0] = min[0];
	state.quad_buffer_ptr->tex_coords[1] = min[1];
	state.quad_buffer_ptr->tex_index = texture_index;
	state.quad_buffer_ptr++;

	state.quad_buffer_ptr->position[0] = x + sprite_width;
	state.quad_buffer_ptr->position[1] = y;
	state.quad_buffer_ptr->position[2] = 0;
	memcpy(state.quad_buffer_ptr->colour, colour, sizeof(vec4));
	state.quad_buffer_ptr->tex_coords[0] = max[0];
	state.quad_buffer_ptr->tex_coords[1] = min[1];
	state.quad_buffer_ptr->tex_index = texture_index;
	state.quad_buffer_ptr++;

	state.quad_buffer_ptr->position[0] = x + sprite_width;
	state.quad_buffer_ptr->position[1] = y + sprite_height;
	state.quad_buffer_ptr->position[2] = 0;
	memcpy(state.quad_buffer_ptr->colour, colour, sizeof(vec4));
	state.quad_buffer_ptr->tex_coords[0] = max[0];
	state.quad_buffer_ptr->tex_coords[1] = max[1];
	state.quad_buffer_ptr->tex_index = texture_index;
	state.quad_buffer_ptr++;

	state.quad_buffer_ptr->position[0] = x;
	state.quad_buffer_ptr->position[1] = y + sprite_height;
	state.quad_buffer_ptr->position[2] = 0;
	memcpy(state.quad_buffer_ptr->colour, colour, sizeof(vec4));
	state.quad_buffer_ptr->tex_coords[0] = min[0];
	state.quad_buffer_ptr->tex_coords[1] = max[1];
	state.quad_buffer_ptr->tex_index = texture_index;
	state.quad_buffer_ptr++;

	state.index_count += 6;
	state.quad_count++;
}
