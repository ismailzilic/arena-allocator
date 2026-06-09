#if defined(_WIN32)

#include "virt_mem.h"
#include <windows.h>

u32 virt_mem_get_page_size(void)
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return sysinfo.dwPageSize;
}

void *virt_mem_reserve(u64 size) { return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE); }

b32 virt_mem_commit(void *addr_ptr, u64 size)
{
	void *ret = VirtualAlloc(addr_ptr, size, MEM_COMMIT, PAGE_READWRITE);
	return ret != NULL;
}

b32 virt_mem_decommit(void *addr_ptr, u64 size) { return VirtualFree(addr_ptr, size, MEM_DECOMMIT); }

b32 virt_mem_release(void *addr_ptr, u64 size) { return VirtualFree(addr_ptr, size, MEM_RELEASE); }

#elif defined(__linux__)

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "virt_mem.h"
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>

u32 virt_mem_get_page_size(void) { return sysconf(_SC_PAGESIZE); }

void *virt_mem_reserve(u64 size)
{
	void *addr = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (addr == MAP_FAILED)
		return NULL;

	return addr;
}

b32 virt_mem_commit(void *addr_ptr, u64 size)
{
	i32 ret = mprotect(addr_ptr, size, PROT_READ | PROT_WRITE);
	return ret == 0;
}

b32 virt_mem_decommit(void *addr_ptr, u64 size)
{
	i32 ret = mprotect(addr_ptr, size, PROT_NONE);

	if (ret < 0) {
		return false;
	}

	ret = madvise(addr_ptr, size, MADV_DONTNEED);
	return ret == 0;
}

b32 virt_mem_release(void *addr_ptr, u64 size)
{
	i32 ret = munmap(addr_ptr, size);
	return ret == 0;
}

#endif
