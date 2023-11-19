#ifndef ENTITY_H_
#define ENTITY_H_

#include "components/keyboard_control_component.h"
#include "vector.h"
#include "components/transform_component.h"
#include "components/rigidbody_component.h"
#include "components/boxcollider_component.h"
#include "components/sprite_component.h"
#include "SDL.h"

typedef struct {
	component_transform_t *transform;
	component_rigidbody_t *rigidbody;
	component_boxcollider_t *boxcollider;
	component_sprite_t *sprite;
	component_keyboard_control_t *keyboard_control;
} components_t;

typedef struct {
	const char *id;
	components_t components;
} entity_t;

void add_component_transform(
	entity_t *entity,
	const float x,
	const float y,
	const vec2_t scale,
	const float rotation
);
void add_component_rigidbody(entity_t *entity, vec2_t vel);
void add_component_boxcollider(entity_t *entity, int width, int height, vec2_t offset, SDL_Color colour);
void add_component_sprite(
	entity_t *entity,
	const char *asset_id,
	const int width,
	const int height,
	const int src_x,
	const int src_y,
	const int z_index,
	const bool is_fixed,
	SDL_RendererFlip is_flipped
);
void add_component_keyboard_control(entity_t *entity);
void free_entity(entity_t *entity);

#endif // ENTITY_H_
