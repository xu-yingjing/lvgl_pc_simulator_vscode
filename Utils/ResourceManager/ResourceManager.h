#ifndef __RESOURCE_MANAGER_H
#define __RESOURCE_MANAGER_H

#include "./Utils/List/List.h"
#include <stdbool.h>

#define RESOURCE_MANAGER_MEMORY_INCLUDE "./App/Memory/Memory.h"
#define RESOURCE_MANAGER_MEMORY_ALLOC   memory_alloc
#define RESOURCE_MANAGER_MEMORY_FREE    memory_free
typedef struct resource_node
{
    char * name;
    void * resource;
    list_node_t node;
} resource_node_t;

typedef struct resource_manager
{
    resource_node_t resource_list_head;
} resource_manager_t;

void resource_manager_init(resource_manager_t * resource_manager);
bool resource_manager_add(resource_manager_t * resource_manager, char * name, void * resource);
bool resource_manager_remove(resource_manager_t * resource_manager, char * name);
void * resource_manager_get(resource_manager_t * resource_manager, char * name);

#endif
