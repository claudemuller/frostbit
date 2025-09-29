#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "entity.h"
#include "state.h"
#include <assert.h>
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
#define SYS_SIG_CAMERA_MOVEMENT ((1U << COMP_TRANSFORM) | (1U << COMP_CAMERA_FOLLOW))
#define SYS_SIG_RENDER ((1U << COMP_TRANSFORM) | (1U << COMP_SPRITE))
#define SYS_SIG_RENDER_COLLIDER ((1U << COMP_TRANSFORM) | (1U << COMP_BOX_COLLIDER))
#define SYS_SIG_KEYBOARD_CONTROL ((1U << COMP_KEYBOARD_CONTROL) | (1U << COMP_SPRITE))
#define SYS_SIG_MOUSE_CONTROL ((1U << COMP_MOUSE_CONTROL))
#define SYS_SIG_ANIMATION ((1U << COMP_ANIMATION))
#define SYS_SIG_COLLISION ((1U << COMP_TRANSFORM) | (1U << COMP_BOX_COLLIDER))

typedef struct {
    float dt;
} MovementCtx;

typedef struct {
    SDL_Event ev;
} EventCtx;

typedef enum {
    CTX_NONE,
    CTX_MOVEMENT,
    CTX_EVENT,
    CTX_COUNT,
} CtxTag;

typedef struct {
    CtxTag tag;
    union {
        MovementCtx movement;
        EventCtx event;
    };
} SystemCtx;

static inline SystemManager* sysmgr_init(MemoryArena* mem)
{
    SystemManager* sysmgr = (SystemManager*)arena_alloc_aligned(mem, sizeof(SystemManager), 16);
    assert(sysmgr && "Failed to allocate system manager.");
    sysmgr->count = 0;
    return sysmgr;
}

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
    for (size_t i = 0; i < state->sysmgr->count; ++i) {
        System* s = &state->sysmgr->systems[i];

        // Keyboard system is called manually on key event
        if (s->component_mask == SYS_SIG_KEYBOARD_CONTROL) continue;

        if (SIGNATURE_MATCH(state->entmgr->signatures[e], s->component_mask)) s->fn(state, e, s->ctx);
    }
}

bool check_aabb_collision(f64 ax, f64 ay, f64 aw, f64 ah, f64 bx, f64 by, f64 bw, f64 bh);
void movement_sys_update(GameState* state, Entity e, void* ctx);
void camera_movement_sys_update(GameState* state, Entity e, void* ctx);
void render_sys_render(GameState* state, Entity e, void* ctx);
void render_collider_sys_render(GameState* state, Entity e, void* ctx);
void keyboard_control_sys_update(GameState* state, Entity e, void* ctx);
void mouse_control_sys_update(GameState* state, Entity e, void* ctx);
void animation_sys_render(GameState* state, Entity e, void* ctx);
void collision_sys_update(GameState* state, Entity e, void* ctx);

#endif // !SYSTEM_H_
