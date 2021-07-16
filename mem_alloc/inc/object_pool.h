/**
 * @file
 * @brief An interface for a object pool, acts as a interface layer for object pools.
 *
 * Defines a generic interface common to all object pools. Each implementer must implement all function calls, along
 * with providing the context to the pool object.
 * 
 * There are helper functions here to ensure the context is passed as the first element. And are basically wrappers for
 * the following:
 * ```
 * pool_interface->allocate(pool_interface->context, ...)
 * ```
 */

#pragma once

#include <stddef.h>
#include "error_codes.h"

typedef struct object_pool object_pool_t;

/**
 * @brief The generic function pointer that will allocate an object from the pool if possible. 
 * 
 * @param[in] context - The pointer to the pool object, used by concrete.
 * @param[inout] object_ptr - The pointer to the element allocated.
 *
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_NO_MEM - No slots are available
 */
typedef error_t(*object_pool_allocate_t)(void * context, void ** object_ptr);

/**
 * @brief The generic function pointer that will deallocate an object from the pool.
 * 
 * The pointer provided will be set to NULL after the call.
 * 
 * @param[in] context - The pointer to the pool object, used by concrete.
 * @param[inout] object_ptr - The pointer to free. This pointer must be set back to NULL after it has been freed 
 *                            (regardless if the freeing was successful).
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_OUT_OF_BOUNDS - The token provided is out of bounds. (is it invalid)
 */
typedef error_t(*object_pool_deallocate_t)(void * context, void ** object_ptr);

/**
 * @brief Function pointer to get the remaining slots in the pool.
 * 
 * @param[in] context - The pointer to the pool object, used by concrete.
 * 
 * @returns The number of unused slots in the pool
 */
typedef size_t(*object_pool_get_available_count_t)(void * context);

struct object_pool
{
    void * context;
    object_pool_allocate_t allocate;
    object_pool_deallocate_t deallocate;
    object_pool_get_available_count_t get_available_count;
};

/**
 * @brief  Requests an object from the pool. If successful, this function will return the token that can be used to
 * fetch the object.
 * 
 * @param[in] interface - The pointer to the pool object.
 * @param[inout] token - The pointer to the element in the object pool.
 *
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_NO_MEM - No slots are available
 */
error_t object_pool_allocate(object_pool_t const * interface, void ** object_ptr);

/**
 * @brief  Frees an object for reuse in the future. Note all objects will be zeroed when freed.
 * 
 * @param[in] interface - The pointer to the pool object
 * @param[inout] token - The token used to free. After calling this on a token, the token is set back to the invalid
 * token. (NULL)
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_OUT_OF_BOUNDS - The token provided is out of bounds.
 */
error_t object_pool_deallocate(object_pool_t const * interface, void ** object_ptr);

/**
 * @brief  Gets the number of remaining slots in the pool.
 * 
 * @param[in] interface  - The pointer to the pool object
 * 
 * @returns The number of unused slots in the pool
 */
size_t object_pool_get_available_count(object_pool_t const * interface);