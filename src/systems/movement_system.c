#include <stdlib.h>
#include "movement_system.h"

int
update_movement_system(GArray *entities, size_t num_entities, double dt)
{
	for (size_t i = 0; i < entities->len; i++) {
		entity_t entity = g_array_index(entities, entity_t, i);
		if (!entity.components.transform)
			return 1;
		if (!entity.components.rigidbody)
			return 1;
		if (!entity.components.sprite)
			return 1;

		component_transform_t *transform = entity.components.transform;

		transform->position.x += entity.components.rigidbody->velocity.x * dt;
		transform->position.y += entity.components.rigidbody->velocity.y * dt;
	}

	return 0;
}

