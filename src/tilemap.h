#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <SDL3/SDL.h>
#include <tmx.h>

void tilemap_init(SDL_Renderer* r);
void tilemap_render_map(tmx_map* map);
void* texmgr_load_texture(const char* fname);
Uint32 timer_func(Uint32 interval, void* param);

#endif // !TILE_MAP_H
