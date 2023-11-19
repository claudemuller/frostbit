#ifndef ASSET_STORE_
#define ASSET_STORE_

#include "SDL.h"

typedef struct {
	SDL_Texture *texture;
	const char* id;	
} texture_t;

typedef struct {
	texture_t *textures;
} asset_store_t;

extern asset_store_t asset_store;

void asset_store_init(void);
void asset_store_add_texture(SDL_Renderer *renderer, const char* asset_id, const char* filename);
SDL_Texture* asset_store_get_texture(const char *asset_id);
void asset_store_free(void);

#endif // ASSET_STORE_
