#include "collision_system.h"

int
update_collision_system(event_bus_t *event_bus, GArray *entities, size_t num_entities)
{
	for (size_t i = 0; i < num_entities; i++) {
		entity_t entityA = g_array_index(entities, entity_t, i);
		if (!entityA.components.boxcollider)
			return 1;
		if (!entityA.components.transform)
			return 1;

		component_transform_t *transformA = entityA.components.transform;
		component_boxcollider_t *colliderA = entityA.components.boxcollider;

		for (size_t j = 0; j < num_entities; j++) {
			entity_t entityB = g_array_index(entities, entity_t, i);
			if (entityA.id == entityB.id) {
				continue;
			}

			if (!entityB.components.boxcollider)
				return 1;
			if (!entityB.components.transform)
				return 1;

			component_transform_t *transformB = entityB.components.transform;
			component_boxcollider_t *colliderB = entityB.components.boxcollider;

			bool is_colliding = check_aabb_collision(
					transformA->position.x, transformA->position.y,
					colliderA->width, colliderA->height,
					transformB->position.x, transformB->position.y,
					colliderB->width, colliderB->height
			);

			if (is_colliding) {
				event_bus->emit(event_bus, EVENT_DESTROY_ENTITY);
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
