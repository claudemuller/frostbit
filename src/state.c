#include "entity.h"
#include "event_bus.h"
#include "state.h"
#include "systems/collision_system.h"
#include "systems/keyboard_control_system.h"
#include "systems/movement_system.h"
#include "systems/render_collider_system.h"
#include "systems/render_system.h"

state_t *state_new(entity_t *entities)
{
    state_t *state = malloc(sizeof(*state));
    if (!state) {
        SDL_LogError(1, "Error allocating space for state\n");
        return NULL;
    }

    state->entities = entities;
    state->render_colliders = false;

    return state;
}

// void state_init(struct state_t *self)
// {
// }

void state_render(state_t *self, SDL_Renderer *renderer)
{
    update_render_system(renderer, self->entities);
    update_render_collider_system(renderer, self->entities);
}

void state_update(state_t *self, double dt)
{
    update_movement_system(self->entities, dt);
    update_collision_system(self->entities);
    update_keyboard_control_system(self->entities, dt);
}

void state_destroy(state_t *self)
{
    // for (size_t i = 0; i < self->num_entites; i++) {
    free(self->entities);
    // }
    free(self->event_bus);
    // free(self->entities);
    free(self);
}
