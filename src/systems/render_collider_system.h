#ifndef RENDER_COLLIDER_SYSTEM_H
#define RENDER_COLLIDER_SYSTEM_H

#include <SDL2/SDL.h>
#include "../asset_store.h"
#include "../event_bus.h"
#include "../entity.h"

void init_render_collider_system(event_bus_t *event_bus);
int update_render_collider_system(SDL_Renderer *renderer, entity_t *entities, size_t num_entities);

#endif // RENDER_COLLIDER_SYSTEM_H
