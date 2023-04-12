#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

typedef struct {
	char *window_title;
	int width;
	int height;
	SDL_Window *window;
	SDL_Renderer *renderer;
} graphics_t;

int graphics_init(graphics_t *graphics);
void graphics_free(graphics_t *graphics);

#endif // GRAPHICS_H
