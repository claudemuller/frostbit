#ifndef RENDER_COLLIDER_SYSTEM_H_
#define RENDER_COLLIDER_SYSTEM_H_

#include "../asset_store.h"
#include "../event_bus.h"
#include "../entity.h"
#include "SDL.h"

void init_render_collider_system(void);
int update_render_collider_system(SDL_Renderer *renderer, entity_t *entities);

#endif // RENDER_COLLIDER_SYSTEM_H_
