#ifndef ENTITY_H_
#define ENTITY_H_

#include "arena.h"
#include "component.h"
#include <SDL3/SDL.h>
#include <stdint.h>

#define MAX_ENTITIES 32

#define ENTITY_HAS(entity_sig, comp_mask) (((entity_sig) & (1U << comp_mask)) == (1U << comp_mask))

typedef uint32_t Signature;
typedef uint32_t Entity;

typedef struct EntityManager {
    uint32_t next_entity_id;
    TransformComponent transform_comps[MAX_ENTITIES];
    SpriteComponent sprite_comps[MAX_ENTITIES];
    AnimationComponent animation_comps[MAX_ENTITIES];
    KeyboardControlComponent keyboard_control_comps[MAX_ENTITIES];
    MouseControlComponent mouse_control_comps[MAX_ENTITIES];
    // TODO: 2d array?
    BoxColliderComponent box_collider_comps[MAX_ENTITIES];
    RigidBodyComponent rigid_body_comps[MAX_ENTITIES];
    CameraFollowComponent camera_follow_comps[MAX_ENTITIES];
    TileinfoComponent tileinfo_comps[MAX_ENTITIES];
    Signature signatures[MAX_ENTITIES];
    bool live_entities[MAX_ENTITIES];
} EntityManager;

EntityManager* entitymgr_init(MemoryArena* mem);
Entity entity_create(EntityManager* entmgr);
void entity_destroy(EntityManager* entmgr, Entity e);

#endif // !ENTITY_H_
