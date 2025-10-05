#include "texture.h"
#include "tilemap.h"
#include "utils/utils.h"
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

static SDL_Renderer* renderer;
static TextureManager* mgr;
static MemoryArena* game_mem;

void texmgr_init(MemoryArena* gmem, SDL_Renderer* r, TextureManager* tm)
{
    renderer = r;
    mgr = tm;
    game_mem = gmem;
}

void texmgr_add_texture(const char* fname)
{
    SDL_Texture* tex = texmgr_load_texture(fname);
    if (!tex) {
        util_error("Failed to load texture: %s", SDL_GetError());
        return;
    }
}

void* texmgr_load_texture(const char* fname)
{
    if (!renderer) {
        util_error("Renderer is NULL :(");
        return NULL;
    }

    if (mgr->count >= MAX_TEXTURES) return NULL;

    // If the texture already exists, return that
    for (size_t i = 0; i < mgr->count; ++i) {
        if (strncmp(mgr->texture_ids[i], fname, strlen(fname)) == 0) {
            return mgr->textures[i];
        }
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
    SDL_DestroySurface(surface);

    // SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

    size_t fnamelen = strlen(fname);
    char* texid = (char*)arena_alloc_aligned(game_mem, fnamelen, 16);
    strncpy(texid, fname, fnamelen);

    mgr->textures[mgr->count] = tex;
    mgr->texture_ids[mgr->count] = texid;
    mgr->count++;

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

void texmgr_destroy(void)
{
    for (size_t i = 0; i < mgr->count; ++i) {
        SDL_DestroyTexture(mgr->textures[i]);
    }
}
