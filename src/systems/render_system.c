#include <SDL2/SDL_image.h>
#include "render_system.h"
#include "../components/transform_component.h"

int
update_render_system(SDL_Renderer *renderer, asset_store_t *asset_store, entity_t *entities, size_t num_entities)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.transform)
			return 1;
		if (!entities[i].components.sprite)
			return 1;

		component_transform_t *transform = entities[i].components.transform;
		component_sprite_t *sprite = entities[i].components.sprite;

		SDL_Rect srcRect = sprite->srcRect;
		SDL_Rect dstRect = {
			transform->x,
			transform->y,
			transform->w,
			transform->h
		};

		SDL_RenderCopyEx(
				renderer,
				asset_store_get_texture(asset_store, sprite->id),
				&srcRect,
				&dstRect,
				0,
				NULL,
				0
		);
	}

	return 0;
}
