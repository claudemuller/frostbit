#ifndef ARENA_H_
#define ARENA_H_

#include <stddef.h>
#include <stdlib.h>

#define MB (1024 * 1024)

typedef struct {
    unsigned char* base; // The start of the arena
    size_t cap;
    size_t offset;
} MemoryArena;

typedef size_t ArenaMarker;

void arena_init(MemoryArena* arena, size_t cap);
void* arena_alloc_aligned(MemoryArena* arena, size_t size, size_t align);
ArenaMarker arena_get_marker(MemoryArena* arena);
void arena_set_marker(MemoryArena* arena, ArenaMarker marker);
void arena_reset(MemoryArena* arena);
void arena_free(MemoryArena* arena);

#endif // !ARENA_H_
