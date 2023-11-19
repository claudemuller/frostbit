#ifndef COLLISION_SYSTEM_H_
#define COLLISION_SYSTEM_H_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../entity.h"
#include "../event_bus.h"

int update_collision_system(event_bus_t *event_bus, entity_t *entities, size_t num_entities);

#endif // COLLISION_SYSTEM_H_

