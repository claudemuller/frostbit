#include "../array.h"
#include "movement_system.h"

int update_movement_system(entity_t *entities, const double dt)
{
    size_t entities_len = array_length(entities);
    for (size_t i = 0; i < entities_len; i++) {
        entity_t entity = entities[i];
        if (!entity.components.transform) {
            continue;
        }

        if (!entity.components.rigidbody) {
            continue;
        }

        if (!entity.components.sprite) {
            continue;
        }

        component_transform_t *transform = entity.components.transform;

        transform->position.x += entity.components.rigidbody->velocity.x * dt;
        transform->position.y += entity.components.rigidbody->velocity.y * dt;
    }

    return 0;
}
