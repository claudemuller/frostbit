#include "array.h"
#include "state_manager.h"
#include <stdlib.h>

static state_manager_t stateman = { 0 };

bool state_manager_init(void)
{
    stateman.stack = malloc(stateman.cap * sizeof(stateman.stack));
    if (!stateman.stack) {
        return false;
    }

    stateman.cap = 3;
    stateman.top = -1;

    return true;
}

void state_manager_free(void)
{
    do {
        int top = state_manager_pop();
        array_free(stateman.stack[top]->entities);
    } while (stateman.top > -1);

    free(stateman.stack);
}

bool state_manager_push(state_t *state)
{
    if (stateman.top + 1 == stateman.cap) {
        if (!state_manager_grow()) {
            SDL_LogError(1, "Error growing state manager's stack\n");
            return false;
        }
    }

    stateman.top++;
    stateman.stack[stateman.top] = state;

    return true;
}

int state_manager_pop(void)
{
    if (stateman.top == -1) {
        return 0;
    }

    state_t *top = state_manager_top();
    if (!top) {
        SDL_LogError(1, "Error poping top state\n");
        return false;
    }

    state_destroy(top);
    stateman.stack[stateman.top] = NULL;
    stateman.top--;

    return stateman.top;
}

state_t *state_manager_top(void)
{
    if (stateman.top == -1) {
        return NULL;
    }

    return stateman.stack[stateman.top];
}

bool state_manager_update(double dt)
{
    state_t *cur_state = state_manager_top();
    if (!cur_state) {
        SDL_LogError(1, "Error poping top state\n");
        return false;
    }

    state_update(cur_state, dt);

    return true;
}

bool state_manager_render(SDL_Renderer *renderer)
{
    state_t *cur_state = state_manager_top();
    if (!cur_state) {
        SDL_LogError(1, "Error poping top state\n");
        return false;
    }

    state_render(cur_state, renderer);

    return true;
}

bool state_manager_grow(void)
{
    state_t **tmp = realloc(stateman.stack, stateman.cap * 2 * sizeof(state_t *));
    if (!tmp) {
        fprintf(stderr, "error resizing state manager");
        return false;
    }

    stateman.cap *= 2;
    stateman.stack = tmp;

    return true;
}
