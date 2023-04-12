#ifndef	ENGINE_H
#define	ENGINE_H

#include <stdbool.h>
#include "graphics.h"
#include "state_manager.h"

extern const int FPS;
extern const int MILLISECS_PER_FRAME;

struct engine_options {
	char *title;
	int width;
	int height;
};

typedef struct {
	bool running;
	int millisecs_previous_frame;
	graphics_t graphics;
	state_manager_t state_manager;
} engine_t;

bool engine_init(engine_t *engine, struct engine_options *options);
void engine_setup(engine_t *engine);
void engine_run(engine_t *engine);
void engine_process_input(engine_t *engine);
void engine_update(engine_t *engine, entity_t *ent);
void engine_render(engine_t *engine, entity_t *ent);
int engine_clean(engine_t *engine);

#endif // ENGINE_H
