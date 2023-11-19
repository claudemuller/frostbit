#include "../array.h"
#include "keyboard_control_system.h"
#include <stdio.h>

#define PLAYER_SPEED 10

// TODO: redo this
static vec2_t key_press_vel = { 0 };

static void key_press(args_t args)
{
    key_press_vel = args.movement;
    printf("key pressed: %f\n", args.movement.x);
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

        component_transform_t *transform = entity.components.transform;
        component_rigidbody_t *rigidbody = entity.components.rigidbody;

        rigidbody->velocity.x += key_press_vel.x * PLAYER_SPEED * dt;
        rigidbody->velocity.y += key_press_vel.y * PLAYER_SPEED * dt;
    }

    return 0;
}
