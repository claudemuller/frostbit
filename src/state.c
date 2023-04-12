#include "state.h"
#include "systems/movement_system.h"
#include "systems/collision_system.h"
#include "systems/render_system.h"

const size_t ENTS_LEN = 3;

state_t*
state_new(entity_t *entities, size_t num_entites)
{
	state_t *state = malloc(sizeof(state_t));
	// TODO: err handling

	state->ents = entities;
	state->num_entites = num_entites;

	state->init = state_init;
	state->render = state_render;
	state->update = state_update;
	state->destroy = state_destroy;

	return state;
}

void
state_init(struct state_t *self)
{

}

void
state_render(state_t *self, SDL_Renderer *renderer, asset_store_t *asset_store)
{
	update_render_system(renderer, asset_store, self->ents, self->num_entites);
}

void
state_update(state_t *self, double dt)
{
	update_movement_system(self->ents, self->num_entites, dt);
	update_collision_system(self->ents, self->num_entites);
}

void
state_destroy(state_t *self)
{
	for (size_t i = 0; i < self->num_entites; i++) {
		free_entity(&self->ents[i]);
	}
	free(self->ents);
	free(self);
}
