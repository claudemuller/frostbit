#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "entity.h"
#include <stdint.h>

#define MAX_SYSTEMS 16

typedef void (*SystemFn)(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx);

typedef struct System {
    Signature component_mask;
    SystemFn fn;
    void* ctx;
} System;

typedef struct SystemManager {
    System systems[MAX_SYSTEMS];
    uint32_t count;
} SystemManager;

static inline void sysmgr_register(SystemManager* mgr, uint32_t mask, SystemFn fn, void* ctx)
{
    if (mgr->count >= MAX_SYSTEMS) return;
    mgr->systems[mgr->count++] = (System){
        .component_mask = mask,
        .fn = fn,
        .ctx = ctx,
    };
}

static inline void sysmgr_update_entity(SystemManager* mgr, EntityManager* entmgr, SDL_Renderer* renderer, Entity e)
{
    for (uint32_t i = 0; i < mgr->count; ++i) {
        System* s = &mgr->systems[i];
        if (SIGNATURE_MATCH(entmgr->signatures[e], s->component_mask)) s->fn(entmgr, renderer, e, s->ctx);
    }
}

void movement_sys_update(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx);
void render_sys_update(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx);

#endif // !SYSTEM_H_
