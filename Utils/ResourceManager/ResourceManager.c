#include "ResourceManager.h"
#include <string.h>
#include RESOURCE_MANAGER_MEMORY_INCLUDE

/**
 * @brief Search resource node by name
 * 
 * @param resource_manager The resource manager
 * @param name Name to search
 * @param resource_node Resource node if finded
 * @return true Found
 * @return false Not found
 */
static bool resource_node_search(resource_manager_t * resource_manager, char * name, resource_node_t ** resource_node)
{
    resource_node_t * index_node;

    index_node = GET_NODE_OWNER(list_get_next(&resource_manager->resource_list_head.node), resource_node_t, node);

    while (index_node->name != (char *)0)
    {
        if (strcmp(index_node->name, name) == 0)
        {
            *resource_node = index_node;
            return true;
        }

        index_node = GET_NODE_OWNER(list_get_next(&index_node->node), resource_node_t, node);
    }

    return false;
}

/**
 * @brief Initialize resource manager
 * 
 * @param resource_manager The resource manager
 */
void resource_manager_init(resource_manager_t * resource_manager)
{
    resource_manager->resource_list_head.name = (char *)0;
    resource_manager->resource_list_head.resource = (void *)0;
    list_init(&resource_manager->resource_list_head.node);
}

/**
 * @brief Add resource node to resource manager
 * 
 * @param resource_manager The resource manager
 * @param name Name of resource
 * @param resource Data of resource
 * @return true Succedded
 * @return false Failed
 */
bool resource_manager_add(resource_manager_t * resource_manager, char * name, void * resource)
{
    resource_node_t * resource_node;

    if (name == (char *)0)
    {
        return false;
    }

    if (resource_node_search(resource_manager, name, (resource_node_t **)0) == true)
    {
        return false;
    }

    resource_node = (resource_node_t *)RESOURCE_MANAGER_MEMORY_ALLOC(sizeof(resource_node_t));
    if (resource_node == (resource_node_t *)0)
    {
        return false;
    }

    resource_node->name = name;
    resource_node->resource = resource;
    list_add(&resource_manager->resource_list_head.node, &resource_node->node);

    return true;
}

/**
 * @brief Remove recource node from recource manager by name
 * 
 * @param resource_manager The resource manager
 * @param name Name of recource node
 * @return true Succedded
 * @return false Failed
 */
bool resource_manager_remove(resource_manager_t * resource_manager, char * name)
{
    resource_node_t * resource_node;

    if (name == (char *)0)
    {
        return false;
    }

    if (resource_node_search(resource_manager, name, &resource_node) == false)
    {
        return false;
    }

    list_del(&resource_node->node);
    RESOURCE_MANAGER_MEMORY_FREE(resource_node);

    return true;
}

/**
 * @brief Get recource data from recource manager by name
 * 
 * @param resource_manager The recource manager
 * @param name Name of resource node
 * @return void* Redource data if not NULL
 */
void * resource_manager_get(resource_manager_t * resource_manager, char * name)
{
    resource_node_t * resource_node;

    if (name == (char *)0)
    {
        return (void *)0;
    }

    if (resource_node_search(resource_manager, name, &resource_node) == false)
    {
        return (void *)0;
    }

    return resource_node->resource;
}
