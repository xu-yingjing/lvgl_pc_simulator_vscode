#ifndef __MEMORY_H
#define __MEMORY_H

#include "./Utils/MemoryManager/MemoryManager.h"

#define CONFIG_MEMORY_HEAP_SIZE (10 * 1024)

void memory_init(void);
void * memory_alloc(size_t size);
void memory_free(void * memory);

#endif
