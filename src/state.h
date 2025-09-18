#ifndef STATE_H_
#define STATE_H_

#include "entity.h"
#include <SDL3/SDL.h>

struct EntityManager;
struct SystemManager;
struct TextureManager;

typedef struct GameState {
    bool is_running;
    bool is_fullscreen;

    SDL_Window* window;
    SDL_Renderer* renderer;

    // Arena data
    struct EntityManager* entmgr;
    struct SystemManager* sysmgr;
    struct TextureManager* texmgr;
} GameState;

extern GameState state;

#endif // !GAME_H_
