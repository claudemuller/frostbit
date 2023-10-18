#include "state.h"
#include "entity.h"
#include "event_bus.h"
#include "systems/movement_system.h"
#include "systems/collision_system.h"
#include "systems/render_system.h"
#include "systems/render_collider_system.h"

state_t*
state_new(entity_t *entities, size_t num_entities)
{
	state_t *state = (state_t*)malloc(sizeof(state_t));
	// TODO: err handling

	state->event_bus = new_event_bus();
	init_render_collider_system(state->event_bus);

	state->entities = entities;
	state->num_entites = num_entities;
	state->render_colliders = false;

	return state;
}

void
state_init(struct state_t *self)
{
}

void
state_render(state_t *self, SDL_Renderer *renderer, asset_store_t *asset_store)
{
	update_render_system(renderer, asset_store, self->entities, self->num_entites);
	update_render_collider_system(renderer, self->entities, self->num_entites);
}

void
state_update(state_t *self, double dt)
{
	update_movement_system(self->entities, self->num_entites, dt);
	update_collision_system(self->event_bus, self->entities, self->num_entites);
	self->event_bus->process_events(self->event_bus);
}

void
state_destroy(state_t *self)
{
	// for (size_t i = 0; i < self->num_entites; i++) {
	free(self->entities);
	// }
	self->event_bus->destroy(self->event_bus);
	free(self->event_bus);
	// free(self->entities);
	free(self);
}
