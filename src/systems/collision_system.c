#include "../array.h"
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

int update_collision_system(entity_t *entities)
{
    size_t entities_len = array_length(entities);
    for (size_t i = 0; i < entities_len; i++) {
        entity_t entityA = entities[i];

        if (!entityA.components.boxcollider) {
            return 1;
        }
        if (!entityA.components.transform) {
            return 1;
        }

        component_transform_t *transformA = entityA.components.transform;
        component_boxcollider_t *colliderA = entityA.components.boxcollider;

        for (size_t j = 0; j < entities_len; j++) {
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
                args_t args = { 0 };
                event_bus_emit(EVT_DESTROY_ENTITY, args);
            }
        }
    }

    return 0;
}
