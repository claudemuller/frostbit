#include "system.h"
#include "entity.h"
#include "state.h"
#include "texture.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

void movement_sys_update(GameState* state, Entity e, void* ctx)
{
    float dt = *(float*)ctx;
    TransformComponent* t = &state->entmgr->transform_comps[e];
    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];

    if (!t || !rb) return;

    SDL_Log("%f", rb->vel.x);

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

    if (!kb) return;

    if (ev.type == SDL_EVENT_KEY_DOWN) {
        if (ev.key.key == SDLK_Q || ev.key.key == SDLK_ESCAPE) {
            state->is_running = false;
        }

        if (ev.key.key == SDLK_F11) {
            SDL_SetWindowFullscreen(state->window, state->is_fullscreen);
            state->is_fullscreen = !state->is_fullscreen;
        }
    }

    RigidBodyComponent* rb = &state->entmgr->rigid_body_comps[e];

    if (!rb) return;

    if (ev.type == SDL_EVENT_KEY_DOWN) {
        if (ev.key.key == SDLK_A) rb->vel.x = -1.5f;
        if (ev.key.key == SDLK_LEFT) rb->vel.x = -1.5f;

        if (ev.key.key == SDLK_D) rb->vel.x = 0.5f;
        if (ev.key.key == SDLK_RIGHT) rb->vel.x = 0.5f;

        if (ev.key.key == SDLK_W) rb->vel.y = -0.5f;
        if (ev.key.key == SDLK_UP) rb->vel.y = -0.5f;

        if (ev.key.key == SDLK_S) rb->vel.y = 0.5f;
        if (ev.key.key == SDLK_DOWN) rb->vel.y = 0.5f;
    }

    if (ev.type == SDL_EVENT_KEY_UP) {
        if (ev.key.key == SDLK_A || ev.key.key == SDLK_LEFT) rb->vel.x = 0.0;
        if (ev.key.key == SDLK_D || ev.key.key == SDLK_RIGHT) rb->vel.x = 0.0;
        if (ev.key.key == SDLK_W || ev.key.key == SDLK_UP) rb->vel.y = 0.0;
        if (ev.key.key == SDLK_S || ev.key.key == SDLK_DOWN) rb->vel.y = 1.0;
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
