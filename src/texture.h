#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL3/SDL.h>
#include <stdint.h>

#define MAX_TEXTURES 15

typedef struct TextureManager {
    size_t count;
    const char* texture_ids[MAX_TEXTURES];
    SDL_Texture* textures[MAX_TEXTURES];
} TextureManager;

void texmgr_init(SDL_Renderer* r);
void texmgr_add_texture(TextureManager* mgr, const char* id, const char* fname);
SDL_Texture* texmgr_get_texture(TextureManager* mgr, const char* id);

#endif // !TEXTURE_H_
