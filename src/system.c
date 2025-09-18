#include "system.h"
#include "entity.h"

void movement_sys_update(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx)
{
    float dt = *(float*)ctx;
    TransformComponent* t = &entmgr->transform_comps[e];
    RigidBodyComponent* rb = &entmgr->rigid_body_comps[e];

    if (!t || !rb) return;

    t->pos.x += rb->vel.x * dt;
    t->pos.y += rb->vel.y * dt;

    // TODO: check if in the map
}

void render_sys_render(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx)
{
    (void)ctx;
    TransformComponent* t = &entmgr->transform_comps[e];
    SpriteComponent* s = &entmgr->sprite_comps[e];

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

void render_collider_sys_render(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx)
{
    (void)ctx;
    TransformComponent* t = &entmgr->transform_comps[e];
    BoxColliderComponent* bc = &entmgr->box_collider_comps[e];

    if (!t || !bc) return;

    SDL_SetRenderDrawColor(renderer, bc->colour.r, bc->colour.g, bc->colour.b, bc->colour.a);
    SDL_RenderRect(renderer,
                   &(SDL_FRect){
                       .x = t->pos.x + bc->offset.x,
                       .y = t->pos.y + bc->offset.y,
                       .w = bc->size.x,
                       .h = bc->size.y,
                   });
}
