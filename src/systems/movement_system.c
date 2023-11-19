#include "../array.h"
#include "movement_system.h"

int update_movement_system(entity_t *entities, double dt)
{
    size_t entities_len = array_length(entities);
    for (size_t i = 0; i < entities_len; i++) {
        entity_t entity = entities[i];
        if (!entity.components.transform) {
            return 1;
        }

        if (!entity.components.rigidbody) {
            return 1;
        }

        if (!entity.components.sprite) {
            return 1;
        }

        component_transform_t *transform = entity.components.transform;

        transform->position.x += entity.components.rigidbody->velocity.x * dt;
        transform->position.y += entity.components.rigidbody->velocity.y * dt;
    }

    return 0;
}
