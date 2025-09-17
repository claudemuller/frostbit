#include "entity.h"
#include "arena.h"
#include <assert.h>
#include <stdint.h>

EntityManager *entity_manager_new(MemoryArena *game_mem)
{
    EntityManager *ent_man = (EntityManager *)arena_alloc_aligned(game_mem, sizeof(EntityManager), 16);
    assert(ent_man && "Failed to allocate entity manager.");

    return ent_man;
}

Entity entity_create(EntityManager *ent_man)
{
    if (ent_man->next_entity_id >= MAX_ENTITIES) return UINT32_MAX;

    Entity e = ent_man->next_entity_id++;
    ent_man->signatures[e] = 0;
    ent_man->live_entities[e] = true;

    SDL_Log("entity created: %d", ent_man->next_entity_id - 1);
    SDL_Log("%d", ent_man->live_entities[0]);

    return e;
}

void entity_destroy(EntityManager *ent_man, Entity e)
{
    if (e >= MAX_ENTITIES || !ent_man->live_entities[e]) return;
    ent_man->signatures[e] = 0;
    ent_man->live_entities[e] = false;
}

// ------------------------------------------------------------------------------------------------

void transform_add(EntityManager *ent_man, Entity e, TransformComponent t)
{
    SDL_Log("transform added");

    if (e >= MAX_ENTITIES || !ent_man->live_entities[e]) return;
    ent_man->transform_comps[e] = t;
    SIGNATURE_SET(ent_man->signatures[e], COMP_TRANSFORM);
}

void transform_remove(EntityManager *ent_man, Entity e)
{
    if (e >= MAX_ENTITIES || !ent_man->live_entities[e]) return;
    SIGNATURE_CLEAR(ent_man->signatures[e], COMP_TRANSFORM);
}

void sprite_add(EntityManager *ent_man, Entity e, SpriteComponent t)
{
    SDL_Log("sprite added");

    if (e >= MAX_ENTITIES || !ent_man->live_entities[e]) return;
    ent_man->sprite_comps[e] = t;
    SIGNATURE_SET(ent_man->signatures[e], COMP_SPRITE);
}

void sprite_remove(EntityManager *ent_man, Entity e)
{
    if (e >= MAX_ENTITIES || ent_man->live_entities[e]) return;
    SIGNATURE_CLEAR(ent_man->signatures[e], COMP_SPRITE);
}
