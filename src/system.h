#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "entity.h"
#include "state.h"
#include <stdint.h>

#define MAX_SYSTEMS 16

typedef void (*SystemFn)(GameState* state, Entity e, void* ctx);

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

static inline void sysmgr_update_entity(GameState* state, Entity e)
{
    for (uint32_t i = 0; i < state->sysmgr->count; ++i) {
        System* s = &state->sysmgr->systems[i];
        if (SIGNATURE_MATCH(state->entmgr->signatures[e], s->component_mask)) s->fn(state, e, s->ctx);
    }
}

void movement_sys_update(GameState* state, Entity e, void* ctx);
void render_sys_render(GameState* state, Entity e, void* ctx);
void render_collider_sys_render(GameState* state, Entity e, void* ctx);
void keyboard_control_sys_update(GameState* state, Entity e, void* ctx);
void mouse_control_sys_update(GameState* state, Entity e, void* ctx);

#endif // !SYSTEM_H_
