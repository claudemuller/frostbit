#include "entity.h"
#include "arena.h"
#include <assert.h>
#include <stdint.h>

EntityManager* entitymgr_init(MemoryArena* mem)
{
    EntityManager* entmgr = (EntityManager*)arena_alloc_aligned(mem, sizeof(EntityManager), 16);
    assert(entmgr && "Failed to allocate entity manager.");
    // 0 is reserved for error handling
    entmgr->next_entity_id = 1;
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
