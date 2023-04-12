#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL2/SDL.h>
#include "../entity.h"

int update_render_system(SDL_Renderer *renderer, entity_t *entities, size_t num_entities);

#endif // RENDER_SYSTEM_H

