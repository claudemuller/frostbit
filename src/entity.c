#include "entity.h"

void
add_component_transform(entity_t *entity, float x, float y, int w, int h)
{
	entity->components.transform = malloc(sizeof(component_transform_t));
	if (entity->components.transform == NULL)
		SDL_Log("failed to add transform component to entity");

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
add_component_boxcollider(entity_t *entity, vec2_t bounds)
{
	entity->components.boxcollider = malloc(sizeof(component_boxcollider_t));
	if (entity->components.boxcollider == NULL)
		SDL_Log("failed to add boxcollider component to entity");

	entity->components.boxcollider->bounds.x = bounds.x;
	entity->components.boxcollider->bounds.y = bounds.y;
}

void
free_entity(entity_t *entity)
{
	if (entity && entity->components.transform) free(entity->components.transform);
	if (entity && entity->components.rigidbody) free(entity->components.rigidbody);
	if (entity && entity->components.boxcollider) free(entity->components.boxcollider);
}

