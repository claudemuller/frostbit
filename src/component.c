#include "component.h"
#include "entity.h"

void transform_add(EntityManager* entmgr, Entity e, Vector2 pos)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->transform_comps[e] = (TransformComponent){
        .pos =
            {
                .x = pos.x,
                .y = pos.y,
            },
        // TODO: temp
        .prev_pos =
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

void camera_follow_add(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_SET(entmgr->signatures[e], COMP_CAMERA_FOLLOW);
}

void camera_follow_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_CAMERA_FOLLOW);
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

void animation_add(EntityManager* entmgr, Entity e, u8 start_frame, int num_frames, int frame_rate_speed, bool loop)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    entmgr->animation_comps[e] = (AnimationComponent){
        .num_frames = num_frames,
        .cur_frame = start_frame,
        .start_frame = start_frame,
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

void tileinfo_add(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;

    TileinfoComponent ti = (TileinfoComponent){
        .entity_id = e,
    };

    entmgr->tileinfo_comps[e] = ti;
    SIGNATURE_SET(entmgr->signatures[e], COMP_TILEINFO);
}

void tileinfo_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_TILEINFO);
}
