/*
 * Every Sprite in the game must use an Array Texture. This simplifies
 * animations somewhat as the animations can be defined as an array of ints.
 * 
 * Because Sprites must use Array Textures, there must be a way to register
 * each sprite sheet and also individual frames (as static sprites).
 * 
 * E.G. All the tiles for any area are loaded as one texture. Lets just say
 * there is sand (0), grass (1), dirt (2).
 * 
 * All one needs to know is which texture to use and which index to render.
 * 
 * Text is handled differently to Sprites. Text uses an old-school Sprite Atlas
 * because the glyphs are different sizes.
 */
#ifndef RENDER_INCLUDED
#define RENDER_INCLUDED
#include <stdio.h>
#include <inttypes.h>
#define GLFW_DLL
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "math_linear.h"

typedef enum {
	UO_AUTO,
	UO_VISIBLE,
	UO_HIDDEN,
	UO_SCROLL,
	UO_INHERIT,
	UO_COUNT
} UiOverflowType;

typedef struct {
	GLuint texture_id;
	vec2 min;
	vec2 max;
} SubTexture2D;

void render_init(GLFWwindow *window, uint32_t render_width, uint32_t render_height);
void render_begin_batch();
void render_end_batch();
void render_flush();
void render_quad_subtexture(const uint32_t texture_id, const float texture_width, const float texture_height, const float sprite_width, const float sprite_height, const float sprite_x, const float sprite_y, const float x, const float y);
#endif
