#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "arena.h"
#include <SDL3/SDL.h>
#include <stdint.h>

#define MAX_TEXTURES 15

typedef struct TextureManager {
    size_t count;
    const char* texture_ids[MAX_TEXTURES];
    SDL_Texture* textures[MAX_TEXTURES];
} TextureManager;

void texmgr_init(MemoryArena* game_mem, SDL_Renderer* r, TextureManager* tm);
void texmgr_add_texture(const char* fname);
void* texmgr_load_texture(const char* fname);
SDL_Texture* texmgr_get_texture(TextureManager* mgr, const char* id);
void texmgr_destroy(void);

#endif // !TEXTURE_H_
