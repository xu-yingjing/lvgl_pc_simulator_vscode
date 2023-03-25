#include "Memory.h"

static unsigned char memory_heap[CONFIG_MEMORY_HEAP_SIZE];
memory_manager_t memory_manager;

/**
 * @brief Initialize memory
 * 
 */
void memory_init(void)
{
    memory_manager_init(&memory_manager, memory_heap, CONFIG_MEMORY_HEAP_SIZE);
}

/**
 * @brief Alloc memory
 * 
 * @param size Size of memory
 * @return void* Allocated memory if it was not (void *)0
 */
void * memory_alloc(size_t size)
{
    return memory_manager_alloc(&memory_manager, size);
}

/**
 * @brief Free allocated memory
 * 
 * @param memory Memory to free
 */
void memory_free(void * memory)
{
    memory_manager_free(&memory_manager, memory);
}
