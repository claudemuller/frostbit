#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "vector.h"
#include "components/transform_component.h"
#include "components/rigidbody_component.h"
#include "components/boxcollider_component.h"
#include "components/sprite_component.h"

typedef struct {
	component_transform_t *transform;
	component_rigidbody_t *rigidbody;
	component_boxcollider_t *boxcollider;
	component_sprite_t *sprite;
} components_t;

typedef struct {
	unsigned int id;
	components_t components;
} entity_t;

void add_component_transform(entity_t *entity, float x, float y, vec2_t scale);
void add_component_rigidbody(entity_t *entity, vec2_t vel);
void add_component_boxcollider(entity_t *entity, int width, int height, vec2_t offset, SDL_Color colour);
void add_component_sprite(
	entity_t *entity,
	char *asset_id,
	int width,
	int height,
	int x,
	int y,
	int z_index,
	bool is_fixed,
	SDL_RendererFlip is_flipped
);
void free_entity(entity_t *entity);

#endif // ENTITY_H
