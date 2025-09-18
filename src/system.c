#include "system.h"

void movement_sys_update(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx)
{
    SDL_Log("movement update");

    float dt = *(float*)ctx;
    TransformComponent* t = &entmgr->transform_comps[e];
    t->pos.x = 100.0f;
}

void render_sys_update(EntityManager* entmgr, SDL_Renderer* renderer, Entity e, void* ctx)
{
    SDL_Log("render update");

    (void)ctx;
    TransformComponent* t = &entmgr->transform_comps[e];
    SpriteComponent* s = &entmgr->sprite_comps[e];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderRect(renderer,
                   &(SDL_FRect){
                       .x = t->pos.x,
                       .y = t->pos.y,
                       .w = s->size.x,
                       .h = s->size.y,
                   });
}
