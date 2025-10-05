#include "system.h"
#include "component.h"
#include "entity.h"
#include "state.h"
#include "utils/utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

/*
 * The update logic for the movement system
 *
 * @param GameState state - the game state
 * @param Entity e - the entity under scrutiny
 * @param SystemCtx* ctx - the context surrounding this entity
 * @return void
 *
 * NOTE: I guess we could just pass the SystemCtx as it is a wrapper?
 */
void movement_sys_update(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_TRANSFORM) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_BOX_COLLIDER) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_RIGID_BODY)) {
        return;
    }

    if (ctx->tag != CTX_MOVEMENT) return;

    TransformComponent* t = &state->entmgr->transform_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];
    BoxColliderComponent* bc = &state->entmgr->box_collider_comps[e];

    if (!t || !rb) return;

    float map_w = (float)(state->level->width * state->level->tile_width * state->scale);
    float map_h = (float)(state->level->height * state->level->tile_height * state->scale);

    f64 dt = ctx->movement.dt;

    t->pos.x += rb->vel.x * dt;
    t->pos.y += rb->vel.y * dt;

    if (rb) {
        t->pos.x = clamp_f(t->pos.x, -bc->offset.w, (map_w / state->scale) - bc->size.w - bc->offset.w);
        t->pos.y = clamp_f(t->pos.y, -bc->offset.h, (map_h / state->scale) - bc->size.h - bc->offset.h);
    }
}

void render_sys_render(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_TRANSFORM) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_SPRITE)) {
        return;
    }

    TransformComponent* t = &state->entmgr->transform_comps[e];
    SpriteComponent* s = &state->entmgr->sprite_comps[e];

    if (!t || !s) return;

    // Don't draw off screen entities
    if (t->pos.x < state->camera.x - s->size.w || t->pos.x > state->camera.x + state->camera.w ||
        t->pos.y < state->camera.y - s->size.h || t->pos.y > state->camera.y + state->camera.h) {
        return;
    }

    SDL_FRect dst = (SDL_FRect){
        .x = t->pos.x - (s->is_fixed ? 0 : state->camera.x), // * state->scale,
        .y = t->pos.y - (s->is_fixed ? 0 : state->camera.y), // * state->scale,
        .w = s->size.w,                                      // * state->scale,
        .h = s->size.h,                                      // * state->scale,
    };
    SDL_RenderTexture(state->renderer, s->texture, &s->src, &dst);
}

void camera_movement_sys_update(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_TRANSFORM) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_CAMERA_FOLLOW)) {
        return;
    }

    TransformComponent* t = &state->entmgr->transform_comps[e];
    CameraFollowComponent* cf = &state->entmgr->camera_follow_comps[e];

    if (!t || !cf) return;

    // 20 * 40
    float map_w = (float)(state->level->width * state->level->tile_width * state->scale);
    float map_h = (float)(state->level->height * state->level->tile_height * state->scale);

    int win_w, win_h;
    SDL_GetWindowSize(state->window, &win_w, &win_h);
    float vw = (float)win_w;
    float vh = (float)win_h;

    float cam_x = t->pos.x - vw / (state->scale * 2.0f);
    float cam_y = t->pos.y - vh / (state->scale * 2.0f);

    float max_x = (map_w - vw) / state->scale;
    float max_y = (map_h - vh) / state->scale;

    float clamped_x = (max_x > 0.0f) ? clamp_f(cam_x, 0.0f, max_x) : 0.0f;
    float clamped_y = (max_y > 0.0f) ? clamp_f(cam_y, 0.0f, max_y) : 0.0f;

    state->camera.x = clamped_x;
    state->camera.y = clamped_y;
    state->camera.w = vw;
    state->camera.h = vh;
}

void keyboard_control_sys_update(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_KEYBOARD_CONTROL) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_RIGID_BODY) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_SPRITE)) {
        return;
    }

    if (ctx->tag != CTX_EVENT) return;

    KeyboardControlComponent* kb = &state->entmgr->keyboard_control_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];
    SpriteComponent* s = &state->entmgr->sprite_comps[e];

    if (!kb || !rb || !s) return;

    SDL_Event ev = ctx->event.ev;

    if (ev.type == SDL_EVENT_KEY_DOWN) {
        if (ev.key.key == SDLK_A || ev.key.key == SDLK_LEFT) {
            rb->vel.x = -50.5f;
            s->is_h_flipped = true;
        }

        if (ev.key.key == SDLK_D || ev.key.key == SDLK_RIGHT) {
            rb->vel.x = 50.5f;
            s->is_h_flipped = false;
        }

        if (ev.key.key == SDLK_W || ev.key.key == SDLK_UP) {
            rb->vel.y = -50.5f;
            s->is_v_flipped = true;
        }

        if (ev.key.key == SDLK_S || ev.key.key == SDLK_DOWN) {
            rb->vel.y = 50.5f;
            s->is_v_flipped = false;
        }
    }

    if (ev.type == SDL_EVENT_KEY_UP) {
        if (ev.key.key == SDLK_A || ev.key.key == SDLK_LEFT) rb->vel.x = 0.0;
        if (ev.key.key == SDLK_D || ev.key.key == SDLK_RIGHT) rb->vel.x = 0.0;
        if (ev.key.key == SDLK_W || ev.key.key == SDLK_UP) rb->vel.y = 0.0;
        if (ev.key.key == SDLK_S || ev.key.key == SDLK_DOWN) rb->vel.y = 0.0;
    }
}

void mouse_control_sys_update(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_MOUSE_CONTROL)) {
        return;
    }

    if (ctx->tag != CTX_MOVEMENT) return;

    MouseControlComponent* m = &state->entmgr->mouse_control_comps[e];

    if (!m) return;

    SDL_Event ev = ctx->event.ev;

    if (ev.button.button == SDL_BUTTON_LEFT) {
        util_info("Left mouse button clicked [%d]", ev.button.clicks);
    }
}

void animation_sys_render(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_ANIMATION) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_SPRITE) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_RIGID_BODY)) {
        return;
    }

    AnimationComponent* a = &state->entmgr->animation_comps[e];
    SpriteComponent* s = &state->entmgr->sprite_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];

    if (!a || !s || !rb) return;

    if (rb->vel.x == 0.0f && rb->vel.y == 0.0f) return;

    a->cur_frame = (((SDL_GetTicks() - a->start_time) * a->frame_rate_speed / 1000) % a->num_frames) + a->start_frame;

    s->src.x = (i32)(a->cur_frame * s->size.w) % (i32)s->texture->w;
    // s->src.w = s->is_h_flipped ? -s->src.w : s->src.w;
    s->src.y = (i32)((a->cur_frame * s->size.w) / s->texture->w) + s->size.h;
    // s->src.h = s->is_v_flipped ? -s->src.h : s->src.h;
}

void render_collider_sys_render(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!state->debug) {
        return;
    }

    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_TRANSFORM) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_BOX_COLLIDER)) {
        return;
    }

    TransformComponent* t = &state->entmgr->transform_comps[e];
    BoxColliderComponent* bc = &state->entmgr->box_collider_comps[e];

    if (!t || !bc) return;

    float x = (t->pos.x + bc->offset.x) - state->camera.x; // * state->scale,
    float y = (t->pos.y + bc->offset.y) - state->camera.y; // * state->scale,

    SDL_SetRenderDrawColor(state->renderer, bc->colour.r, bc->colour.g, bc->colour.b, bc->colour.a);
    SDL_RenderRect(state->renderer,
                   &(SDL_FRect){
                       .x = x,
                       .y = y,
                       .w = bc->size.w,
                       .h = bc->size.h,
                   });
}

void collision_sys_update(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_TRANSFORM) ||
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_BOX_COLLIDER) ||
        // Only check collision against moving objects
        !ENTITY_HAS(state->entmgr->signatures[e], COMP_RIGID_BODY)) {
        return;
    }

    TransformComponent* t = &state->entmgr->transform_comps[e];
    BoxColliderComponent* bc = &state->entmgr->box_collider_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];

    if (!t || !bc || !rb) {
        return;
    }

    float e_bc_x = (t->pos.x + bc->offset.x) - state->camera.x; // * state->scale,
    float e_bc_y = (t->pos.y + bc->offset.y) - state->camera.y; // * state->scale,

    // TODO: add a terrain collisions render system
    for (size_t i = 0; i < state->n_terrain_collisions; ++i) {
        SDL_FRect ter_bc = state->terrain_collisions[i];

        float ter_bc_x = ter_bc.x - state->camera.x; // * state->scale,
        float ter_bc_y = ter_bc.y - state->camera.y; // * state->scale,

        // TODO: need to know what type of collision to do - here it's rect vs rect
        if (check_aabb_collision(e_bc_x, e_bc_y, bc->size.w, bc->size.h, ter_bc_x, ter_bc_y, ter_bc.w, ter_bc.h)) {
            util_info("collided with terrain");
        }
    }

    for (uint32_t i = 0; i < state->entmgr->next_entity_id - 1; ++i) {
        Entity other_e = i;

        if (other_e == e) continue;

        TransformComponent* other_t = &state->entmgr->transform_comps[other_e];
        BoxColliderComponent* other_bc = &state->entmgr->box_collider_comps[other_e];

        float other_e_bc_x = (other_t->pos.x + other_bc->offset.x) - state->camera.x; // * state->scale,
        float other_e_bc_y = (other_t->pos.y + other_bc->offset.y) - state->camera.y; // * state->scale,

        if (check_aabb_collision(e_bc_x,
                                 e_bc_y,
                                 bc->size.w,
                                 bc->size.h,
                                 other_e_bc_x,
                                 other_e_bc_y,
                                 other_bc->size.w,
                                 other_bc->size.h)) {
            // printf("%d - e.x:%f e.y:%f e.w:%f e.h:%f\n%d - oe.x:%f oe.y:%f oe.w:%f oe.h:%f\n\n",
            //        e,
            //        e_bc_x,
            //        e_bc_y,
            //        bc->size.w,
            //        bc->size.h,
            //        other_e,
            //        other_e_bc_x,
            //        other_e_bc_y,
            //        other_bc->size.w,
            //        other_bc->size.h);

            // TODO: temp
            t->pos = t->prev_pos;

            // state->eventbus->emit(state->eventbus, (Event){.type = EVT_DEAD, .entity_id = e});
        } else {
            // TODO: temp
            t->prev_pos = t->pos;
        }
    }
}

bool check_aabb_collision(f64 ax, f64 ay, f64 aw, f64 ah, f64 bx, f64 by, f64 bw, f64 bh)
{
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

void debug_sys_update(GameState* state, Entity e, SystemCtx* ctx)
{
    if (!state->debug) {
        return;
    }

    if (!ENTITY_HAS(state->entmgr->signatures[e], COMP_TILEINFO)) {
        return;
    }

    TileinfoComponent* ti = &state->entmgr->tileinfo_comps[e];

    if (!ti) return;

    BoxColliderComponent* bc = &state->entmgr->box_collider_comps[e];
    TransformComponent* t = &state->entmgr->transform_comps[e];

    bool has_collider = ENTITY_HAS(state->entmgr->signatures[e], COMP_BOX_COLLIDER);
    char entid[50];
    snprintf(entid, 30, "eid:%d col:%d", e, has_collider);

    if (t && bc) {
        float x = (t->pos.x + bc->offset.x) - state->camera.x; // * state->scale,
        float y = (t->pos.y + bc->offset.y) - state->camera.y; // * state->scale,

        // Draw debug text
        SDL_SetRenderDrawColor(state->renderer, bc->colour.r, bc->colour.g, bc->colour.b, bc->colour.a);
        SDL_RenderDebugText(state->renderer, x, y - 10, entid);
    }

    SpriteComponent* s = &state->entmgr->sprite_comps[e];

    if (s) {
        float x = t->pos.x - state->camera.x; // * state->scale,
        float y = t->pos.y - state->camera.y; // * state->scale,

        // Draw sprite indicator
        SDL_SetRenderDrawColor(state->renderer, 180, 180, 180, 255);
        SDL_RenderRect(state->renderer,
                       &(SDL_FRect){
                           .x = x,
                           .y = y,
                           .w = s->size.w,
                           .h = s->size.h,
                       });
    }
}
