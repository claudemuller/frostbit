#include "../array.h"
#include "render_collider_system.h"

bool render_collider = false;

static void collide(args_t args)
{
    render_collider = true;
}

void init_render_collider_system(void)
{
    event_bus_on_event(EVT_DESTROY_ENTITY, &collide);
}

int update_render_collider_system(SDL_Renderer *renderer, entity_t *entities)
{
    if (!render_collider) {
        return 0;
    }
    render_collider = false;

    size_t entities_len = array_length(entities);
    for (size_t i = 0; i < entities_len; i++) {
        entity_t entity = entities[i];
        if (!entity.components.transform) {
            continue;
        }
        if (!entity.components.boxcollider) {
            continue;
        }

        component_transform_t *transform = entity.components.transform;
        component_boxcollider_t *collider = entity.components.boxcollider;

        SDL_Rect collider_rect = {
            transform->position.x,
            transform->position.y,
            collider->width * transform->scale.x,
            collider->height * transform->scale.y
        };

        SDL_SetRenderDrawColor(
            renderer,
            collider->colour.r,
            collider->colour.g,
            collider->colour.b,
            collider->colour.a
        );
        SDL_RenderDrawRect(renderer, &collider_rect);
    }

    return 0;
}
