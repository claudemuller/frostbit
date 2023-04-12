#include <stdlib.h>
#include "state_manager.h"

int
state_manager_init(state_manager_t *stateman) {
	stateman->cap = 3;
	stateman->stack = malloc(stateman->cap * sizeof(state_t*));
	stateman->top = -1;
	return 0;
}

int
state_manager_free(state_manager_t *stateman)
{
	do {
		state_manager_pop(stateman);
	} while (stateman->top > -1);
	free(stateman->stack);
	return 0;
}

int
state_manager_push(state_manager_t *stateman, state_t *state)
{
	if (stateman->top+1 == stateman->cap)
		state_manager_grow(stateman);

	stateman->top++;
	stateman->stack[stateman->top] = state;

	if (state->init != NULL)
		state->init();

	return stateman->top;
}

int
state_manager_pop(state_manager_t *stateman)
{
	if (stateman->top == -1)
		return 0;

	state_t *top = state_manager_top(stateman);
	if (top != NULL && top->destroy != NULL)
		top->destroy();

	stateman->stack[stateman->top] = NULL;
	stateman->top--;

	return stateman->top;
}

state_t *
state_manager_top(state_manager_t *stateman)
{
	if (stateman->top == -1)
		return NULL;
	return stateman->stack[stateman->top];
}

int
state_manager_update(state_manager_t *stateman, double dt)
{
	state_t *state = state_manager_top(stateman);
	if (state != NULL && state->update != NULL)
		return state->update(dt);
	return 1;
}

int
state_manager_render(state_manager_t *stateman, double dt)
{
	// state_t *state = state_manager_top(stateman);
	// if (state != NULL && state->draw != NULL)
	// 	return state->draw(dt);
	// return 1;
	return 0;
}

int
state_manager_grow(state_manager_t *stateman)
{
	stateman->cap *= 2;
	stateman->stack = realloc(stateman->stack, stateman->cap * sizeof(state_t*));
	return stateman->cap;
}
