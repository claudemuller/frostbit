#include "event_bus.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static EventBus event_bus = {0};

bool event_bus_init(void)
{
    EventBus* event_bus = malloc(sizeof(EventBus));
    if (!event_bus) {
        return false;
    }

    event_bus->handlers = NULL;
    event_bus->poll = NULL;

    event_bus->on_event = event_bus_on_event;
    event_bus->emit = event_bus_emit;
    event_bus->process_events = event_bus_process_events;
    event_bus->destroy = event_bus_destroy;

    return true;
}

void event_bus_on_event(EventType type, EventHandlerFn EventHandlerFn)
{
    EventHandler handler = {
        .type = type,
        .handler = EventHandlerFn,
    };
    array_push(event_bus.handlers, handler);
}

void event_bus_emit(EventType type, EventArgs args)
{
    Event ev = {
        .type = type,
        .args = args,
    };
    array_push(event_bus.poll, ev);
}

void event_bus_process_events(void)
{
    size_t poll_len = array_length(event_bus.poll);
    if (poll_len == 0) {
        return;
    }

    size_t handlers_len = array_length(event_bus.handlers);

    for (size_t i = 0; i < poll_len; i++) {
        Event ev = event_bus.poll[i];

        for (size_t j = 0; j < handlers_len; j++) {
            EventHandler hn = event_bus.handlers[j];
            if (ev.type == hn.type) {
                hn.handler(ev.args);

                // TODO: figure out how to remove items from array
                event_bus.poll[i] = (Event){
                    .type = EVT_DEAD,
                };
            }
        }
    }
}

void event_bus_destroy(void)
{
    free(event_bus.handlers);
    free(event_bus.poll);
}
