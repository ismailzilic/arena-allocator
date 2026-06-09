#include "arena.h"
#include "typedefs.h"

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	mem_arena *arena = arena_alloc(GiB(1), MiB(32));
	
	printf("press someting to allocate\n");
	getc(stdin);

	arena_push(arena, MiB(16), true);
	
	printf("press someting to deallocate\n");
	getc(stdin);

	arena_release(arena);

	printf("finished executing.\n");
	getc(stdin);

	return 0;
}
