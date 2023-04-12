#ifndef STATE_H
#define STATE_H

#include "entity.h"
#include "asset_store.h"

typedef struct state_t {
	entity_t *ents;
	size_t num_entites;

	void (*init)(struct state_t *self);
	void (*update)(struct state_t *self, double dt);
	void (*render)(struct state_t *self, SDL_Renderer *renderer, asset_store_t *asset_store);
	void (*destroy)(struct state_t *self);
} state_t;

state_t* state_new(entity_t *entities, size_t num_entites);
void state_init(struct state_t *self);
void state_render(struct state_t *self, SDL_Renderer *renderer, asset_store_t *asset_store);
void state_update(struct state_t *self, double dt);
void state_destroy(state_t *self);

#endif // STATE_H
