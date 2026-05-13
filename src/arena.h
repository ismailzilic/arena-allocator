#ifndef ARENA_H
#define ARENA_H

#include "typedefs.h"

typedef struct {
	u64 pos;
	u64 size;
} mem_arena;

#define ALIGN_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))

#define ARENA_BASE_POS (sizeof(mem_arena))
#define ARENA_ALIGN (sizeof(void *))

mem_arena *arena_create(u64 size);
void arena_destroy(mem_arena *arena);
void *arena_push(mem_arena *arena, u64 size);
void arena_pop(mem_arena *arena, u64 size);
void arena_pop_to(mem_arena *arena, u64 pos);
void arena_clear(mem_arena *arena);

#endif // ARENA_H
