#include "arena.h"

void arena_init(MemoryArena* arena, size_t cap)
{
    arena->base = (unsigned char*)malloc(cap);
    arena->cap = cap;
    arena->offset = 0;
}

static size_t align_forward(size_t ptr, size_t align)
{
    size_t misalignment = ptr & (align - 1);
    if (misalignment) ptr += (align - misalignment);
    return ptr;
}

void* arena_alloc_aligned(MemoryArena* arena, size_t size, size_t align)
{
    size_t aligned_offset = align_forward(arena->offset, align);
    if (aligned_offset + size > arena->cap) return NULL;
    void* ptr = arena->base + aligned_offset;
    arena->offset = aligned_offset + size;
    return ptr;
}

ArenaMarker arena_get_marker(MemoryArena* arena)
{
    return arena->offset;
}

void arena_set_marker(MemoryArena* arena, ArenaMarker marker)
{
    arena->offset = marker;
}

void arena_reset(MemoryArena* arena)
{
    arena->offset = 0;
}

void arena_free(MemoryArena* arena)
{
    free(arena->base);
    arena->cap = 0;
    arena->offset = 0;
}
