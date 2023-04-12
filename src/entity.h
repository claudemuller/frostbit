#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "vector.h"

typedef struct {
	float x;
	float y;
	int w;
	int h;
} component_transform_t;

typedef struct {
	vec2_t velocity;
} component_rigidbody_t;

typedef struct {
	component_transform_t *transform;
	component_rigidbody_t *rigidbody;
} components_t;

typedef struct {
	unsigned int id;
	components_t components;
} entity_t;

void add_component_transform(entity_t *entity, float x, float y, int w, int h);
void add_component_rigidbody(entity_t *entity, vec2_t vel);
void free_entity(entity_t *entity);
int update_transform_system(entity_t *entity, double dt);
int update_render_system(SDL_Renderer *renderer, entity_t *entity);

#endif // ENTITY_H
