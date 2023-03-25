#ifndef __LIST_H
#define __LIST_H

#include <stddef.h>

typedef struct list_node
{
    struct list_node * prev;
    struct list_node * next;
} list_node_t;

#define GET_NODE_OWNER(node, owner_type, owner_member)  ((owner_type *)((size_t)(node) - (size_t)(&((owner_type *)0)->owner_member)))

/**
 * @brief Initialize list
 * 
 * @param head Head node of list
 */
static inline void list_init(list_node_t * head)
{
    head->prev = head;
    head->next = head;
}

/**
 * @brief Insert node after specified node
 * 
 * @param node Specified node
 * @param entry Node to insert
 */
static inline void list_add(list_node_t * node, list_node_t * entry)
{
    node->next->prev = entry;
    entry->next = node->next;
    node->next = entry;
    entry->prev = node;
}

/**
 * @brief Remove node from it's list
 * 
 * @param entry Node to remove
 */
static inline void list_del(list_node_t * entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

/**
 * @brief Get next node
 * 
 * @param entry Specified node
 * @return list_node_t* The next node
 */
static inline list_node_t * list_get_next(list_node_t * entry)
{
    return entry->next;
}

typedef struct slist_node
{
    struct slist_node * next;
} slist_node_t;

/**
 * @brief Initialize single list
 * 
 * @param head Head node of single list
 */
static inline void slist_init(slist_node_t * head)
{
    head->next = (slist_node_t *)0;
}

/**
 * @brief Insert node after specified node
 * 
 * @param node Specified node
 * @param entry Node to insert
 */
static inline void slist_add(slist_node_t * node, slist_node_t * entry)
{
    entry->next = node->next;
    node->next = entry;
}

/**
 * @brief Remove node from specified single list
 * 
 * @param head Head of specified single list
 * @param entry Node to remove
 */
static inline void slist_del(slist_node_t * head, slist_node_t * entry)
{
    slist_node_t * index = head;

    while ((index->next != (slist_node_t *)0) && (index->next != entry))
    {
        index = index->next;
    }

    if (index->next != (slist_node_t *)0)
    {
        index->next = index->next->next;
    }
}

/**
 * @brief Get next node
 * 
 * @param entry Specified node
 * @return slist_node_t The next node
 */
static inline slist_node_t * slist_get_next(slist_node_t * entry)
{
    return entry->next;
}

#endif
