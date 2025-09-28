#include "entity.h"
#include "arena.h"
#include <assert.h>
#include <stdint.h>

EntityManager* entitymgr_init(MemoryArena* mem)
{
    EntityManager* entmgr = (EntityManager*)arena_alloc_aligned(mem, sizeof(EntityManager), 16);
    util_debug("%ld\n", sizeof(EntityManager));
    assert(entmgr && "Failed to allocate entity manager.");
    entmgr->next_entity_id = 0;
    return entmgr;
}

Entity entity_create(EntityManager* entmgr)
{
    if (entmgr->next_entity_id >= MAX_ENTITIES) return UINT32_MAX;

    Entity e = entmgr->next_entity_id++;
    entmgr->signatures[e] = 0;
    entmgr->live_entities[e] = true;

    return e;
}

void entity_destroy(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->signatures[e] = 0;
    entmgr->live_entities[e] = false;
}

// ------------------------------------------------------------------------------------------------
// Components
// ------------------------------------------------------------------------------------------------

void transform_add(EntityManager* entmgr, Entity e, Vector2 pos)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->transform_comps[e] = (TransformComponent){
        .pos =
            {
                .x = pos.x,
                .y = pos.y,
            },
    };

    SIGNATURE_SET(entmgr->signatures[e], COMP_TRANSFORM);
}

void transform_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_TRANSFORM);
}

void sprite_add(EntityManager* entmgr, Entity e, SDL_Texture* tex, Vector2 size, SDL_FRect src, bool is_fixed)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->sprite_comps[e] = (SpriteComponent){
        .texture = tex,
        .size = size,
        .src = src,
        .is_fixed = is_fixed,
    };

    SIGNATURE_SET(entmgr->signatures[e], COMP_SPRITE);
}

void sprite_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_SPRITE);
}

void animation_add(EntityManager* entmgr, Entity e, int num_frames, int frame_rate_speed, bool loop)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->animation_comps[e] = (AnimationComponent){
        .num_frames = num_frames,
        .frame_rate_speed = frame_rate_speed,
        .loop = loop,
    };

    SIGNATURE_SET(entmgr->signatures[e], COMP_ANIMATION);
}

void animation_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_ANIMATION);
}

void keyboard_control_add(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->keyboard_control_comps[e] = (KeyboardControlComponent){0};
    SIGNATURE_SET(entmgr->signatures[e], COMP_KEYBOARD_CONTROL);
}

void keyboard_control_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_KEYBOARD_CONTROL);
}

void mouse_control_add(EntityManager* entmgr, Entity e, MouseControlComponent m)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->mouse_control_comps[e] = m;
    SIGNATURE_SET(entmgr->signatures[e], COMP_MOUSE_CONTROL);
}

void mouse_control_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_MOUSE_CONTROL);
}

void box_collider_add(EntityManager* entmgr, Entity e, Vector2 size, Vector2 offset)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    BoxColliderComponent bc = (BoxColliderComponent){
        .size = size,
        .offset = offset,
        .colour = (SDL_Color){255, 0, 0, 255},
    };

    entmgr->box_collider_comps[e] = bc;
    SIGNATURE_SET(entmgr->signatures[e], COMP_BOX_COLLIDER);
}

void box_collider_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_BOX_COLLIDER);
}

void rigid_body_add(EntityManager* entmgr, Entity e, Vector2 v)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    RigidBodyComponent rb = (RigidBodyComponent){
        .vel = v,
    };

    entmgr->rigid_body_comps[e] = rb;
    SIGNATURE_SET(entmgr->signatures[e], COMP_RIGID_BODY);
}

void rigid_body_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_RIGID_BODY);
}
