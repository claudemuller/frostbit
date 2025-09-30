#include "eventbus.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// static EventBus event_bus = {0};

bool eventbus_init(EventBus* ebus)
{
    // EventBus* event_bus = debug_malloc(sizeof(EventBus), __FILE__, __LINE__);
    // if (!event_bus) {
    //     return false;
    // }

    // event_bus->handlers = {};
    // ebus->poll = NULL;

    ebus->on_event = eventbus_on_event;
    ebus->emit = eventbus_emit;
    ebus->process_events = eventbus_process_events;
    ebus->destroy = eventbus_destroy;

    return true;
}

void eventbus_on_event(EventBus* ebus, EventType type, EventHandlerFn EventHandlerFn)
{
    EventHandler handler = {
        .type = type,
        .handler = EventHandlerFn,
    };
    // array_push(event_bus.handlers, handler);
    ebus->handlers[ebus->handler_count++] = handler;
}

void eventbus_emit(EventBus* ebus, Event ev)
{
    // array_push(event_bus.poll, ev);
    ebus->poll[ebus->poll_count++] = ev;
}

void eventbus_process_events(EventBus* ebus)
{
    // size_t poll_len = array_length(event_bus.poll);
    if (ebus->poll_count == 0) {
        return;
    }

    // size_t handlers_len = array_length(event_bus.handlers);

    for (size_t i = 0; i < ebus->poll_count; i++) {
        Event ev = ebus->poll[i];

        for (size_t j = 0; j < ebus->handler_count; j++) {
            EventHandler hn = ebus->handlers[j];
            if (ev.type == hn.type) {
                hn.handler(ev);

                // TODO: figure out how to remove items from array
                // ebus->poll[i] = (Event){
                //     .type = EVT_DEAD,
                // };
                ebus->poll_count--;
            }
        }
    }
}

void eventbus_destroy(void)
{
    // free(event_bus.handlers);
    // free(event_bus.poll);
}
