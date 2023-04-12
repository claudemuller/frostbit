#include <stdlib.h>
#include "movement_system.h"

int
update_movement_system(entity_t *entities, size_t num_entities, double dt)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.transform)
			return 1;

		entities[i].components.transform->x += entities[i].components.rigidbody->velocity.x * dt;
		entities[i].components.transform->y += entities[i].components.rigidbody->velocity.y * dt;
	}

	return 0;
}

