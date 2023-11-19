#ifndef KEYBOARD_CONTROL_COMPONENT_
#define KEYBOARD_CONTROL_COMPONENT_

#include "../vector.h"

typedef struct {
	vec2_t up_vel;
	vec2_t right_vel;
	vec2_t down_vel;
	vec2_t left_vel;
} component_keyboard_control_t;

#endif // KEYBOARD_CONTROL_COMPONENT_
