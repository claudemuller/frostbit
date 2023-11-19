#ifndef GFX_H_
#define GFX_H_

#include "SDL.h"

typedef struct {
	char *window_title;
	int width;
	int height;
	SDL_Window *window;
	SDL_Renderer *renderer;
} graphics_t;

int gfx_init(graphics_t *graphics);
void gfx_free(graphics_t *graphics);

#endif // GFX_H_
