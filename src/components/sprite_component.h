#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
	char *id;
	int width;
	int height;
	int z_index;
	bool is_fixed;
	SDL_Rect srcRect;
} component_sprite_t;

#endif // SPRITE_COMPONENT_H
