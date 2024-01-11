#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture.h"
#include "gl_utils.h"

#include <time.h>

typedef struct {
	Texture *store;
	uint32_t count;
	uint32_t max;
} State;

static State state = {0};

uint32_t
texture_create(const char *path, int *width, int *height, int *channel_count)
{
	int i;
	Texture *texture;

	if (NULL == state.store) {
		state.store = calloc(1, sizeof(Texture));
		state.max = 1;
	}

	/* Check if texture already exists. */
	for (i = 0; i < state.count; ++i) {
		if (0 == strcmp(state.store[i].path, path)) {
			*width = state.store[i].width;
			*height = state.store[i].height;
			*channel_count = state.store[i].channel_count;
			return state.store[i].id;
		}
	}

	/* Need to make space if full. */
	if (state.count == state.max) {
		Texture *new_store = realloc(state.store, 2 * state.max * sizeof(Texture));
		if (NULL == new_store) {
			printf("Failed to reallocate texture store. Exiting.\n");
			exit(EXIT_FAILURE);
		} else {
			state.max *= 2;
			state.store = new_store;
		}
	}

	texture = &state.store[state.count++];
	texture->id = gl_texture_2d_create(path, width, height, channel_count);
	texture->width = *width;
	texture->height = *height;
	texture->channel_count = *channel_count;
	strcpy(texture->path, path);

	return texture->id;
}
