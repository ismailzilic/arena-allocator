#ifndef VIRT_MEM_H
#define VIRT_MEM_H

#include "typedefs.h"

u32 virt_mem_get_page_size(void);
void* virt_mem_reserve(u64 size);
b32 virt_mem_commit(void *addr_ptr, u64 size);
b32 virt_mem_decommit(void *addr_ptr, u64 size);
b32 virt_mem_release(void *addr_ptr, u64 size);

#endif
