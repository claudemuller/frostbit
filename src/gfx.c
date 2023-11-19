#include "gfx.h"
#include <stdbool.h>

int gfx_init(graphics_t *graphics)
{
    if (!graphics->window_title) {
        graphics->window_title = "";
    }

    if (!graphics->width) {
        graphics->width = 800;
    }

    if (!graphics->height) {
        graphics->height = 600;
    }

    graphics->window = SDL_CreateWindow(
        graphics->window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        graphics->width,
        graphics->height,
        SDL_WINDOW_SHOWN
    );
    if (!graphics->window) {
        SDL_Log("Unabled to create window: %s", SDL_GetError());
        return false;
    }

    graphics->renderer = SDL_CreateRenderer(
        graphics->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!graphics->renderer) {
        SDL_Log("Unabled to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void gfx_free(graphics_t *graphics)
{
    SDL_DestroyWindow(graphics->window);
    SDL_DestroyRenderer(graphics->renderer);
}
