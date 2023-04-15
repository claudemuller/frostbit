#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <stdlib.h>
#include <glib-2.0/glib.h>

typedef enum {
	EVENT_DESTROY_ENTITY,
} event_type_t;

typedef struct {
	event_type_t type;
	// TODO: args
} event_t;

typedef void (*handler_fn)(void);

typedef struct {
	event_type_t type;
	handler_fn handler;
} handler_t;

typedef struct event_bus_t {
	GArray *listeners;
	size_t num_listeners;
	GArray *poll;
	size_t num_poll;

	void (*add_handler)(struct event_bus_t *self, handler_t handler);
	void (*on_event)(struct event_bus_t *self, event_type_t type, handler_fn handler_fn);
	void (*add_event)(struct event_bus_t *self, event_t ev);
	void (*emit)(struct event_bus_t *self, event_type_t type);
	void (*process_events)(struct event_bus_t *self);
	void (*destroy)(struct event_bus_t *self);
} event_bus_t;

event_bus_t* new_event_bus(void);
void event_bus_add_handler(event_bus_t *self, handler_t handler);
void event_bus_on_event(event_bus_t *self, event_type_t type, handler_fn handler_fn);
void event_bus_add_event(event_bus_t *self, event_t ev);
void event_bus_emit(event_bus_t *self, event_type_t type);
void event_bus_process_events(event_bus_t *self);
void event_bus_destroy(event_bus_t *self);

#endif // EVENT_BUS_H
