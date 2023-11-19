#include "../array.h"
#include "keyboard_control_system.h"
#include <stdio.h>

#define PLAYER_SPEED 100

// TODO: redo this
static SDL_Event key_event = { 0 };

static void key_press(args_t args)
{
    key_event = args.event;
    printf("key pressed\n");
}

void init_keyboard_control_system(void)
{
    event_bus_on_event(EVT_PLAYER_MOVE, &key_press);
}

int update_keyboard_control_system(entity_t *entities, const double dt)
{
    size_t entities_len = array_length(entities);
    for (size_t i = 0; i < entities_len; i++) {
        entity_t entity = entities[i];
        if (!entity.components.transform) {
            return 1;
        }

        if (!entity.components.rigidbody) {
            return 1;
        }

        if (!entity.components.keyboard_control) {
            return 1;
        }

        vec2_t move_vec = { 0 };

        switch (key_event.key.keysym.sym) {
        case SDLK_w: {
            move_vec.y = -1;
        } break;

        case SDLK_a: {
            move_vec.x = -1;
        } break;

        case SDLK_s: {
            move_vec.y = 1;
        } break;

        case SDLK_d: {
            move_vec.x = 1;
        } break;
        }

        component_transform_t *transform = entity.components.transform;
        component_rigidbody_t *rigidbody = entity.components.rigidbody;

        rigidbody->velocity.x += move_vec.x * PLAYER_SPEED * dt;
        rigidbody->velocity.y += move_vec.y * PLAYER_SPEED * dt;
    }
}
