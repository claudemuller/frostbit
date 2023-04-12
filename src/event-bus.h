#ifndef EVENT_BUS_H
#define EVENT_BUS_H

typedef enum {
	EVENT_DESTROY,
} event_type_t;

typedef struct {
	event_type_t type;
	// TODO: args
} event_t;

typedef void (*handler_fn)(void*);

typedef struct {
	event_type_t type;
	handler_fn handler;
} handler_t;

handler_t *listeners;
event_t *poll;

void
add_handler_to_listeners(handler_t hn)
{
	listeners[0] = hn;
}

void
on_event(event_type_t type, handler_fn handler)
{
	handler_t hn = {
		.type = type,
		.handler = handler,
	};
	add_handler_to_listeners(hn);
}

void add_event_to_poll(event_t ev)
{
	poll[0] = ev;
}

void emit(event_type_t type)
{
	event_t ev = {
		.type = type
	};
	add_event_to_poll(ev);
}

void
process_events(void)
{
	// TODO: loop over events and run handler.
}

#endif // EVENT_BUS_H
