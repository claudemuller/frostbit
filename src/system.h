#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "entity.h"
#include "state.h"
#include <assert.h>
#include <stdint.h>

#define MAX_SYSTEMS 16

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

typedef void (*SystemFn)(GameState* state, Entity e, SystemCtx* ctx);

typedef struct System {
    Signature component_mask;
    SystemFn fn;
    void* ctx;
} System;

typedef struct SystemManager {
    System update_systems[MAX_SYSTEMS];
    System render_systems[MAX_SYSTEMS];
    size_t n_update_systems;
    size_t n_render_systems;
} SystemManager;

#define SYS_SIG_MOVEMENT ((1U << COMP_TRANSFORM) | (1U << COMP_RIGID_BODY))
#define SYS_SIG_CAMERA_MOVEMENT ((1U << COMP_TRANSFORM) | (1U << COMP_CAMERA_FOLLOW))
#define SYS_SIG_KEYBOARD_CONTROL ((1U << COMP_KEYBOARD_CONTROL) | (1U << COMP_SPRITE))
#define SYS_SIG_MOUSE_CONTROL ((1U << COMP_MOUSE_CONTROL))
#define SYS_SIG_COLLISION ((1U << COMP_TRANSFORM) | (1U << COMP_BOX_COLLIDER))
#define SYS_SIG_PHYSICS ((1U << COMP_TRANSFORM) | (1U << COMP_RIGID_BODY))
#define SYS_SIG_APPLY ((1U << COMP_TRANSFORM) | (1U << COMP_RIGID_BODY))
#define SYS_SIG_DEBUG ((1U << COMP_TILEINFO))

#define SYS_SIG_RENDER ((1U << COMP_TRANSFORM) | (1U << COMP_SPRITE))
#define SYS_SIG_TILEMAP_RENDER ((1U << COMP_NONE))
#define SYS_SIG_RENDER_COLLIDER ((1U << COMP_TRANSFORM) | (1U << COMP_BOX_COLLIDER))
#define SYS_SIG_ANIMATION ((1U << COMP_ANIMATION))

static inline SystemManager* sysmgr_init(MemoryArena* mem)
{
    SystemManager* sysmgr = (SystemManager*)arena_alloc_aligned(mem, sizeof(SystemManager), 16);
    assert(sysmgr && "Failed to allocate system manager.");

    sysmgr->n_update_systems = 0;
    sysmgr->n_render_systems = 0;

    return sysmgr;
}

static inline void sysmgr_register_update_sys(SystemManager* mgr, uint32_t mask, SystemFn fn, SystemCtx* ctx)
{
    if (mgr->n_update_systems >= MAX_SYSTEMS) return;
    mgr->update_systems[mgr->n_update_systems++] = (System){
        .component_mask = mask,
        .fn = fn,
        .ctx = ctx,
    };
}

static inline void sysmgr_register_render_sys(SystemManager* mgr, uint32_t mask, SystemFn fn, SystemCtx* ctx)
{
    if (mgr->n_render_systems >= MAX_SYSTEMS) return;
    mgr->render_systems[mgr->n_render_systems++] = (System){
        .component_mask = mask,
        .fn = fn,
        .ctx = ctx,
    };
}

static inline void sysmgr_update_entity(GameState* state, Entity e)
{
    for (size_t i = 0; i < state->sysmgr->n_update_systems; ++i) {
        System* s = &state->sysmgr->update_systems[i];

        // Keyboard system is called manually on key event
        if (s->component_mask == SYS_SIG_KEYBOARD_CONTROL) continue;

        if (SIGNATURE_MATCH(state->entmgr->signatures[e], s->component_mask)) s->fn(state, e, s->ctx);
    }
}

static inline void sysmgr_render_entity(GameState* state, Entity e)
{
    for (size_t i = 0; i < state->sysmgr->n_render_systems; ++i) {
        System* s = &state->sysmgr->render_systems[i];

        if (SIGNATURE_MATCH(state->entmgr->signatures[e], s->component_mask)) s->fn(state, e, s->ctx);
    }
}

void sys_update_movement(GameState* state, Entity e, SystemCtx* ctx);
void sys_update_camera_movement(GameState* state, Entity e, SystemCtx* ctx);
void sys_update_keyboard_control(GameState* state, Entity e, SystemCtx* ctx);
void sys_update_mouse_control(GameState* state, Entity e, SystemCtx* ctx);
void sys_update_collision(GameState* state, Entity e, SystemCtx* ctx);
void sys_update_physics(GameState* state, Entity e, SystemCtx* ctx);
void sys_update_apply(GameState* state, Entity e, SystemCtx* ctx);
void sys_update_debug(GameState* state, Entity e, SystemCtx* ctx);

void sys_render_entities(GameState* state, Entity e, SystemCtx* ctx);
void sys_render_tilemap(GameState* state, Entity e, SystemCtx* ctx);
void sys_render_tilemap_collider(GameState* state, Entity e, SystemCtx* ctx);
void sys_render_collider(GameState* state, Entity e, SystemCtx* ctx);
void sys_render_animation(GameState* state, Entity e, SystemCtx* ctx);

#endif // !SYSTEM_H_
