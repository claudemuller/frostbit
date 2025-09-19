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

#define SYS_SIG_MOVEMENT ((1U << COMP_TRANSFORM) | (1U << COMP_RIGID_BODY))
#define SYS_SIG_RENDER ((1U << COMP_TRANSFORM) | (1U << COMP_SPRITE))
#define SYS_SIG_RENDER_COLLIDER ((1U << COMP_TRANSFORM) | (1U << COMP_BOX_COLLIDER))
#define SYS_SIG_KEYBOARD_CONTROL ((1U << COMP_KEYBOARD_CONTROL))
#define SYS_SIG_MOUSE_CONTROL ((1U << COMP_MOUSE_CONTROL))
#define SYS_SIG_ANIMATION ((1U << COMP_ANIMATION))
#define SYS_SIG_COLLISION ((1U << COMP_TRANSFORM) | (1U << COMP_BOX_COLLIDER))

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

        // Keyboard system is called manually on key event
        if (s->component_mask == SYS_SIG_KEYBOARD_CONTROL) continue;

        if (SIGNATURE_MATCH(state->entmgr->signatures[e], s->component_mask)) s->fn(state, e, s->ctx);
    }
}

bool check_aabb_collision(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh);
void movement_sys_update(GameState* state, Entity e, void* ctx);
void render_sys_render(GameState* state, Entity e, void* ctx);
void render_collider_sys_render(GameState* state, Entity e, void* ctx);
void keyboard_control_sys_update(GameState* state, Entity e, void* ctx);
void mouse_control_sys_update(GameState* state, Entity e, void* ctx);
void animation_sys_render(GameState* state, Entity e, void* ctx);
void collision_sys_update(GameState* state, Entity e, void* ctx);

#endif // !SYSTEM_H_
