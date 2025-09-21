#include "texture.h"
#include "SDL3/SDL_render.h"
#include "tilemap.h"
#include "utils/utils.h"
#include <SDL3/SDL_error.h>
#include <SDL3_image/SDL_image.h>

static SDL_Renderer* renderer;

void texmgr_init(SDL_Renderer* r)
{
    renderer = r;
}

void texmgr_add_texture(TextureManager* mgr, const char* id, const char* fname)
{
    if (mgr->count >= MAX_TEXTURES) return;

    SDL_Texture* tex = texmgr_load_texture(fname);
    if (!tex) {
        util_error("Failed to load texture: %s", SDL_GetError());
        return;
    }

    mgr->textures[mgr->count] = tex;
    mgr->texture_ids[mgr->count] = id;
    mgr->count++;
}

void* texmgr_load_texture(const char* fname)
{
    if (!renderer) {
        util_error("Renderer is NULL :(");
        return NULL;
    }

    SDL_Surface* surface = IMG_Load(fname);
    if (!surface) {
        util_error("Failed to load .bmp: %s", SDL_GetError());
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (!tex) {
        util_error("Failed to create texture: %s", SDL_GetError());
        return NULL;
    }
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

    return tex;
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
