#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../entity.h"
#include "../event_bus.h"

int update_collision_system(event_bus_t *event_bus, entity_t *entities, size_t num_entities);
bool check_aabb_collision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH);

#endif // COLLISION_SYSTEM_H

