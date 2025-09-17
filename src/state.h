#ifndef STATE_H_
#define STATE_H_

// #include "entity.h"
// #include "system.h"
// #include "texture.h"
#include <SDL3/SDL.h>

typedef struct GameState {
    bool is_running;
    bool is_fullscreen;
    SDL_Window *window;
    SDL_Renderer *renderer;
    // Arena data
    // EntityManager* ent_man;
    // SystemManager* sys_man;
    // TextureManager* tex_man;
} GameState;

#endif // !GAME_H_
