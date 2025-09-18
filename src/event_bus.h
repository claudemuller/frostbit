#ifndef EVENT_BUS_H_
#define EVENT_BUS_H_

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef enum { EVT_DEAD, EVT_DESTROY_ENTITY, EVT_PLAYER_MOVE } EventType;

typedef union {
    SDL_Event event;
    int i;
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

typedef struct event_bus_t {
    EventHandler* handlers;
    Event* poll;

    void (*on_event)(EventType type, EventHandlerFn handler_fn);
    void (*emit)(EventType type, EventArgs args);
    void (*process_events)(void);
    void (*destroy)(void);
} EventBus;

bool event_bus_init(void);
void event_bus_on_event(EventType type, EventHandlerFn handler_fn);
void event_bus_emit(EventType type, EventArgs args);
void event_bus_process_events(void);
void event_bus_destroy(void);

#endif // EVENT_BUS_H_
