#pragma once

/**
 * @file    An interface for a object pool
 */

#include <stddef.h>
#include "error_codes.h"

#define OBJECT_POOL_TOKEN_INVALID (SIZE_MAX)

typedef struct s_object_pool object_pool_t;
typedef error_t(*object_pool_allocate_t)(void * pool, size_t * token);
typedef error_t(*object_pool_fetch_t)(void * pool, size_t token, void ** object_pointer);
typedef error_t(*object_pool_deallocate_t)(void * pool, size_t * token);

struct s_object_pool
{
    void * context;
    object_pool_allocate_t allocate;
    object_pool_fetch_t fetch;
    object_pool_deallocate_t deallocate;
};

inline error_t object_pool_validate_interface(object_pool_t const * interface)
{
    if ((interface == NULL) || (interface->allocate == NULL) || (interface->fetch == NULL) || (interface->deallocate == NULL)) return ERR_NULL_POINTER;
    return ERR_NONE;
}

/**
 *  @brief  Requests an object from the pool. If successful, this function will return the token that can be used to fetch the object.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  @param[inout] token - The token used to do future retrieves and freeing of objects
 * 
 *  @returns    ERR_NONE - A slot has been allocated, the token represents an object within the pool.
 *              ERR_NO_MEM - No slots are available
 *              ERR_NOT_INITIALISED - The pool has not been initialised.
 */
inline error_t object_pool_allocate(object_pool_t const * interface, size_t * token)
{
    error_t ret;
    ret = object_pool_validate_interface(interface);
    if (ret == ERR_NONE)
    {
        ret = interface->allocate(interface->context, token);
    }
    return ret;
}

/**
 *  @brief  Gets the object at the token. Provide a pointer that will point to the object.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  @param[inout] token - The token used to retrieve
 *  @param[inout] object_pointer - Pointer to the object.
 * 
 *  @returns    ERR_NONE - A slot has been allocated, the token represents an object within the pool.
 *              ERR_NULL_POINTER - A null pointer was found
 *              ERR_NOT_INITIALISED - The pool has not been initialised.
 *              ERR_OUT_OF_BOUNDS - The token provided is out of bounds
 *              ERR_INVALID_STATE - Token provided points to an unallocated object.
 */
inline error_t object_pool_fetch(object_pool_t const * interface, size_t token, void ** object_pointer)
{
    error_t ret;
    ret = object_pool_validate_interface(interface);
    if (ret == ERR_NONE)
    {
        ret = interface->fetch(interface->context, token, object_pointer);
    }
    return ret;
}

/**
 *  @brief  Frees an object for reuse in the future. Note all objects will be zeroed when freed.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  @param[inout] token - The token used to free. After calling this on a token, the token is set back to the invalid token.
 * 
 *  @returns    ERR_NONE - A slot has been allocated, the token represents an object within the pool.
 *              ERR_NULL_POINTER - A null pointer was found
 *              ERR_NOT_INITIALISED - The pool has not been initialised.
 *              ERR_NOOP - The token is invalid, no freeing
 */
inline error_t object_pool_deallocate(object_pool_t const * interface, size_t * token)
{
    error_t ret;
    ret = object_pool_validate_interface(interface);
    if (ret == ERR_NONE)
    {
        ret = interface->deallocate(interface->context, token);
    }
    return ret;
}