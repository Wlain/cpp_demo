#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <inttypes.h>

typedef struct {
	uint32_t id;
	char path[64];
	int width;
	int height;
	int channel_count;
} Texture;

uint32_t texture_create(const char *path, int *width, int *height, int *channel_count);

#endif
