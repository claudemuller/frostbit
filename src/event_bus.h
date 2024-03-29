#ifndef EVENT_BUS_H_
#define EVENT_BUS_H_

#include "vector.h"
#include <stdbool.h>
#include "SDL.h"

typedef enum {
	EVT_DEAD,
	EVT_DESTROY_ENTITY,
	EVT_PLAYER_MOVE
} event_type_t;

typedef union {
	SDL_Event event;
	int i;
} args_t;

typedef struct {
	event_type_t type;
	args_t args;
} event_t;

typedef void (*handler_fn)(args_t);

typedef struct {
	event_type_t type;
	handler_fn handler;
} handler_t;

typedef struct event_bus_t {
	handler_t *handlers;
	event_t *poll;

	void (*on_event)(event_type_t type, handler_fn handler_fn);
	void (*emit)(event_type_t type, args_t args);
	void (*process_events)(void);
	void (*destroy)(void);
} event_bus_t;

bool event_bus_init(void);
void event_bus_on_event(event_type_t type, handler_fn handler_fn);
void event_bus_emit(event_type_t type, args_t args);
void event_bus_process_events(void);
void event_bus_destroy(void);

#endif // EVENT_BUS_H_
