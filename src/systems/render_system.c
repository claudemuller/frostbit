#include "render_system.h"

int
update_render_system(SDL_Renderer *renderer, entity_t *entities, size_t num_entities)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.transform)
			return 1;

		SDL_Rect dst = {
			.x = entities[i].components.transform->x,
			.y = entities[i].components.transform->y,
			.w = entities[i].components.transform->w,
			.h = entities[i].components.transform->h,
		};
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &dst);
	}

	return 0;
}
