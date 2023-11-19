#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#include "state.h"
#include <stdbool.h>

typedef struct {
	state_t **stack;
	int cap;
	int top;
} state_manager_t;

bool state_manager_init(void);
void state_manager_free(void);
bool state_manager_push(state_t *state);
int state_manager_pop(void);
state_t *state_manager_top(void);
bool state_manager_update(double dt);
bool state_manager_render(SDL_Renderer *renderer);
bool state_manager_grow(void);

#endif // STATE_MANAGER_H_
