#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include <glib-2.0/glib.h>
#include "../entity.h"

int update_movement_system(GArray *entities, size_t num_entities, double dt);

#endif // MOVEMENT_SYSTEM_H
