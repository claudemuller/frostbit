#include "render_collider_system.h"

bool render_collider = false;

void
collide(void)
{
	printf("collision from event\n");
	render_collider = true;
}

void
init_render_collider_system(event_bus_t *event_bus)
{
	event_bus->on_event(event_bus, EVENT_DESTROY_ENTITY, &collide);
}

int
update_render_collider_system(SDL_Renderer *renderer, entity_t *entities, size_t num_entities)
{
	if (!render_collider)
		return 0;

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
