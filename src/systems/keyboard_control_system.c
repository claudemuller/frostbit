#include "keyboard_control_system.h"
#include <stdio.h>

static void key_press(vec2_t args)
{
    printf("key pressed: %f\n", args.x);
}

void init_keyboard_control_system(void)
{
    event_bus_on_event(EVT_PLAYER_MOVE, &key_press);
}

int update_keyboard_control_system(entity_t *entities)
{
}
