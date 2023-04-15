#include "entity.h"

void
add_component_transform(entity_t *entity, float x, float y, vec2_t scale)
{
	entity->components.transform = malloc(sizeof(component_transform_t));
	if (entity->components.transform == NULL)
		SDL_Log("failed to add transform component to entity");

	entity->components.transform->position.x = x;
	entity->components.transform->position.y = y;
	entity->components.transform->scale.x = scale.x;
	entity->components.transform->scale.y = scale.y;
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
add_component_boxcollider(entity_t *entity, int width, int height, vec2_t offset, SDL_Color colour)
{
	entity->components.boxcollider = malloc(sizeof(component_boxcollider_t));
	if (entity->components.boxcollider == NULL)
		SDL_Log("failed to add boxcollider component to entity");

	entity->components.boxcollider->width = width;
	entity->components.boxcollider->height = height;
	entity->components.boxcollider->offset = offset;
	entity->components.boxcollider->colour = colour;
}

void add_component_sprite(
	entity_t *entity,
	char *asset_id,
	int width,
	int height,
	int src_x,
	int src_y,
	int z_index,
	bool is_fixed,
	SDL_RendererFlip is_flipped
)
{
	entity->components.sprite = malloc(sizeof(component_sprite_t));
	if (entity->components.sprite == NULL)
		SDL_Log("failed to add sprite component to entity");

	entity->components.sprite->id = asset_id;
	entity->components.sprite->width = width;
	entity->components.sprite->height = height;
	entity->components.sprite->z_index = z_index;
	entity->components.sprite->is_fixed = is_fixed;
	entity->components.sprite->is_flipped = is_flipped;
	entity->components.sprite->src_rect = (SDL_Rect){
		.x = src_x,
		.y = src_y,
		.w = width,
		.h = height
	};
}

void
free_entity(entity_t *entity)
{
	if (entity && entity->components.transform) free(entity->components.transform);
	if (entity && entity->components.rigidbody) free(entity->components.rigidbody);
	if (entity && entity->components.boxcollider) free(entity->components.boxcollider);
}

