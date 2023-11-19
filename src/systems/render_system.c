#include "../array.h"
#include "../components/transform_component.h"
#include "render_system.h"
#include <SDL2/SDL_image.h>
#include <string.h>

int update_render_system(SDL_Renderer *renderer, entity_t *entities)
{
    size_t entities_len = array_length(entities);
    for (size_t i = 0; i < entities_len; i++) {
        entity_t entity = entities[i];

        if (!entity.components.transform) {
            return 1;
        }
        if (!entity.components.sprite) {
            return 1;
        }

        component_transform_t *transform = entity.components.transform;
        component_sprite_t *sprite = entity.components.sprite;

        SDL_Rect srcRect = sprite->src_rect;
        SDL_Rect dstRect = {
            transform->position.x,
            transform->position.y,
            sprite->width * transform->scale.x,
            sprite->height * transform->scale.y
        };

        SDL_RenderCopyEx(
            renderer,
            asset_store_get_texture(sprite->id),
            &srcRect,
            &dstRect,
            transform->rotation,
            NULL,
            sprite->is_flipped
        );
    }

    return 0;
}
