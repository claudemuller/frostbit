#ifndef STATE_H_
#define STATE_H_

#include "event_bus.h"
#include "entity.h"
#include "asset_store.h"

#define MAX_ENTITIES 50

typedef struct state_t {
	event_bus_t *event_bus;
	entity_t *entities;
	bool render_colliders;
} state_t;

state_t* state_new(void);
void state_init(struct state_t *self);
void state_render(struct state_t *self, SDL_Renderer *renderer);
void state_update(struct state_t *self, double dt);
void state_destroy(state_t *self);

#endif // STATE_H_
