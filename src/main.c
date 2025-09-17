#include "arena.h"
#include "game.h"
#include <SDL3/SDL.h>

int main(void)
{
    MemoryArena game_mem;
    arena_init(&game_mem, 1 * MB);

    if (!game_init(&game_mem)) {
        SDL_Log("Failed to start game");
        return EXIT_FAILURE;
    }

    if (!game_run(&game_mem)) {
        SDL_Log("Failed to start game");
        return EXIT_FAILURE;
    }

    game_destroy();

    return EXIT_SUCCESS;
}
