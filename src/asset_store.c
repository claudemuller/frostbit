#include "array.h"
#include "asset_store.h"
#include <SDL2/SDL_image.h>

asset_store_t asset_store = { 0 };

void asset_store_init(void)
{
}

void asset_store_add_texture(SDL_Renderer *renderer, const char *asset_id, const char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    texture_t texture = {
        .id = asset_id,
        .texture = tex
    };
    array_push(asset_store.textures, texture);
}

SDL_Texture *asset_store_get_texture(const char *asset_id)
{
    size_t len = array_length(asset_store.textures);
    for (size_t i = 0; i < len; i++) {
        // TODO: should we rather use strncmp here?
        if (strcmp(asset_store.textures[i].id, asset_id) == 0) {
            return asset_store.textures[i].texture;
        }
    }

    return NULL;
}

void asset_store_free(void)
{
    array_free(asset_store.textures);
}
