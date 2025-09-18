#ifndef ENTITY_H_
#define ENTITY_H_

#include "arena.h"
#include <SDL3/SDL.h>
#include <stdint.h>

#define MAX_ENTITIES 32
#define MAX_COMPONENTS 1024

#define SIGNATURE_SET(sig, comp) ((sig) |= (1U << (comp)))
#define SIGNATURE_CLEAR(sig, comp) ((sig) &= ~(1U << (comp)))
#define SIGNATURE_MATCH(entity_sig, system_mask) (((entity_sig) & (system_mask)) == (system_mask))

typedef uint32_t Signature;
typedef uint32_t Entity;

enum ComponentID {
    COMP_TRANSFORM = 0,
    COMP_SPRITE = 1,
    COMP_COUNT = MAX_COMPONENTS,
};

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    Vector2 pos;
    Vector2 scale;
    float rotation;
} TransformComponent;

typedef struct {
    char* asset_id;
    Vector2 size;
    int z_index;
    bool is_fixed;
    SDL_FlipMode is_flipped;
    SDL_Rect src;
} SpriteComponent;

typedef struct {
    int num_frames;
    int cur_frame;
    int frame_rate_speed;
    bool loop;
    int start_time;
} AnimationComponent;

typedef struct {
    Vector2 up_vel;
    Vector2 down_vel;
    Vector2 left_vel;
    Vector2 right_vel;
} KeyboardControlComponent;

typedef struct EntityManager {
    uint32_t next_entity_id;
    TransformComponent transform_comps[MAX_ENTITIES];
    SpriteComponent sprite_comps[MAX_ENTITIES];
    Signature signatures[MAX_ENTITIES];
    bool live_entities[MAX_ENTITIES];
} EntityManager;

EntityManager* entity_manager_new(MemoryArena* game_mem);
Entity entity_create(EntityManager* entmgr);
void entity_destroy(EntityManager* entmgr, Entity e);

void transform_add(EntityManager* entmgr, Entity e, TransformComponent t);
void transform_remove(EntityManager* entmgr, Entity e);
void sprite_add(EntityManager* entmgr, Entity e, SpriteComponent t);
void sprite_remove(EntityManager* entmgr, Entity e);

#endif // !ENTITY_H_
