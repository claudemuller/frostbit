#ifndef RENDER_SYSTEM_H_
#define RENDER_SYSTEM_H_

#include "../entity.h"
#include "../asset_store.h"
#include "SDL.h"

int update_render_system(SDL_Renderer *renderer, entity_t *entities);

#endif // RENDER_SYSTEM_H_

