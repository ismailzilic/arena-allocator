#include "arena.h"

#include <stdlib.h>
#include <string.h>

mem_arena *arena_create(u64 size)
{
	mem_arena *arena = (mem_arena *)malloc(size);

	arena->size = size;
	arena->pos = ARENA_BASE_POS;

	return arena;
}

void arena_destroy(mem_arena *arena)
{
	memset(arena, 0, sizeof(mem_arena) * arena->size);
	free(arena);
}

void *arena_push(mem_arena *arena, u64 size)
{
	u64 aligned = ALIGN_POW2(arena->pos, ARENA_ALIGN);
	u64 new_pos = aligned + size;

	if (new_pos > arena->size || new_pos < 0) {
		abort();
	}

	arena->pos = new_pos;

	u8 *output = (u8 *)arena + aligned;

	return output;
}

void arena_pop(mem_arena *arena, u64 size)
{
	u64 last_pos = arena->pos - ARENA_BASE_POS;

	size = (size < last_pos) ? size : last_pos;
	arena->pos -= size;
}

void arena_pop_to(mem_arena *arena, u64 pos)
{
	u64 size = (pos < arena->pos) ? arena->pos - pos : 0;
	arena_pop(arena, size);
}

void arena_clear(mem_arena *arena)
{
	memset(arena, 0, sizeof(mem_arena) * arena->size);
	arena->pos = ARENA_BASE_POS;
	arena->size = 0;
}
