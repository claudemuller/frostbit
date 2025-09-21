#ifndef STATE_H_
#define STATE_H_

#include "entity.h"
#include "event_bus.h"
#include <SDL3/SDL.h>
#include <tmx.h>

struct EntityManager;
struct SystemManager;
struct TextureManager;
struct EventBus;

typedef struct GameState {
    // Memory arena allocated
    struct EntityManager* entmgr;
    struct SystemManager* sysmgr;
    struct TextureManager* texmgr;
    struct EventBus* eventbus;

    // Allocated externally
    tmx_map* level;
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool is_running;
    bool is_fullscreen;
} GameState;

extern GameState state;

#endif // !GAME_H_
