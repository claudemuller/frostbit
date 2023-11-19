#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#include "state.h"
#include <stdbool.h>

typedef struct {
	state_t **stack;
	int cap;
	int top;
} state_manager_t;

bool state_manager_init(state_manager_t *stateman);
void state_manager_free(state_manager_t *stateman);
bool state_manager_push(state_manager_t *stateman, state_t *state);
int state_manager_pop(state_manager_t *stateman);
state_t *state_manager_top(state_manager_t *stateman);
bool state_manager_update(state_manager_t *stateman, double dt);
bool state_manager_render(state_manager_t *stateman, SDL_Renderer *renderer);
bool state_manager_grow(state_manager_t *stateman);

#endif // STATE_MANAGER_H_
