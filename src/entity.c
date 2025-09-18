#include "entity.h"
#include <assert.h>
#include <stdint.h>

Entity entity_create(EntityManager* entmgr)
{
    if (entmgr->next_entity_id >= MAX_ENTITIES) return UINT32_MAX;

    Entity e = entmgr->next_entity_id++;
    entmgr->signatures[e] = 0;
    entmgr->live_entities[e] = true;

    SDL_Log("entity created: %d", entmgr->next_entity_id - 1);
    SDL_Log("%d", entmgr->live_entities[0]);

    return e;
}

void entity_destroy(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    entmgr->signatures[e] = 0;
    entmgr->live_entities[e] = false;
}

// ------------------------------------------------------------------------------------------------

void transform_add(EntityManager* entmgr, Entity e, TransformComponent t)
{
    SDL_Log("transform added");

    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    entmgr->transform_comps[e] = t;
    SIGNATURE_SET(entmgr->signatures[e], COMP_TRANSFORM);
}

void transform_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_TRANSFORM);
}

void sprite_add(EntityManager* entmgr, Entity e, SpriteComponent t)
{
    SDL_Log("sprite added");

    if (e >= MAX_ENTITIES || !entmgr->live_entities[e]) return;
    entmgr->sprite_comps[e] = t;
    SIGNATURE_SET(entmgr->signatures[e], COMP_SPRITE);
}

void sprite_remove(EntityManager* entmgr, Entity e)
{
    if (e >= MAX_ENTITIES || entmgr->live_entities[e]) return;
    SIGNATURE_CLEAR(entmgr->signatures[e], COMP_SPRITE);
}
