#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../vector.h"

typedef struct {
	vec2_t position;
	vec2_t scale;
	float rotation;
} component_transform_t;

#endif // TRANSFORM_COMPONENT_H
