#include "SDL.h"
#include "engine.h"
#include "gfx.h"
#include "state_manager.h"
#include <stdbool.h>
#include <stdio.h>

int main(void)
{
    struct engine_options options = {
        .title = "Frostbit",
        .width = 1280,
        .height = 1080
    };

    engine_t engine;
    if (!engine_init(&engine, &options)) {
        SDL_LogCritical(1, "Error initialising engine\n");
        return 1;
    }

    engine_run(&engine);

    engine_clean(&engine);

    return 0;
}
