#include "system.h"
#include "entity.h"
#include "state.h"

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

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    // SDL_RenderRect(renderer,
    //                &(SDL_FRect){
    //                    .x = t->pos.x,
    //                    .y = t->pos.y,
    //                    .w = s->size.x,
    //                    .h = s->size.y,
    //                });
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

    if (ev.key.key == SDLK_Q || ev.key.key == SDLK_ESCAPE) {
        state->is_running = false;
    }

    if (ev.key.key == SDLK_F11) {
        SDL_SetWindowFullscreen(state->window, state->is_fullscreen);
        state->is_fullscreen = !state->is_fullscreen;
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
