#include "collision_system.h"

static bool check_aabb_collision(
    const double aX, const double aY,
    const double aW, const double aH,
    const double bX, const double bY,
    const double bW, const double bH
)
{
    return aX < bX + bW
        && aX + aW > bX
        && aY < bY + bH
        && aY + aH > bY;
}

int update_collision_system(event_bus_t *event_bus, entity_t *entities, size_t num_entities)
{
    for (size_t i = 0; i < num_entities; i++) {
        entity_t entityA = entities[i];

        if (!entityA.components.boxcollider) {
            return 1;
        }
        if (!entityA.components.transform) {
            return 1;
        }

        component_transform_t *transformA = entityA.components.transform;
        component_boxcollider_t *colliderA = entityA.components.boxcollider;

        for (size_t j = 0; j < num_entities; j++) {
            entity_t entityB = entities[j];
            if (entityA.id == entityB.id) {
                continue;
            }

            if (!entityB.components.boxcollider) {
                return 1;
            }
            if (!entityB.components.transform) {
                return 1;
            }

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
