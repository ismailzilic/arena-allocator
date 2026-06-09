#include "arena.h"
#include "virt_mem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mem_arena *arena_alloc(u64 reserve_size, u64 commit_size)
{
	u64 page_size = virt_mem_get_page_size();

	reserve_size = ALIGN_POW2(reserve_size, page_size);
	commit_size = ALIGN_POW2(commit_size, page_size);

	mem_arena *arena = virt_mem_reserve(reserve_size);

	if (arena == NULL) {
		fprintf(stderr, "arena_alloc(): failed to reserve memory.\n");
		abort();
	}

	if (!virt_mem_commit(arena, commit_size)) {
		fprintf(stderr, "arena_alloc(): failed to commit to memory.\n");
		abort();
	}

	arena->res_size = reserve_size;
	arena->cmt_size = commit_size;
	arena->pos = ARENA_BASE_POS;
	arena->cmt_pos = commit_size;

	return arena;
}

void arena_release(mem_arena *arena)
{
	if (!virt_mem_release(arena, arena->res_size)) {
		fprintf(stderr, "arena_release(): failed to release memory.\n");
		abort();
	};
}

void *arena_push(mem_arena *arena, u64 size, b32 zeroed)
{
	u64 pos_pre = ALIGN_POW2(arena->pos, ARENA_ALIGN);
	u64 pos_new = pos_pre + size;

	if (pos_new > arena->res_size) {
		abort();
	}

	if (pos_new > arena->cmt_pos) {
		u64 cmt_new_aligned = pos_new + arena->cmt_size - 1;
		cmt_new_aligned -= cmt_new_aligned % arena->cmt_size;

		u64 cmt_new_clamped = CLAMP_DOWN(cmt_new_aligned, arena->res_size);

		u64 commit_size = cmt_new_clamped - arena->cmt_pos;
		u8 *cmt_ptr = (u8 *)arena + arena->cmt_pos;

		if (!virt_mem_commit(cmt_ptr, commit_size)) {
			abort();
		}

		arena->cmt_pos = cmt_new_aligned;
	}

	void *result = 0;

	if (arena->cmt_pos >= pos_pre) {
		result = (u8 *)arena + pos_pre;
		arena->pos = pos_new;
	}

	if (zeroed) {
		memset(result, 0, size);
	}

	return result;
}

void arena_pop(mem_arena *arena, u64 size)
{
	size = CLAMP_DOWN(size, arena->pos - ARENA_BASE_POS);
	arena->pos -= size;

	if (!virt_mem_decommit(&arena->pos, size)) {
		fprintf(stderr, "arena_pop(): failed to decommit memory.\n");
		abort();
	}
}

void arena_pop_to(mem_arena *arena, u64 pos)
{
	u64 size = (pos < arena->pos) ? arena->pos - pos : 0;
	arena_pop(arena, size);
}

void arena_clear(mem_arena *arena) { arena_pop_to(arena, ARENA_BASE_POS); }
