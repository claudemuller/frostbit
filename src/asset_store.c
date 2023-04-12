#include <SDL2/SDL_image.h>
#include "asset_store.h"

void
asset_store_init(asset_store_t *asset_store)
{
	asset_store->textures = hashmap_create();
}

void
asset_store_add_texture(asset_store_t *asset_store, SDL_Renderer *renderer, char* asset_id, char* filename)
{
	SDL_Surface *surface = IMG_Load(filename);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	hashmap_set(asset_store->textures, asset_id, sizeof(asset_id)-1, (uintptr_t)texture);
}

SDL_Texture*
asset_store_get_texture(asset_store_t *asset_store, char *asset_id)
{
	uintptr_t texture;
	hashmap_get(asset_store->textures, asset_id, sizeof(asset_id)-1, &texture);
	return (SDL_Texture*)texture;
}

void
asset_store_free(asset_store_t *asset_store)
{
	hashmap_free(asset_store->textures);
}
