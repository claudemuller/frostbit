#include "system.h"
#include "entity.h"
#include "state.h"
#include "texture.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

void movement_sys_update(GameState* state, Entity e, void* ctx)
{
    float dt = *(float*)ctx;
    TransformComponent* t = &state->entmgr->transform_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];

    if (!t || !rb) return;

    t->pos.x += rb->vel.x * dt;
    t->pos.y += rb->vel.y * dt;

    // TODO: check if in the map
}

void render_sys_render(GameState* state, Entity e, void* ctx)
{
    (void)ctx;
    TransformComponent* t = &state->entmgr->transform_comps[e];
    SpriteComponent* s = &state->entmgr->sprite_comps[e];

    if (!t || !s) return;

    SDL_FRect dst = (SDL_FRect){
        .x = t->pos.x,
        .y = t->pos.y,
        .w = s->size.x,
        .h = s->size.y,
    };
    SDL_Texture* tex = texmgr_get_texture(state->texmgr, s->asset_id);
    SDL_RenderTexture(state->renderer, tex, &s->src, &dst);
}

void render_collider_sys_render(GameState* state, Entity e, void* ctx)
{
    (void)ctx;
    TransformComponent* t = &state->entmgr->transform_comps[e];
    BoxColliderComponent* bc = &state->entmgr->box_collider_comps[e];

    if (!t || !bc) return;

    SDL_SetRenderDrawColor(state->renderer, bc->colour.r, bc->colour.g, bc->colour.b, bc->colour.a);
    SDL_RenderRect(state->renderer,
                   &(SDL_FRect){
                       .x = t->pos.x + bc->offset.x,
                       .y = t->pos.y + bc->offset.y,
                       .w = bc->size.x,
                       .h = bc->size.y,
                   });
}

void keyboard_control_sys_update(GameState* state, Entity e, void* ctx)
{
    SDL_Event ev = *(SDL_Event*)ctx;
    KeyboardControlComponent* kb = &state->entmgr->keyboard_control_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];

    if (!kb || !rb) return;

    if (ev.type == SDL_EVENT_KEY_DOWN) {
        if (ev.key.key == SDLK_A) rb->vel.x = -1.5f;
        if (ev.key.key == SDLK_LEFT) rb->vel.x = -1.5f;

        if (ev.key.key == SDLK_D) rb->vel.x = 1.5f;
        if (ev.key.key == SDLK_RIGHT) rb->vel.x = 1.5f;

        if (ev.key.key == SDLK_W) rb->vel.y = -1.5f;
        if (ev.key.key == SDLK_UP) rb->vel.y = -1.5f;

        if (ev.key.key == SDLK_S) rb->vel.y = 1.5f;
        if (ev.key.key == SDLK_DOWN) rb->vel.y = 1.5f;
    }

    if (ev.type == SDL_EVENT_KEY_UP) {
        if (ev.key.key == SDLK_A || ev.key.key == SDLK_LEFT) rb->vel.x = 0.0;
        if (ev.key.key == SDLK_D || ev.key.key == SDLK_RIGHT) rb->vel.x = 0.0;
        if (ev.key.key == SDLK_W || ev.key.key == SDLK_UP) rb->vel.y = 0.0;
        if (ev.key.key == SDLK_S || ev.key.key == SDLK_DOWN) rb->vel.y = 0.0;
    }
}

void mouse_control_sys_update(GameState* state, Entity e, void* ctx)
{
    SDL_Event ev = *(SDL_Event*)ctx;
    MouseControlComponent* m = &state->entmgr->mouse_control_comps[e];

    if (!m) return;

    if (ev.button.button == SDL_BUTTON_LEFT) {
        SDL_Log("Left mouse button clicked [%d]", ev.button.clicks);
    }
}

void animation_sys_update(GameState* state, Entity e, void* ctx)
{
    (void)ctx;
    AnimationComponent* a = &state->entmgr->animation_comps[e];
    SpriteComponent* s = &state->entmgr->sprite_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];

    if (!a || !s || !rb) return;

    if (rb->vel.x == 0.0f && rb->vel.y == 0.0f) return;

    a->cur_frame = ((SDL_GetTicks() - a->start_time) * a->frame_rate_speed / 1000) % a->num_frames;

    if (rb->vel.y < 0) s->src.y = 0.0f;
    if (rb->vel.y > 0) s->src.y = s->size.y * 2.0f;
    if (rb->vel.x < 0) s->src.y = s->size.y;
    if (rb->vel.x > 0) s->src.y = s->size.y * 3.0f;

    s->src.x = a->cur_frame * s->size.x;
}
