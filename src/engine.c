#include <SDL2/SDL_image.h>
#include "engine.h"
#include "entity.h"
#include "state.h"
#include "vector.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000/FPS;

bool engine_init(engine_t *engine, struct engine_options *options)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to init SDL: %s", SDL_GetError());
		return false;
	}

	if (options != NULL) {
		engine->graphics.width = options->width;
		engine->graphics.height = options->height;
		engine->graphics.window_title = options->title;
	}

	if (!graphics_init(&engine->graphics)) 
		return false;

	state_manager_init(&engine->state_manager);
	// asset_store_init(&engine->asset_store);

	engine->running = true;

	return true;
}

void engine_setup(engine_t *engine)
{
	// asset_store_add_texture(&engine->asset_store, engine->graphics.renderer, "tilemap", "./assets/tilemaps/jungle.png");
	// asset_store_add_texture(&engine->asset_store, engine->graphics.renderer, "tank", "./assets/tank.png");

	// load_tilemap_data("./assets/tilemaps/jungle.map");

	// GArray *entities = g_array_new(false, false, sizeof(entity_t));
	entity_t *entities = (entity_t*)malloc(sizeof(entity_t)*MAX_ENTITIES);

	entity_t entity1;
	entity1.id = 1;
	add_component_transform(&entity1, 10, 10, (vec2_t){1, 1});
	add_component_boxcollider(&entity1, 32, 32, (vec2_t){1, 1}, (SDL_Color){255, 0, 0, 255});
	add_component_rigidbody(&entity1, (vec2_t){1, 1});
	add_component_sprite(&entity1, "tank", 32, 32, 0, 0, 1, false, SDL_FLIP_NONE);
	// g_array_append_val(entities, entity1);
	entities[0] = entity1;

	entity_t entity2;
	entity2.id = 2;
	add_component_transform(&entity2, 100, 100, (vec2_t){1, 1});
	add_component_boxcollider(&entity2, 30, 30, (vec2_t){1, 1}, (SDL_Color){255, 0, 0, 255});
	add_component_rigidbody(&entity2, (vec2_t){-10, -10});
	add_component_sprite(&entity2, "tank", 32, 32, 0, 0, 1, false, SDL_FLIP_NONE);
	// g_array_append_val(entities, entity2);
	entities[1] = entity2;

	state_t *state1 = state_new(entities, 2);
	state_manager_push(&engine->state_manager, state1);
}

void engine_run(engine_t *engine)
{
	engine_setup(engine);

	while (engine->running) {
		engine_process_input(engine);
		engine_update(engine);
		engine_render(engine);
	}
}

void engine_process_input(engine_t *engine)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			engine->running = false;
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				engine->running = false;
			}
		}

	}
}

void engine_update(engine_t *engine)
{
	int time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks() - engine->millisecs_previous_frame);
	if (time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME) SDL_Delay(time_to_wait);

	double dt = (SDL_GetTicks() - engine->millisecs_previous_frame) / 1000.0;
	engine->millisecs_previous_frame = SDL_GetTicks();

	// state_manager_update(&engine->state_manager, dt);
}

void engine_render(engine_t *engine)
{
	SDL_SetRenderDrawColor(engine->graphics.renderer, 21, 21, 21, 255);
	SDL_RenderClear(engine->graphics.renderer);

	// state_manager_render(&engine->state_manager, engine->graphics.renderer, &engine->asset_store);

	SDL_RenderPresent(engine->graphics.renderer);
}

int engine_clean(engine_t *engine)
{
	state_manager_free(&engine->state_manager);
	asset_store_free(&engine->asset_store);
	graphics_free(&engine->graphics);
	SDL_Quit();

	return 0;
}

void load_tilemap_data(char *filename)
{

}
