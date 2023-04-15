#include <stdlib.h>
#include "movement_system.h"

int
update_movement_system(entity_t *entities, size_t num_entities, double dt)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.transform)
			return 1;
		if (!entities[i].components.rigidbody)
			return 1;
		if (!entities[i].components.sprite)
			return 1;

		component_transform_t *transform = entities[i].components.transform;

		transform->position.x += entities[i].components.rigidbody->velocity.x * dt;
		transform->position.y += entities[i].components.rigidbody->velocity.y * dt;
	}

	return 0;
}

