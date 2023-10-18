#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "graphics.h"
#include "state_manager.h"

int main(void) {
	// Init options to be optional.
	struct engine_options options = {0};
	options.title = "Some Rando Game";
	options.width = 800;
	options.height = 600;

	engine_t engine;
	if (!engine_init(&engine, &options))
		return 1;

	engine_run(&engine);

	engine_clean(&engine);

	return 0;
}
