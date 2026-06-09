#ifndef ARENA_H
#define ARENA_H

#include "typedefs.h"

typedef struct {
	u64 res_size;
	u64 cmt_size;

	u64 pos;
	u64 cmt_pos;
} mem_arena;

#define ALIGN_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))
#define CLAMP_DOWN(x, y) ((x) < (y) ? (x) : (y))
#define CLAMP_UP(x, y) ((x) > (y) ? (x) : (y))

#define ARENA_BASE_POS (sizeof(mem_arena))
#define ARENA_ALIGN (sizeof(void *))

mem_arena *arena_alloc(u64 reserve_size, u64 commit_size);
void arena_release(mem_arena *arena);
void *arena_push(mem_arena *arena, u64 size, b32 zeroed);
void arena_pop(mem_arena *arena, u64 size);
void arena_pop_to(mem_arena *arena, u64 pos);
void arena_clear(mem_arena *arena);

#define ARENA_PUSH_STRUCT(arena, T) (T *)(arena_push((arena), sizeof(T)))
#define ARENA_PUSH_ARRAY(arena, T, size) (T *)(arena_push((arena), sizeof(T) * (size)))

#endif // ARENA_H
