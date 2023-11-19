#ifndef SPRITE_COMPONENT_H_
#define SPRITE_COMPONENT_H_

#include "SDL.h"
#include <stdbool.h>

typedef struct {
	SDL_Rect src_rect;
	int width;
	int height;
	int z_index;
	SDL_RendererFlip is_flipped;
	bool is_fixed;
	const char *id;
} component_sprite_t;

#endif // SPRITE_COMPONENT_H_
