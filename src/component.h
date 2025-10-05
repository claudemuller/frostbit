#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "utils/utils.h"
#include <SDL3/SDL.h>
#include <stdint.h>

#define MAX_COMPONENTS 1024

#define SIGNATURE_SET(sig, comp) ((sig) |= (1U << (comp)))
#define SIGNATURE_CLEAR(sig, comp) ((sig) &= ~(1U << (comp)))
#define SIGNATURE_MATCH(entity_sig, system_mask) (((entity_sig) & (system_mask)) == (system_mask))

typedef uint32_t Entity;
typedef struct EntityManager EntityManager;

typedef union Vector2 {
    struct {
        f32 x;
        f32 y;
    };
    struct {
        f32 w;
        f32 h;
    };
} Vector2;

enum ComponentID {
    COMP_NONE = 0,
    COMP_TRANSFORM,
    COMP_SPRITE,
    COMP_ANIMATION,
    COMP_KEYBOARD_CONTROL,
    COMP_MOUSE_CONTROL,
    COMP_BOX_COLLIDER,
    COMP_RIGID_BODY,
    COMP_CAMERA_FOLLOW,
    COMP_TILEINFO,
    COMP_COUNT = MAX_COMPONENTS,
};

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

typedef enum {
    COLLIDER_TYPE_GENERIC = 0,
    COLLIDER_TYPE_Z,
} ColliderType;

typedef struct {
    Vector2 size;
    Vector2 offset;
    SDL_Color colour;
    ColliderType type;
} BoxColliderComponent;

typedef struct {
    Vector2 vel;
    Vector2 next_pos;
} RigidBodyComponent;

typedef struct {
    Entity entity_id;
} TileinfoComponent;

typedef struct {
    Vector2 s;
} CameraFollowComponent;

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
void box_collider_add(EntityManager* entmgr, Entity e, Vector2 size, Vector2 offset, ColliderType type);
void box_collider_remove(EntityManager* entmgr, Entity e);
void rigid_body_add(EntityManager* entmgr, Entity e, Vector2 v, Vector2 np);
void rigid_body_remove(EntityManager* entmgr, Entity e);
void tileinfo_add(EntityManager* entmgr, Entity e);
void tileinfo_remove(EntityManager* entmgr, Entity e);

#endif // !COMPONENT_H_
