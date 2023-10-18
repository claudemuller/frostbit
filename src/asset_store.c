#include <SDL2/SDL_image.h>
#include "asset_store.h"

void asset_store_init(asset_store_t *asset_store)
{
	// asset_store->textures = g_hash_table_new(g_str_hash, g_str_equal);
}

void asset_store_add_texture(asset_store_t *asset_store, SDL_Renderer *renderer, char* asset_id, char* filename)
{
	SDL_Surface *surface = IMG_Load(filename);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	// g_hash_table_insert(asset_store->textures, asset_id, texture);
}

SDL_Texture *asset_store_get_texture(asset_store_t *asset_store, char *asset_id)
{
	// SDL_Texture *texture = (SDL_Texture*)g_hash_table_lookup(asset_store->textures, asset_id);
	// return texture;
}

void asset_store_free(asset_store_t *asset_store)
{
	// g_hash_table_destroy(asset_store->textures);
}
