#ifndef KEYBOARD_CONTROL_SYSTEM_
#define KEYBOARD_CONTROL_SYSTEM_

#include "../event_bus.h"
#include "../entity.h"
#include "SDL.h"

void init_keyboard_control_system(void);
int update_keyboard_control_system(entity_t *entities);

#endif // KEYBOARD_CONTROL_SYSTEM_
