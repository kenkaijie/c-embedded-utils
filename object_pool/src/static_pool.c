#include "static_pool.h"

#include <string.h>

static inline void _lock_if_needed(static_pool_t * pool)
{
    if (pool->m_config.use_lock)
    {
        critical_section_enter(&pool->m_config.critical_section);
    }
}

static inline void _unlock_if_needed(static_pool_t * pool)
{
    if (pool->m_config.use_lock)
    {
        critical_section_exit(&pool->m_config.critical_section);
    }
}

static inline bool _is_token_in_bounds(static_pool_t * pool, size_t token)
{
    return token < pool->m_config.object_count;
}

static inline bool _is_token_allocated(static_pool_t * pool, size_t token)
{
    // note we don;t check bounds here, call _is_token_in_bounds first.
    return (pool->m_config.metadata_buffer[token].allocated);
}

static inline error_t _validate_config(static_pool_config_t const * config)
{
    if ((config->buffer == NULL) || (config->metadata_buffer == 0)) return ERR_NULL_POINTER;
    if (config->buffer_size < (config->object_size * config->object_count)) return ERR_INVALID_ARG;
    if (config->use_lock)
    {
        error_t ret;
        ret = critical_section_validate_interface(&config->critical_section);
        if (ret != ERR_NONE) return ret;
    }
    return ERR_NONE;
}

void static_pool_get_interface(object_pool_t * interface, static_pool_t * pool)
{
    interface->context = (void *) pool;
    interface->allocate = (object_pool_allocate_t)static_pool_allocate;
    interface->fetch = (object_pool_fetch_t)static_pool_fetch;
    interface->deallocate = (object_pool_deallocate_t)static_pool_deallocate;
    interface->get_available_count = (object_pool_get_available_count_t)static_pool_get_available_count;
}

error_t static_pool_init(static_pool_t * pool, static_pool_config_t const * config)
{
    error_t ret;
    if ((pool == NULL) || (config == NULL)) return ERR_NULL_POINTER;
    ret = _validate_config(config);
    if (ret != ERR_NONE) return ret;

    pool->m_initialised = true;
    pool->m_config = *config;
    pool->m_slots_remaining = config->object_count;
    // initialise all the metadata and other fields
    for (size_t idx = 0; idx < pool->m_config.object_count; ++idx)
    {
        pool->m_config.metadata_buffer[idx].allocated = false;
        pool->m_config.metadata_buffer[idx].object_location = (void *)&pool->m_config.buffer[idx * pool->m_config.object_size];
    }

    return ERR_NONE;
}

error_t static_pool_deinit(static_pool_t * pool)
{
    if (pool == NULL) return ERR_NULL_POINTER;
    pool->m_initialised = false;
    return ERR_NONE;
}

error_t static_pool_allocate(static_pool_t * pool, size_t * token)
{
    error_t ret;
    size_t object_index = OBJECT_POOL_TOKEN_INVALID;
    if ((pool == NULL) || (token == NULL)) return ERR_NULL_POINTER;
    if (!pool->m_initialised) return ERR_NOT_INITIALISED;

    _lock_if_needed(pool);

    // find a spare slot.
    for (size_t idx = 0; idx < pool->m_config.object_count; ++idx)
    {
        if (!pool->m_config.metadata_buffer[idx].allocated)
        {
            pool->m_config.metadata_buffer[idx].allocated = true;
            object_index = idx;
            pool->m_slots_remaining--;
            break;
        }
    }

    _unlock_if_needed(pool);

    if (object_index != OBJECT_POOL_TOKEN_INVALID)
    {
        // we found a spot and have allocated it
        *token = object_index;
        return ERR_NONE;
    }
    return ERR_NO_MEM;
}

error_t static_pool_fetch(static_pool_t * pool, size_t token, void ** object_pointer)
{
    error_t ret;
    if ((pool == NULL) || (object_pointer == NULL)) return ERR_NULL_POINTER;
    if (!pool->m_initialised) return ERR_NOT_INITIALISED;
    if (!_is_token_in_bounds(pool, token)) return ERR_OUT_OF_BOUNDS;

    if (_is_token_allocated(pool, token))
    {
        *object_pointer = pool->m_config.metadata_buffer[token].object_location;
        ret = ERR_NONE;
    }
    else
    {
        *object_pointer = NULL;
        ret = ERR_INVALID_STATE;
    }
    return ret;
}

error_t static_pool_deallocate(static_pool_t * pool, size_t * token)
{
    error_t ret = ERR_NOOP;
    if ((pool == NULL) || (token == NULL)) return ERR_NULL_POINTER;
    if (!pool->m_initialised) return ERR_NOT_INITIALISED;
    if (!_is_token_in_bounds(pool, *token)) return ERR_OUT_OF_BOUNDS;

    if (_is_token_allocated(pool, *token))
    {
        _lock_if_needed(pool);
        static_metadata_t * metadata = &pool->m_config.metadata_buffer[*token];
        metadata->allocated = false;
        memset(metadata->object_location, 0, pool->m_config.object_size);
        *token = OBJECT_POOL_TOKEN_INVALID;
        pool->m_slots_remaining++;
        _unlock_if_needed(pool);
        
        ret = ERR_NONE;
    }
    else
    {
        ret = ERR_INVALID_STATE;
    }
    
    return ret;
}

error_t static_pool_get_available_count(static_pool_t * pool, size_t * unused_count)
{
    error_t ret = ERR_NONE;
    if ((pool == NULL) || (unused_count == NULL)) return ERR_NULL_POINTER;
    if (!pool->m_initialised) return ERR_NOT_INITIALISED;

    *unused_count = pool->m_slots_remaining;

    return ret;
}