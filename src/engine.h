#ifndef	ENGINE_H_
#define	ENGINE_H_

#include "entity.h"
#include "gfx.h"
#include "state_manager.h"
#include "asset_store.h"
#include <stdbool.h>

extern const int FPS;
extern const int MILLISECS_PER_FRAME;

struct engine_options {
	char *title;
	int width;
	int height;
	bool fullscreen;
};

typedef struct {
	graphics_t graphics;
	state_manager_t state_manager;
	asset_store_t asset_store;
	bool running;
} engine_t;

bool engine_init(engine_t *engine, struct engine_options *options);
void engine_setup(engine_t *engine);
void engine_run(engine_t *engine);
void engine_process_input(engine_t *engine);
void engine_update(engine_t *engine);
void engine_render(engine_t *engine);
int engine_clean(engine_t *engine);
bool load_tilemap_data(const char *filename, state_t *state);

#endif // ENGINE_H_
