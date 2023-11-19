#ifndef BOXCOLLIDER_COMPONENT_H_
#define BOXCOLLIDER_COMPONENT_H_

#include "../vector.h"
#include "SDL.h"

typedef struct {
	vec2_t offset;
	int width;
	int height;
	SDL_Color colour;
} component_boxcollider_t;

#endif // BOXCOLLIDER_COMPONENT_H_
