#include "static_pool.h"

#include <string.h>

static inline error_t _validate_config(static_pool_config_t const * config)
{
    if ((config->buffer == NULL) || (config->allocation_stack == 0)) return ERR_NULL_POINTER;
    if (config->buffer_size < (config->object_size * config->object_count)) return ERR_INVALID_ARG;
    return ERR_NONE;
}

error_t static_pool_as_object_pool(static_pool_t * pool, object_pool_t * interface)
{
    if (interface == NULL) return ERR_NULL_POINTER;
    interface->context = (void *) pool;
    interface->allocate = (object_pool_allocate_t)static_pool_allocate;
    interface->deallocate = (object_pool_deallocate_t)static_pool_deallocate;
    interface->get_available_count = (object_pool_get_available_count_t)static_pool_get_available_count;
    return ERR_NONE;
}

error_t static_pool_init(static_pool_t * pool, static_pool_config_t const * config)
{
    error_t ret;
    if (config == NULL) return ERR_NULL_POINTER;
    ret = _validate_config(config);
    if (ret != ERR_NONE) return ret;

    pool->m_config = *config;

    // no need to check here, the arguments are checked in the validate config for static pool
    ptr_stack_init(&pool->m_free_stack, pool->m_config.allocation_stack, pool->m_config.object_count);
    for (size_t idx = 0; idx < pool->m_config.object_count; ++idx)
    {
        ptr_stack_push(&pool->m_free_stack, (void *)&pool->m_config.buffer[idx * pool->m_config.object_size]);
    }
    // initialise all the metadata and other fields
    return ERR_NONE;
}

void static_pool_deinit(static_pool_t * pool)
{
    ptr_stack_deinit(&pool->m_free_stack);
}

error_t static_pool_allocate(static_pool_t * pool, void ** token)
{
    error_t ret;
    void * temp_token = NULL;
    if (token == NULL) return ERR_NULL_POINTER;

    ret = ptr_stack_pop(&pool->m_free_stack, &temp_token);

    if (ret == ERR_NONE)
    {
        // we found a spot and have allocated it
        *token = temp_token;
        return ERR_NONE;
    }
    return ERR_NO_MEM;
}

error_t static_pool_deallocate(static_pool_t * pool, void ** object_pointer)
{
    if (object_pointer == NULL) return ERR_NULL_POINTER;
    if (*object_pointer == NULL) return ERR_OUT_OF_BOUNDS;

    // don't need to check, if the stack is full this must have been a bad value, just NULL the pointer and move on. User's responsibility for memory leakages.
    ptr_stack_push(&pool->m_free_stack, *object_pointer);
    *object_pointer = NULL;

    return ERR_NONE;
}

size_t static_pool_get_available_count(static_pool_t * pool)
{
    // the stack item remaining represents the number of allocated items
    return pool->m_config.object_count - ptr_stack_get_remaining_count(&pool->m_free_stack);
}