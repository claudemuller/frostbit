#ifndef ENTITY_H_
#define ENTITY_H_

#include "arena.h"
#include "utils/utils.h"
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
    COMP_ANIMATION = 2,
    COMP_KEYBOARD_CONTROL = 3,
    COMP_MOUSE_CONTROL = 4,
    COMP_BOX_COLLIDER = 5,
    COMP_RIGID_BODY = 6,
    COMP_CAMERA_FOLLOW = 7,
    COMP_COUNT = MAX_COMPONENTS,
};

typedef union {
    struct {
        f32 x;
        f32 y;
    };
    struct {
        f32 w;
        f32 h;
    };
} Vector2;

typedef struct {
    Vector2 pos;
    Vector2 scale;
    f32 rotation;
} TransformComponent;

typedef struct {
    SDL_Texture* texture;
    Vector2 size; // TODO: ??
    u8 z_index;
    bool is_fixed;
    SDL_FlipMode is_h_flipped;
    SDL_FlipMode is_v_flipped;
    SDL_FRect src;
} SpriteComponent;

typedef struct {
    u8 num_frames;
    u8 start_frame;
    u8 cur_frame;
    i16 frame_rate_speed;
    bool loop;
    // TODO: ??
    int start_time;
} AnimationComponent;

typedef struct {
    Vector2 up_vel;
    Vector2 down_vel;
    Vector2 left_vel;
    Vector2 right_vel;
} KeyboardControlComponent;

typedef struct {
    Vector2 up_vel;
    Vector2 down_vel;
    Vector2 left_vel;
    Vector2 right_vel;
} MouseControlComponent;

typedef struct {
    Vector2 size;
    Vector2 offset;
    SDL_Color colour;
} BoxColliderComponent;

typedef struct {
    Vector2 vel;
} RigidBodyComponent;

typedef struct {
    Vector2 s;
} CameraFollowComponent;

typedef struct EntityManager {
    uint32_t next_entity_id;
    TransformComponent transform_comps[MAX_ENTITIES];
    SpriteComponent sprite_comps[MAX_ENTITIES];
    AnimationComponent animation_comps[MAX_ENTITIES];
    KeyboardControlComponent keyboard_control_comps[MAX_ENTITIES];
    MouseControlComponent mouse_control_comps[MAX_ENTITIES];
    BoxColliderComponent box_collider_comps[MAX_ENTITIES];
    RigidBodyComponent rigid_body_comps[MAX_ENTITIES];
    CameraFollowComponent camera_follow_comps[MAX_ENTITIES];
    Signature signatures[MAX_ENTITIES];
    bool live_entities[MAX_ENTITIES];
} EntityManager;

EntityManager* entitymgr_init(MemoryArena* mem);

Entity entity_create(EntityManager* entmgr);
void entity_destroy(EntityManager* entmgr, Entity e);

void transform_add(EntityManager* entmgr, Entity e, Vector2 pos);
void transform_remove(EntityManager* entmgr, Entity e);
void camera_follow_add(EntityManager* entmgr, Entity e);
void camera_follow_remove(EntityManager* entmgr, Entity e);
void sprite_add(EntityManager* entmgr, Entity e, SDL_Texture* tex, Vector2 size, SDL_FRect src, bool is_fixed);
void sprite_remove(EntityManager* entmgr, Entity e);
void animation_add(EntityManager* entmgr, Entity e, u8 start_frame, int num_frames, int frame_rate_speed, bool loop);
void animation_remove(EntityManager* entmgr, Entity e);
void keyboard_control_add(EntityManager* entmgr, Entity e);
void keyboard_control_remove(EntityManager* entmgr, Entity e);
void mouse_control_add(EntityManager* entmgr, Entity e, MouseControlComponent m);
void mouse_control_remove(EntityManager* entmgr, Entity e);
void box_collider_add(EntityManager* entmgr, Entity e, Vector2 size, Vector2 offset);
void box_collider_remove(EntityManager* entmgr, Entity e);
void rigid_body_add(EntityManager* entmgr, Entity e, Vector2 v);
void rigid_body_remove(EntityManager* entmgr, Entity e);

#endif // !ENTITY_H_
