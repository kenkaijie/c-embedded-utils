#include <cemb/static_pool.h>

#include <assert.h>
#include <string.h>

static inline ErrorCode_t static_pool_validate_config(StaticPoolConfig_t const * config)
{
    assert(config->buffer);
    assert(config->allocation_stack);

    if (config->buffer_size < (config->object_size * config->object_count)) return ERR_INVALID_ARG;
    return ERR_NONE;
}

ErrorCode_t static_pool_as_i_pool_allocator(StaticPool_t * pool, IPoolAllocator_t * interface)
{
    assert(pool);
    assert(interface);

    interface->context = (void *) pool;
    interface->allocate = (i_pool_allocate_allocate_t)static_pool_allocate;
    interface->deallocate = (i_pool_allocate_deallocate_t)static_pool_deallocate;
    interface->get_available_count = (i_pool_allocate_get_available_count_t)static_pool_get_available_count;
    return ERR_NONE;
}

ErrorCode_t static_pool_init(StaticPool_t * pool, StaticPoolConfig_t const * config)
{
    assert(pool);
    assert(config);

    ErrorCode_t ret;
    ret = static_pool_validate_config(config);
    if (ret != ERR_NONE) return ret;

    pool->config = *config;

    // no need to check here, the arguments are checked in the validate config for static pool
    ptr_stack_init(&pool->free_stack, pool->config.allocation_stack, pool->config.object_count);
    for (size_t idx = 0; idx < pool->config.object_count; ++idx)
    {
        ptr_stack_push(&pool->free_stack, (void *)&pool->config.buffer[idx * pool->config.object_size]);
    }
    // initialise all the metadata and other fields
    return ERR_NONE;
}

void static_pool_deinit(StaticPool_t * pool)
{
    assert(pool);

    ptr_stack_deinit(&pool->free_stack);
}

ErrorCode_t static_pool_allocate(StaticPool_t * pool, void ** object_pointer)
{
    assert(pool);
    assert(object_pointer);

    ErrorCode_t ret;
    void * temp_token = NULL;

    ret = ptr_stack_pop(&pool->free_stack, &temp_token);

    if (ret == ERR_NONE)
    {
        // we found a spot and have allocated it
        *object_pointer = temp_token;
        return ERR_NONE;
    }
    return ERR_NO_MEM;
}

ErrorCode_t static_pool_deallocate(StaticPool_t * pool, void ** object_pointer)
{
    assert(pool);
    assert(object_pointer);

    if (*object_pointer == NULL) return ERR_OUT_OF_BOUNDS;

    // don't need to check, if the stack is full this must have been a bad value, just NULL the pointer and move on. User's responsibility for memory leakages.
    ptr_stack_push(&pool->free_stack, *object_pointer);
    *object_pointer = NULL;

    return ERR_NONE;
}

size_t static_pool_get_available_count(StaticPool_t * pool)
{
    assert(pool);
    // the stack item remaining represents the number of allocated items
    return pool->config.object_count - ptr_stack_get_remaining_count(&pool->free_stack);
}