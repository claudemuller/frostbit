#include "collision_system.h"

int
update_collision_system(entity_t *entities, size_t num_entities)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.boxcollider)
			return 1;
		if (!entities[i].components.transform)
			return 1;

		component_transform_t *transformA = entities[i].components.transform;
		component_boxcollider_t *colliderA = entities[i].components.boxcollider;

		for (size_t j = 0; j < num_entities; j++) {
			if (entities[i].id == entities[j].id) {
				continue;
			}

			if (!entities[j].components.boxcollider)
				return 1;
			if (!entities[j].components.transform)
				return 1;

			component_transform_t *transformB = entities[j].components.transform;
			component_boxcollider_t *colliderB = entities[j].components.boxcollider;

			bool is_colliding = check_aabb_collision(
					transformA->position.x, transformA->position.y,
					colliderA->width, colliderA->height,
					transformB->position.x, transformB->position.y,
					colliderB->width, colliderB->height
			);

			if (is_colliding)
				printf("collision\n");
		}
	}

	return 0;
}

bool
check_aabb_collision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
{
	return aX < bX + bW
		&& aX + aW > bX
		&& aY < bY + bH
		&& aY + aH > bY;
}
