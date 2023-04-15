#include "render_collider_system.h"

int
update_render_collider_system(SDL_Renderer *renderer, entity_t *entities, size_t num_entities)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.transform)
			return 1;
		if (!entities[i].components.boxcollider)
			return 1;

		component_transform_t *transform = entities[i].components.transform;
		component_boxcollider_t *collider = entities[i].components.boxcollider;

		SDL_Rect collider_rect = {
			transform->position.x,
			transform->position.y,
			collider->width * transform->scale.x,
			collider->height * transform->scale.y
		};

		SDL_SetRenderDrawColor(renderer, collider->colour.r, collider->colour.g, collider->colour.b, collider->colour.a);
		SDL_RenderDrawRect(renderer, &collider_rect);
	}

	return 0;
}
