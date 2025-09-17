#ifndef GAME_H_
#define GAME_H_

#include "arena.h"
#include <SDL3/SDL.h>

bool game_init(MemoryArena*);
bool game_run(MemoryArena*);
void game_destroy(void);

#endif // !GAME_H_
