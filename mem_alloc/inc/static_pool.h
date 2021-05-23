#pragma once

/**
 * @file   A statically allocated object pool (no expansion possible). Provides optional config for critical locking. Note it is the user's responsibility to ensure that the pointers passed are correct (and from this
 *          pool). If a safe pool is required, this will not work, try another pool type with safety features.
 * 
 */ 

#include <stdbool.h>
#include "object_pool.h"
#include "error_codes.h"
#include "critical_section.h"
#include "ptr_stack.h"

typedef struct s_static_pool static_pool_t;
typedef struct s_static_pool_config static_pool_config_t;

struct s_static_pool_config
{
    void ** allocation_stack; /**< an allocation stack needed to for keeping track of the free spaces, should have object_size items. */
    uint8_t * buffer; /**< The size of the bufer to manage */
    size_t buffer_size; /**< Size of the buffer in bytes, note the object size must be an integer multiple of the object size. */
    size_t object_size; /**< Size of each object in bytes. */
    size_t object_count; /**< Number of objects this pool manages. Note that (object_size * object_count == buffer_size) */
};

struct s_static_pool
{
    static_pool_config_t m_config;
    ptr_stack_t m_free_stack;
};

/**
 *  @brief  Gets the object pool interface coresponding to this static pool.
 * 
 *  @param[in] pool - The pool to create the interface for
 *  @param[in] interface - The interface to configure
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 */
error_t static_pool_as_object_pool(static_pool_t * pool, object_pool_t * interface);

/**
 *  @brief  Initialises a static pool with the provided config.
 * 
 *  @param[in] pool - The pointer to the static pool object
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_INVALID_ARG - The provided configuration is invalid
 */
error_t static_pool_init(static_pool_t * pool, static_pool_config_t const * config);

/**
 *  @brief  Deinitialises a static pool.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  
 */
void static_pool_deinit(static_pool_t * pool);

/**
 *  @brief  Requests an object from the pool. If successful, this function will return the token that can be used to fetch the object.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  @param[inout] object_ptr - The pointer to the object acquired.
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_NO_MEM - No slots are available
 */
error_t static_pool_allocate(static_pool_t * pool, void ** object_ptr);

/**
 *  @brief  Frees an object for reuse in the future. Note objects may not be zeroed when freed.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  @param[inout] object_ptr - The object to free back inot the pool. The pointer provided will return to NULL after freeing. Note the object provided should be 
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_OUT_OF_BOUNDS - The token provided is out of bounds.
 */
error_t static_pool_deallocate(static_pool_t * pool, void ** object_ptr);

/**
 *  @brief  Gets the number of remaining slots in the pool.
 * 
 *  @param[in] pool  - The pointer to the static pool object
 * 
 *  @returns    The number of unused slots in the pool
 */
size_t static_pool_get_available_count(static_pool_t * pool);
