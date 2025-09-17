#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "entity.h"

typedef void (*SystemFn)(EntityManager *ent_man, SDL_Renderer *renderer, Entity e, void *ctx);

typedef struct {
    Signature component_mask;
    SystemFn fn;
    void *ctx;
} System;

void movement_sys_update(EntityManager *ent_man, SDL_Renderer *renderer, Entity e, void *ctx);
void render_sys_update(EntityManager *ent_man, SDL_Renderer *renderer, Entity e, void *ctx);

#endif // !SYSTEM_H_
