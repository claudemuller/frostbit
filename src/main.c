#include "arena.h"
#include "game.h"
#include "utils/utils.h"
#include <SDL3/SDL.h>

int main(void)
{
    util_info("starting game...");

    MemoryArena game_mem;
    arena_init(&game_mem, 1 * MB);

    if (!game_init(&game_mem)) {
        util_error("Failed to start game");
        return EXIT_FAILURE;
    }

    if (!game_run()) {
        util_error("Failed to start game");
        return EXIT_FAILURE;
    }

    game_destroy();

    return EXIT_SUCCESS;
}
