#include "collision_system.h"

int
update_collision_system(entity_t *entities, size_t num_entities)
{
	for (size_t i = 0; i < num_entities; i++) {
		if (!entities[i].components.boxcollider)
			return 1;
		if (!entities[i].components.transform)
			return 1;

		for (size_t j = 0; j < num_entities; j++) {
			if (entities[i].id != entities[j].id) {
				printf("checking %d with %d\n", entities[i].id, entities[j].id);

				if (!entities[j].components.boxcollider)
					return 1;
				if (!entities[j].components.transform)
					return 1;

				bool is_colliding = check_aabb_collision(
					entities[i].components.transform->x,
					entities[i].components.transform->y,
					entities[i].components.transform->w,
					entities[i].components.transform->h,
					entities[j].components.transform->x,
					entities[j].components.transform->y,
					entities[j].components.transform->w,
					entities[j].components.transform->h
				);

				if (is_colliding)
					printf("collision\n");
			}
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
