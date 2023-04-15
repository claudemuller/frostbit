#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
	SDL_Rect src_rect;
	int width;
	int height;
	int z_index;
	SDL_RendererFlip is_flipped;
	bool is_fixed;
	char *id;
} component_sprite_t;

#endif // SPRITE_COMPONENT_H
