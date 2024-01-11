#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "src/ecs.h"
#include "texture.h"
#include "render.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "lib/nuklear.h"
#include "lib/nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define RENDER_WIDTH 640
#define RENDER_HEIGHT 360
#define SCALE 2
#define WINDOW_WIDTH RENDER_WIDTH * SCALE
#define WINDOW_HEIGHT RENDER_HEIGHT * SCALE

void glfw_framebuffer_size_callback();

/* == ECS Component Definitions ===================================== */

typedef struct {
	float x;
	float y;
} PositionComponent;

typedef struct {
	float x;
	float y;
} VelocityComponent;

typedef struct {
	uint32_t texture_id;
	uint32_t texture_width;
	uint32_t texture_height;
	uint32_t sprite_width;
	uint32_t sprite_height;
} SpriteComponent;

/* ================================================================== */

/* == ECS System Functions ========================================== */
void
system_move()
{
	uint32_t i;
	QueryResult *qr = ecs_query(2, 0, 2);
	render_begin_batch();
	for (i = 0; i < qr->count; ++i) {
		PositionComponent *pos = (PositionComponent*)ecs_get(qr->list[i], 0);
		VelocityComponent *vel = (VelocityComponent*)ecs_get(qr->list[i], 2);
		pos->x += vel->x;
		pos->y += vel->y;
		if (pos->x > RENDER_WIDTH || pos->x < 0)
			vel->x *= -1;
		if (pos->y > RENDER_HEIGHT || pos->y < 0)
			vel->y *= -1;
	}
	render_end_batch();
	render_flush();
}

void
system_draw()
{
	uint32_t i;
	QueryResult *qr = ecs_query(2, 0, 1);
	render_begin_batch();
	for (i = 0; i < qr->count; ++i) {
		PositionComponent *pos = (PositionComponent*)ecs_get(qr->list[i], 0);
		SpriteComponent *spr = (SpriteComponent*)ecs_get(qr->list[i], 1);
		render_quad_subtexture(spr->texture_id, spr->texture_width, spr->texture_height, spr->sprite_width, spr->sprite_height, 0, 0, pos->x, pos->y);
	}
	render_end_batch();
	render_flush();
}
/* ================================================================== */

int
main(void)
{
	struct nk_glfw glfw = {0};
	struct nk_context *ctx = NULL;
	struct nk_font_atlas *atlas;

	/* GLFW Setup */
	GLFWwindow *window;
	GLFWmonitor *monitor;
	const GLFWvidmode *mode;
	uint32_t screen_width, screen_height;
	double time_now, time_delta, time_last_frame = 0, fps;

	/* Init GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(SCALE * RENDER_WIDTH, SCALE * RENDER_HEIGHT, "Ember ECS Test", NULL, NULL);

	assert(window);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

	assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

	screen_width  = mode->width;
	screen_height = mode->height;

	glfwSetWindowPos(window, screen_width / 2 - WINDOW_WIDTH / 2, screen_height / 2 - WINDOW_HEIGHT / 2);

	glClearColor(0.2, 0.1, 0.2, 1);

	render_init(window, RENDER_WIDTH, RENDER_HEIGHT);

	/* Nuklear Setup */
	ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
	nk_glfw3_font_stash_begin(&glfw, &atlas);
	nk_glfw3_font_stash_end(&glfw);

/* == ECS init ====================================================== */
	ecs_init(3, sizeof(PositionComponent), sizeof(SpriteComponent), sizeof(VelocityComponent));
/* ================================================================== */

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		nk_glfw3_new_frame(&glfw);

		time_now = glfwGetTime();
		time_delta = time_now - time_last_frame;
		fps = (1000.0 / time_delta) / 1000.0;
		glClear(GL_COLOR_BUFFER_BIT);

/* == ECS update systems ============================================ */
		system_move();
		system_draw();
/* ================================================================== */

		/* Nuklear UI handling */
		if (nk_begin(ctx, "Manage Entities", nk_rect(10, 10, 300, WINDOW_HEIGHT - 20),
			NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
			NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
			static int32_t position = 1, sprite = 1, velocity = 1;
			static int32_t amount;

			nk_layout_row_dynamic(ctx, 30, 3);
			position = nk_check_label(ctx, "Position", position); 
			sprite = nk_check_label(ctx, "Sprite", sprite);
			velocity = nk_check_label(ctx, "Velocity", velocity);

			nk_layout_row_dynamic(ctx, 30, 2);
			nk_label(ctx, "FPS:", NK_TEXT_LEFT);
			{
				char s[5] = {0};
				sprintf(s, "%.0f", fps);
				nk_label(ctx, s, NK_TEXT_RIGHT);
			}

			nk_layout_row_dynamic(ctx, 30, 2);
			nk_label(ctx, "Entity Count:", NK_TEXT_LEFT);
			{
				char s[10] = {0};
				uint32_t count = ecs_query(0)->count;
				sprintf(s, "%u", count);
				nk_label(ctx, s, NK_TEXT_RIGHT);
			}

			nk_layout_row_dynamic(ctx, 25, 1);
			nk_property_int(ctx, "Amount:", -20000, &amount, 20000, 1000, 1);

			nk_layout_row_dynamic(ctx, 30,  2);
			if (nk_button_label(ctx, "Create")) {
				uint32_t i;
				for (i = 0; i < amount; ++i) {
					Entity e = ecs_create();
					PositionComponent pos = {0};
					VelocityComponent vel = {0};
					SpriteComponent spr = {0};
					int32_t w, h, c;
					spr.texture_id = texture_create("assets/sprites/pig.png", &w, &h, &c);
					spr.texture_width = w;
					spr.texture_height = h;
					spr.sprite_width = 32;
					spr.sprite_height = 32;
					pos.x = rand() % RENDER_WIDTH;
					pos.y = rand() % RENDER_HEIGHT;
					vel.x = rand() % 6 - 3;
					vel.y = rand() % 6 - 3;
					if (position)
						ecs_add(e.id, 0, &pos);
					if (sprite)
						ecs_add(e.id, 1, &spr);
					if (velocity)
						ecs_add(e.id, 2, &vel);
				}
			}
			if (nk_button_label(ctx, "Destroy")) {
				uint32_t i;
				QueryResult *qr = ecs_query(0);
				for (i = 0; i < amount; ++i) {
					ecs_kill(qr->list[i]);
				}
			}
		}
		nk_end(ctx);

	        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

		glfwSwapBuffers(window);
		time_last_frame = time_now;
	}

	puts("Exiting...\n");

	return 0;
}

void
glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
		glViewport(0, 0, width, height);
}
