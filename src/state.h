#ifndef STATE_H
#define STATE_H

#include "entity.h"

typedef unsigned int (*fn)(void);
typedef unsigned int (*fn_fl)(float);

typedef struct {
	fn init;
	fn_fl update;
	fn_fl draw;
	fn destroy;
} state_t;

#endif // STATE_H
