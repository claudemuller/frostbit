#ifndef ASSET_STORE
#define ASSET_STORE

#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <SDL2/SDL.h>
#include "map.h"

typedef struct {
	GHashTable *textures;
} asset_store_t;

void asset_store_add_texture(asset_store_t *asset_store, SDL_Renderer *renderer, char* asset_id, char* filename);
SDL_Texture* asset_store_get_texture(asset_store_t *asset_store, char *asset_id);
void asset_store_init(asset_store_t *asset_store);
void asset_store_free(asset_store_t *asset_store);

#endif // ASSET_STORE
