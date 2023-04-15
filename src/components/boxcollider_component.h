#ifndef BOXCOLLIDER_COMPONENT_H
#define BOXCOLLIDER_COMPONENT_H

#include <SDL2/SDL.h>
#include "../vector.h"

typedef struct {
	vec2_t offset;
	int width;
	int height;
	SDL_Color colour;
} component_boxcollider_t;

#endif // BOXCOLLIDER_COMPONENT_H

