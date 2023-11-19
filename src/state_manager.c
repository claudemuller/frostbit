#include "state_manager.h"
#include <stdlib.h>

bool state_manager_init(state_manager_t *stateman)
{
    stateman->stack = malloc(stateman->cap * sizeof(stateman->stack));
    if (!stateman->stack) {
        return false;
    }

    stateman->cap = 3;
    stateman->top = -1;

    return true;
}

void state_manager_free(state_manager_t *stateman)
{
    do {
        state_manager_pop(stateman);
    } while (stateman->top > -1);

    free(stateman->stack);
}

bool state_manager_push(state_manager_t *stateman, state_t *state)
{
    if (stateman->top + 1 == stateman->cap) {
        if (!state_manager_grow(stateman)) {
            SDL_LogError(1, "Error growing state manager's stack\n");
            return false;
        }
    }

    stateman->top++;
    stateman->stack[stateman->top] = state;

    // state_init(state);

    return true;
}

int state_manager_pop(state_manager_t *stateman)
{
    if (stateman->top == -1) {
        return 0;
    }

    state_t *top = state_manager_top(stateman);
    if (!top) {
        SDL_LogError(1, "Error poping top state\n");
        return false;
    }

    state_destroy(top);
    stateman->stack[stateman->top] = NULL;
    stateman->top--;

    return stateman->top;
}

state_t *state_manager_top(state_manager_t *stateman)
{
    if (stateman->top == -1) {
        return NULL;
    }

    return stateman->stack[stateman->top];
}

bool state_manager_update(state_manager_t *stateman, double dt)
{
    state_t *cur_state = state_manager_top(stateman);
    if (!cur_state) {
        SDL_LogError(1, "Error poping top state\n");
        return false;
    }

    state_update(cur_state, dt);

    return true;
}

bool state_manager_render(state_manager_t *stateman, SDL_Renderer *renderer)
{
    state_t *cur_state = state_manager_top(stateman);
    if (!cur_state) {
        SDL_LogError(1, "Error poping top state\n");
        return false;
    }

    state_render(cur_state, renderer);

    return true;
}

bool state_manager_grow(state_manager_t *stateman)
{
    state_t **tmp = realloc(stateman->stack, stateman->cap * 2 * sizeof(state_t *));
    if (!tmp) {
        fprintf(stderr, "error resizing state manager");
        return false;
    }

    stateman->cap *= 2;
    stateman->stack = tmp;

    return true;
}
