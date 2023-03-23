#ifndef __LIST_H
#define __LIST_H

typedef struct list_node
{
    struct list_node * prev;
    struct list_node * next;
} list_node_t;

/* For 32-bits host */
// #define GET_NODE_OWNER(node, owner_type, owner_member)  ((owner_type *)((char *)(node) - (unsigned long)(&((owner_type *)0)->owner_member)))
/* For 64-bits host */
#define GET_NODE_OWNER(node, owner_type, owner_member)  ((owner_type *)((char *)(node) - (unsigned long long)(&((owner_type *)0)->owner_member)))

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

#endif
