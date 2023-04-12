#include "entity.h"

void
add_component_transform(entity_t *entity, float x, float y, int w, int h)
{
	entity->components.transform = malloc(sizeof(component_transform_t));
	if (entity->components.transform == NULL)
		SDL_Log("failed to add transform component to entity");

	printf("entity %d gets transform [%d:%d %d:%d]\n", entity->id, x, y, w, h);
	entity->components.transform->x = x;
	entity->components.transform->y = y;
	entity->components.transform->w = w;
	entity->components.transform->h = h;
}

void
add_component_rigidbody(entity_t *entity, vec2_t vel)
{
	entity->components.rigidbody = malloc(sizeof(component_rigidbody_t));
	if (entity->components.rigidbody == NULL)
		SDL_Log("failed to add rigidbody component to entity");

	entity->components.rigidbody->velocity.x = vel.x;
	entity->components.rigidbody->velocity.y = vel.y;
}

void
free_entity(entity_t *entity)
{
	if (entity->components.transform) free(entity->components.transform);
	if (entity->components.rigidbody) free(entity->components.rigidbody);
}

int
update_transform_system(entity_t *entities, size_t num_entities, double dt)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.transform)
			return 1;
		if (!entities[i].components.rigidbody)
			return 1;

		entities[i].components.transform->x += entities[i].components.rigidbody->velocity.x * dt;
		entities[i].components.transform->y += entities[i].components.rigidbody->velocity.y * dt;
	}

	return 0;
}

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
