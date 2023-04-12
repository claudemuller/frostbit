#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "state.h"

typedef struct {
	state_t **stack;
	int cap;
	int top;
} state_manager_t;

int state_manager_init(state_manager_t *stateman);
int state_manager_free(state_manager_t *stateman);
int state_manager_push(state_manager_t *stateman, state_t *state);
int state_manager_pop(state_manager_t *stateman);
state_t *state_manager_top(state_manager_t *stateman);
int state_manager_update(state_manager_t *stateman, double dt);
int state_manager_render(state_manager_t *stateman, SDL_Renderer *renderer, asset_store_t *asset_store);
int state_manager_grow(state_manager_t *stateman);

#endif // STATE_MANAGER_H
