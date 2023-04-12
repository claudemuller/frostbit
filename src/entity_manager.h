#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entity.h"

typedef struct {
	entity_t **entities;
	int cap;
	int top;
} entity_manager_t;

#endif // ENTITY_MANAGER_H
