#ifndef __MEMORY_MANAGER_H
#define __MEMORY_MANAGER_H

#include "./Utils/List/List.h"
#include <stddef.h>
#include <stdbool.h>

#define CONFIG_BYTE_ALIGNMENT               8
#define CONFIG_RECORD_MINIMUM_FREE_REMAIN   1

typedef struct memory_block
{
    size_t size;
    slist_node_t node;
} memory_block_t;

typedef struct memory_manager
{
    memory_block_t start_block;
    memory_block_t * end_block;

    size_t free_remain;
#if (CONFIG_RECORD_MINIMUM_FREE_REMAIN != 0)
    size_t minimum_free_remain;
#endif
} memory_manager_t;

bool memory_manager_init(memory_manager_t * memory_manager, void * heap, size_t size);
void * memory_manager_alloc(memory_manager_t * memory_manager, size_t size);
void memory_manager_free(memory_manager_t * memory_manager, void * memory);
size_t memory_manager_get_free_remain(memory_manager_t * memory_manager);
#if (CONFIG_RECORD_MINIMUM_FREE_REMAIN != 0)
size_t memory_manager_get_minimum_free_remain(memory_manager_t * memory_manager);
#endif

#endif
