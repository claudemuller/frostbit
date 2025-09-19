#include "texture.h"
#include "SDL3/SDL_render.h"
#include <SDL3_image/SDL_image.h>

void texmgr_add_texture(SDL_Renderer* r, TextureManager* mgr, const char* id, const char* fname)
{
    if (mgr->count >= MAX_TEXTURES) return;

    SDL_Surface* surface = IMG_Load(fname);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load .bmp: %s", SDL_GetError());
        return;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surface);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create texture: %s", SDL_GetError());
        return;
    }
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

    mgr->textures[mgr->count] = tex;
    mgr->texture_ids[mgr->count] = id;
    mgr->count++;
}

SDL_Texture* texmgr_get_texture(TextureManager* mgr, const char* id)
{
    for (size_t i = 0; i < mgr->count; ++i) {
        if (strncmp(mgr->texture_ids[i], id, strlen(id)) == 0) {
            return mgr->textures[i];
        }
    }
    return NULL;
}
