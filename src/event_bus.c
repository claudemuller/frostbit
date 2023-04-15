#include "event_bus.h"

event_bus_t*
new_event_bus(void)
{
	event_bus_t *event_bus = malloc(sizeof(event_bus_t));
	// TODO: err stuff

	handler_t *listeners = malloc(sizeof(handler_t)*10);
	event_t *poll = malloc(sizeof(event_t)*10);

	event_bus->listeners = listeners;
	event_bus->num_listeners = 0;
	event_bus->poll = poll;
	event_bus->num_poll = 0;

	event_bus->add_handler = event_bus_add_handler;
	event_bus->on_event = event_bus_on_event;
	event_bus->add_event = event_bus_add_event;
	event_bus->emit = event_bus_emit;
	event_bus->process_events = event_bus_process_events;
	event_bus->destroy = event_bus_destroy;

	return event_bus;
}

void
event_bus_add_handler(event_bus_t *self, handler_t handler)
{
	self->listeners[0] = handler;
	self->num_listeners = 3;
	self->listeners[1] = handler;
	self->listeners[2] = handler;
}

void
event_bus_on_event(event_bus_t *self, event_type_t type, handler_fn handler_fn)
{
	handler_t handler = {
		.type = type,
		.handler = handler_fn,
	};
	self->add_handler(self, handler);
}

void
event_bus_add_event(event_bus_t *self, event_t ev)
{
	self->poll[0] = ev;
	self->num_poll = 1;
}

void
event_bus_emit(event_bus_t *self, event_type_t type)
{
	event_t ev = {
		.type = type
	};
	self->add_event(self, ev);
}

void
event_bus_process_events(event_bus_t *self)
{
	if (self->num_poll == 0)
		return;

	for (size_t i = 0; i < self->num_poll; i++)
		for (size_t j = 0; j < self->num_listeners; j++)
			if (self->poll[i].type == self->listeners[j].type)
				self->listeners[j].handler();
}

void
event_bus_destroy(event_bus_t *self)
{
	free(self->listeners);
	free(self->poll);
}
