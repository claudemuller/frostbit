#include "array.h"
#include "event_bus.h"
#include <stdbool.h>
#include <stdio.h>

event_bus_t *event_bus_new(void)
{
    event_bus_t *event_bus = malloc(sizeof(event_bus_t));
    if (!event_bus) {
        return NULL;
    }

    // handler_t *listeners = malloc(sizeof(handler_t) * 10);
    // event_t *poll = malloc(sizeof(event_t) * 10);

    event_bus->listeners = NULL; // listeners;
    event_bus->num_listeners = 0;
    event_bus->poll = NULL; // poll;
    event_bus->num_poll = 0;

    event_bus->add_handler = event_bus_add_handler;
    event_bus->on_event = event_bus_on_event;
    event_bus->add_event = event_bus_add_event;
    event_bus->emit = event_bus_emit;
    event_bus->process_events = event_bus_process_events;
    event_bus->destroy = event_bus_destroy;

    return event_bus;
}

void event_bus_add_handler(event_bus_t *self, handler_t handler)
{
    array_push(self->listeners, handler);
    // self->listeners[self->num_listeners] = handler;
    self->num_listeners++;
}

void event_bus_on_event(event_bus_t *self, event_type_t type, handler_fn handler_fn)
{
    handler_t handler = {
        .type = type,
        .handler = handler_fn,
    };
    self->add_handler(self, handler);
}

void event_bus_add_event(event_bus_t *self, event_t ev)
{
    array_push(self->poll, ev);
    self->poll[self->num_poll] = ev;
    self->num_poll++;
}

void event_bus_emit(event_bus_t *self, event_type_t type)
{
    event_t ev = {
        .type = type
    };
    self->add_event(self, ev);
}

void event_bus_process_events(event_bus_t *self)
{
    if (array_length(self->poll) == 0) {
        return;
    }

    for (size_t i = 0; i < 1; i++) {
        for (size_t j = 0; j < 1; j++) {
            event_t ev = self->poll[i];
            handler_t hn = self->listeners[i];
            if (ev.type == hn.type) {
                hn.handler();
            }
        }
    }
}

void event_bus_destroy(event_bus_t *self)
{
    free(self->listeners);
    free(self->poll);
}
