#ifndef GAME_H_
#define GAME_H_

#include "arena.h"
#include <SDL3/SDL.h>

bool game_init(MemoryArena* game_mem);
bool game_run(MemoryArena* game_mem);
void game_destroy(MemoryArena* game_mem);

#endif // !GAME_H_
