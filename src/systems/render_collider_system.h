#ifndef RENDER_COLLIDER_SYSTEM_H
#define RENDER_COLLIDER_SYSTEM_H

#include <SDL2/SDL.h>
#include "../asset_store.h"
#include "../entity.h"

int update_render_collider_system(SDL_Renderer *renderer, entity_t *entities, size_t num_entities);

#endif // RENDER_COLLIDER_SYSTEM_H
