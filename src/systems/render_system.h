#ifndef RENDER_SYSTEM_H_
#define RENDER_SYSTEM_H_

#include <SDL2/SDL.h>
#include "../entity.h"
#include "../asset_store.h"

int update_render_system(SDL_Renderer *renderer, entity_t *entities, size_t num_entities);

#endif // RENDER_SYSTEM_H_

