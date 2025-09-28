#ifndef STATE_H_
#define STATE_H_

#include "entity.h"
#include "eventbus.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <tmx.h>

struct EntityManager;
struct SystemManager;
struct TextureManager;
struct EventBus;

typedef struct {
    Vector2 pos;
    Vector2 size;
    Vector2 src;
    SDL_Texture* texture;
} Tile;

typedef struct GameState {
    // Memory arena allocated
    struct EntityManager* entmgr;
    struct SystemManager* sysmgr;
    struct TextureManager* texmgr;
    struct EventBus* eventbus;
    Tile* terrain_tiles;

    // Allocated externally
    // TODO: do I want to store this?
    tmx_map* level;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_FRect camera;
    u32 n_terrain_tiles;
    Entity player;
    f32 scale;
    bool is_running;
    bool is_fullscreen;
} GameState;

extern GameState state;

#endif // !GAME_H_
