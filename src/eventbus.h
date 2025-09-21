#ifndef EVENT_BUS_H_
#define EVENT_BUS_H_

#include "utils/utils.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

#define MAX_EVT_EVENTS 100
#define MAX_EVT_HANDLERS 50

typedef enum { EVT_NONE, EVT_DEAD, EVT_DESTROY_ENTITY, EVT_PLAYER_MOVE } EventType;

typedef union {
    SDL_Event event;
    i32 i;
} EventArgs;

typedef struct {
    EventType type;
    EventArgs args;
} Event;

typedef void (*EventHandlerFn)(EventArgs);

typedef struct {
    EventType type;
    EventHandlerFn handler;
} EventHandler;

typedef struct EventBus {
    size_t handler_count;
    EventHandler handlers[MAX_EVT_HANDLERS];
    size_t poll_count;
    Event poll[MAX_EVT_EVENTS];

    void (*on_event)(struct EventBus* ebus, EventType type, EventHandlerFn handler_fn);
    void (*emit)(struct EventBus* ebus, EventType type, EventArgs args);
    void (*process_events)(struct EventBus* ebus);
    void (*destroy)(void);
} EventBus;

bool eventbus_init(EventBus* ebus);
void eventbus_on_event(EventBus* ebus, EventType type, EventHandlerFn handler_fn);
void eventbus_emit(EventBus* ebus, EventType type, EventArgs args);
void eventbus_process_events(EventBus* ebus);
void eventbus_destroy(void);

#endif // EVENT_BUS_H_
