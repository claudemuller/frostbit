#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "state.h"
#include <SDL3/SDL.h>
#include <tmx.h>

void tilemap_init(SDL_Renderer* r);
void tilemap_load_level(GameState* state);
void tilemap_render_map(GameState* state, tmx_map* map);
void* texmgr_load_texture(const char* fname);

#endif // !TILE_MAP_H
