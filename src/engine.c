#include "engine.h"
#include "vector.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000/FPS;

unsigned int
init_state1(void)
{
	printf("init state\n");
	return 0;
}
unsigned int
destroy_state1(void)
{
	printf("destroy state\n");
	return 0;
}
unsigned int
update_state1(float dt)
{
	printf("update state:%f\n", dt);
	return 0;
}

bool
engine_init(engine_t *engine, struct engine_options *options)
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

	engine->running = true;
	return true;
}

const int ENTS_LEN = 3;
entity_t ents[ENTS_LEN];
state_t state1;

void
engine_setup(engine_t *engine)
{
	state1.init = init_state1;
	state1.update = update_state1;
	state1.destroy = destroy_state1;

	state_manager_push(&engine->state_manager, &state1);

	add_component_transform(&ents[0], 10, 10, 30, 30);
	add_component_rigidbody(&ents[0], (vec2_t){10, 10});

	add_component_transform(&ents[1], 100, 100, 40, 30);
	add_component_rigidbody(&ents[1], (vec2_t){-10, -10});
}

void
engine_run(engine_t *engine)
{
	engine_setup(engine);

	while (engine->running) {
		engine_process_input(engine);
		engine_update(engine, ents);
		engine_render(engine, ents);
	}
}

void
engine_process_input(engine_t *engine)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			engine->running = false;
	}
}

void
engine_update(engine_t *engine, entity_t *ents)
{
	int time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks() - engine->millisecs_previous_frame);
    if (time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME) SDL_Delay(time_to_wait);

    double dt = (SDL_GetTicks() - engine->millisecs_previous_frame) / 1000.0;
    engine->millisecs_previous_frame = SDL_GetTicks();

	// state_manager_update(&engine->state_manager, 10.0f);
	for (size_t i = 0; i < ENTS_LEN; i++) {
		update_transform_system(&ents[i], dt);
		// update_collision_system(&ents[i], dt);
	}
}

void
engine_render(engine_t *engine, entity_t *ents)
{
	SDL_SetRenderDrawColor(engine->graphics.renderer, 21, 21, 21, 255);
    SDL_RenderClear(engine->graphics.renderer);

	// state_manager_render(&engine->state_manager, 10.0f);
	for (size_t i = 0; i < ENTS_LEN; i++)
		update_render_system(engine->graphics.renderer, &ents[i]);

    SDL_RenderPresent(engine->graphics.renderer);
}

int
engine_clean(engine_t *engine)
{
	state_manager_free(&engine->state_manager);
	graphics_free(&engine->graphics);
	SDL_Quit();
	return 0;
}
