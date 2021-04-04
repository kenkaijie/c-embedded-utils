#pragma once

/**
 * @file    An interface for a object pool
 */

#include <stddef.h>
#include "error_codes.h"

typedef struct s_object_pool object_pool_t;
typedef error_t(*object_pool_allocate_t)(void * pool, void ** object_ptr);
typedef error_t(*object_pool_deallocate_t)(void * pool, void ** object_ptr);
typedef size_t(*object_pool_get_available_count_t)(void * pool);

struct s_object_pool
{
    void * context;
    object_pool_allocate_t allocate;
    object_pool_deallocate_t deallocate;
    object_pool_get_available_count_t get_available_count;
};

/**
 *  @brief  Requests an object from the pool. If successful, this function will return the token that can be used to fetch the object.
 * 
 *  @param[in] interface - The pointer to the pool object
 *  @param[inout] token - The token used to do future retrieves and freeing of objects
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_NO_MEM - No slots are available
 */
inline error_t object_pool_allocate(object_pool_t const * interface, void ** object_ptr)
{
    return interface->allocate(interface->context, object_ptr);
}

/**
 *  @brief  Frees an object for reuse in the future. Note all objects will be zeroed when freed.
 * 
 *  @param[in] interface - The pointer to the pool object
 *  @param[inout] token - The token used to free. After calling this on a token, the token is set back to the invalid token.
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_NOOP - The token is invalid, no freeing
 *              ERR_OUT_OF_BOUNDS - The token provided is out of bounds.
 *              ERR_INVALID_STATE - Token provided points to an unallocated object.
 */
inline error_t object_pool_deallocate(object_pool_t const * interface, void ** object_ptr)
{
    return interface->deallocate(interface->context, object_ptr);
}

/**
 *  @brief  Gets the number of remaining slots in the pool.
 * 
 *  @param[in] interface  - The pointer to the pool object
 * 
 *  @returns    The number of unused slots in the pool
 */
inline size_t object_pool_get_available_count(object_pool_t const * interface)
{
    return interface->get_available_count(interface->context);
}
