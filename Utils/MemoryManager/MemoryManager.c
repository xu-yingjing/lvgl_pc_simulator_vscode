#include "MemoryManager.h"

#define BYTE_ALIGNMENT                                          (CONFIG_BYTE_ALIGNMENT)
#define BYTE_ALIGNMENT_MASK                                     ((size_t)(BYTE_ALIGNMENT) - 1)
#define IS_ALIGNMENT(x)                                         ((((size_t)(x) & BYTE_ALIGNMENT_MASK) == 0) ? true : false)
#define ALIGNMENT_DOWN(x)                                       ((size_t)(x) & ~BYTE_ALIGNMENT_MASK)
#define ALIGNMENT_UP(x)                                         ALIGNMENT_DOWN((size_t)(x) + BYTE_ALIGNMENT_MASK)

static const size_t memory_block_struct_size                    = ALIGNMENT_UP(sizeof(memory_block_t));
static const size_t memory_block_minimun_size                   = memory_block_struct_size << 1;
static const size_t memory_block_allocated_bit                  = (((size_t)1) << ((sizeof(size_t) * 8) - 1));

#define MEMORY_BLOCK_GET_NEXT(memory_block)                     GET_NODE_OWNER(slist_get_next(&memory_block->node), memory_block_t, node)
#define MEMORY_BLOCK_SET_SIZE(memory_block, memory_block_size)  ((memory_block)->size = ((memory_block)->size & memory_block_allocated_bit) | (memory_block_size))
#define MEMORY_BLOCK_GET_SIZE(memory_block)                     ((memory_block)->size & ~memory_block_allocated_bit)
#define MEMORY_BLOCK_SET_ALLOCATED(memory_block)                ((memory_block)->size |= memory_block_allocated_bit)
#define MEMORY_BLOCK_SET_UNALLOCATED(memory_block)              ((memory_block)->size &= ~memory_block_allocated_bit)
#define IS_MEMORY_BLOCK_ALLOCATED(memory_block)                 ((((memory_block)->size & memory_block_allocated_bit) != 0) ? true : false)

/**
 * @brief Add memory block to memory manager's memery block list
 * 
 * @param memory_manager The memory manager
 * @param memory_block Memory block to add
 */
static void memory_block_add(memory_manager_t * memory_manager, memory_block_t * memory_block_insert)
{
    memory_block_t * memory_block;

    memory_block = &memory_manager->start_block;
    while ((size_t)MEMORY_BLOCK_GET_NEXT(memory_block) < (size_t)memory_block_insert)
    {
        memory_block = MEMORY_BLOCK_GET_NEXT(memory_block);
    }

    if (((size_t)memory_block + MEMORY_BLOCK_GET_SIZE(memory_block)) == (size_t)memory_block_insert)
    {
        MEMORY_BLOCK_SET_SIZE(memory_block, MEMORY_BLOCK_GET_SIZE(memory_block) + MEMORY_BLOCK_GET_SIZE(memory_block_insert));
        memory_block_insert = memory_block;
    }

    if (((size_t)memory_block_insert + MEMORY_BLOCK_GET_SIZE(memory_block_insert)) == (size_t)MEMORY_BLOCK_GET_NEXT(memory_block))
    {
        if (MEMORY_BLOCK_GET_NEXT(memory_block) != memory_manager->end_block)
        {
            MEMORY_BLOCK_SET_SIZE(memory_block_insert, MEMORY_BLOCK_GET_SIZE(memory_block_insert) + MEMORY_BLOCK_GET_SIZE(MEMORY_BLOCK_GET_NEXT(memory_block)));
            slist_del(&memory_manager->start_block.node, &MEMORY_BLOCK_GET_NEXT(memory_block)->node);
        }
    }

    if (memory_block_insert != memory_block)
    {
        slist_add(&memory_block->node, &memory_block_insert->node);
    }
}

/**
 * @brief Initialize memory manager
 * 
 * @param memory_manager The memory manager
 * @param heap Point to heap memory
 * @param size Size of heap memory
 * @return true Succedded
 * @return false The memory manager was initialized
 */
bool memory_manager_init(memory_manager_t * memory_manager, void * heap, size_t size)
{
    size_t aligned_address;
    memory_block_t * first_memory_block;

    aligned_address = (size_t)heap;
    if (IS_ALIGNMENT(aligned_address) == false)
    {
        aligned_address = ALIGNMENT_UP(aligned_address);
        size -= (aligned_address - (size_t)heap);
    }
    
    first_memory_block = (memory_block_t *)aligned_address;
    memory_manager->end_block = (memory_block_t *)(ALIGNMENT_DOWN(aligned_address + size - memory_block_struct_size));

    slist_init(&memory_manager->start_block.node);
    slist_init(&memory_manager->end_block->node);

    slist_add(&memory_manager->start_block.node, &first_memory_block->node);
    slist_add(&first_memory_block->node, &memory_manager->end_block->node);

    MEMORY_BLOCK_SET_UNALLOCATED(&memory_manager->start_block);
    MEMORY_BLOCK_SET_UNALLOCATED(memory_manager->end_block);
    MEMORY_BLOCK_SET_UNALLOCATED(first_memory_block);

    MEMORY_BLOCK_SET_SIZE(&memory_manager->start_block, 0);
    MEMORY_BLOCK_SET_SIZE(memory_manager->end_block, 0);
    MEMORY_BLOCK_SET_SIZE(first_memory_block, (size_t)memory_manager->end_block - aligned_address);

    memory_manager->free_remain = MEMORY_BLOCK_GET_SIZE(first_memory_block);
#if (CONFIG_RECORD_MINIMUM_FREE_REMAIN != 0)
    memory_manager->minimum_free_remain = memory_manager->free_remain;
#endif

    return true;
}

/**
 * @brief Alloc memory
 * 
 * @param memory_manager The memory manager
 * @param size Size of memory to alloc
 * @return void* Allocated memory if it was not (void *)0
 */
void * memory_manager_alloc(memory_manager_t * memory_manager, size_t size)
{
#define IS_SIZE_WITHIN_RANGE(size)      (((size & memory_block_allocated_bit) == 0) ? true : false)
#define IS_SIZE_GREATER_THAN_ZERO(size) ((size > 0) ? true : false)
#define IS_SIZE_NOT_OVERFLOW(size)      (((size + memory_block_struct_size) > size) ? true : false)
#define IS_SIZE_LEGAL(size)             (IS_SIZE_WITHIN_RANGE(size) && IS_SIZE_GREATER_THAN_ZERO(size) && IS_SIZE_NOT_OVERFLOW(size))

    memory_block_t * memory_block_prev;
    memory_block_t * memory_block;
    memory_block_t * memory_block_new;
    void * memory;

    if (IS_SIZE_LEGAL(size) == false)
    {
        return (void *)0;
    }

    size += memory_block_struct_size;
    if (IS_ALIGNMENT(size) == false)
    {
        size = ALIGNMENT_UP(size);
    }

    if (IS_SIZE_LEGAL(size) == false)
    {
        return (void *)0;
    }

    memory_block_prev = &memory_manager->start_block;
    memory_block = MEMORY_BLOCK_GET_NEXT(memory_block_prev);

    while ((MEMORY_BLOCK_GET_SIZE(memory_block) < size) && (memory_block != memory_manager->end_block))
    {
        memory_block_prev = memory_block;
        memory_block = MEMORY_BLOCK_GET_NEXT(memory_block_prev);
    }

    if (memory_block == memory_manager->end_block)
    {
        return (void *)0;
    }

    memory = (void *)((size_t)memory_block + memory_block_struct_size);
    slist_del(&memory_manager->start_block.node, &memory_block->node);

    if ((MEMORY_BLOCK_GET_SIZE(memory_block) - size) > memory_block_minimun_size)
    {
        memory_block_new = (memory_block_t *)((size_t)memory_block + size);
        MEMORY_BLOCK_SET_UNALLOCATED(memory_block_new);
        MEMORY_BLOCK_SET_SIZE(memory_block_new, MEMORY_BLOCK_GET_SIZE(memory_block) - size);
        MEMORY_BLOCK_SET_SIZE(memory_block, size);
        memory_block_add(memory_manager, memory_block_new);
    }

    memory_manager->free_remain -= MEMORY_BLOCK_GET_SIZE(memory_block);
#if (CONFIG_RECORD_MINIMUM_FREE_REMAIN != 0)
    if (memory_manager->free_remain < memory_manager->minimum_free_remain)
    {
        memory_manager->minimum_free_remain = memory_manager->free_remain;
    }
#endif

    MEMORY_BLOCK_SET_ALLOCATED(memory_block);
    slist_init(&memory_block->node);

    return memory;
}

/**
 * @brief Free allocated memory
 * 
 * @param memory_manager The memory manager
 * @param memory Memory to free
 */
void memory_manager_free(memory_manager_t * memory_manager, void * memory)
{
    memory_block_t * memory_block;

    memory_block = (memory_block_t *)((size_t)memory - memory_block_struct_size);

    if (IS_MEMORY_BLOCK_ALLOCATED(memory_block) == false)
    {
        return;
    }

    if (slist_get_next(&memory_block->node) != (slist_node_t *)0)
    {
        return;
    }

    MEMORY_BLOCK_SET_UNALLOCATED(memory_block);
    memory_manager->free_remain += MEMORY_BLOCK_GET_SIZE(memory_block);
    memory_block_add(memory_manager, memory_block);
}

/**
 * @brief Get remain free heap size
 * 
 * @param memory_manager The memory manager
 * @return size_t Size of remain free heap
 */
size_t memory_manager_get_free_remain(memory_manager_t * memory_manager)
{
    return memory_manager->free_remain;
}

#if (CONFIG_RECORD_MINIMUM_FREE_REMAIN != 0)
/**
 * @brief Get minimum remain free heap size
 * 
 * @param memory_manager The memory manager
 * @return size_t Minimum size of remain free heap
 */
size_t memory_manager_get_minimum_free_remain(memory_manager_t * memory_manager)
{
    return memory_manager->minimum_free_remain;
}
#endif
