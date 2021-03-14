#pragma once

/**
 * @file   A statically allocated object pool (no expansion possible). Provides optional config for critical locking.
 * 
 */ 

#include <stdbool.h>
#include "object_pool.h"
#include "error_codes.h"
#include "critical_section.h"

typedef struct s_static_pool static_pool_t;
typedef struct s_static_pool_config static_pool_config_t;
typedef struct s_static_metadata static_metadata_t;


struct s_static_metadata
{
    bool allocated;
};

struct s_static_pool_config
{
    static_metadata_t * metadata_buffer; /**< buffer used to store metadata for the static pool, must contain object count entries. */
    uint8_t * buffer; /**< The size of the bufer to manage */
    size_t buffer_size; /**< Size of the buffer in bytes, note the object size must be an integer multiple of the object size. */
    size_t object_size; /**< Size of each object in bytes. */
    size_t object_count; /**< Number of objects this pool manages. Note that (object_size * object_count == buffer_size) */
    bool use_lock; /**< Set this to true if you wish the critical section to be used. */
    critical_section_t critical_section; /**< Optional interface to a critical section lock. If use_lock is set, this must be a valid lock interface. */
};

struct s_static_pool
{
    bool m_initialised;
    size_t m_slots_remaining;
    static_pool_config_t m_config;
};

void static_pool_get_interface(object_pool_t * interface, static_pool_t * pool);

/**
 *  @brief  Initialises a static pool with the provided config.
 * 
 *  @param[in] pool - The pointer to the static pool object
 * 
 *  @returns    ERR_NONE
 */
error_t static_pool_init(static_pool_t * pool, static_pool_config_t const * config);

/**
 *  @brief  Deinitialises a static pool.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  
 *  @returns    
 */
error_t static_pool_deinit(static_pool_t * pool);

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
error_t static_pool_allocate(static_pool_t * pool, size_t * token);

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
error_t static_pool_fetch(static_pool_t * pool, size_t token, void ** object_pointer);

/**
 *  @brief  Frees an object for reuse in the future. Note objects may not be zeroed when freed.
 * 
 *  @param[in] pool - The pointer to the static pool object
 *  @param[inout] token - The token used to free. After calling this on a token, the token is set back to the invalid token.
 * 
 *  @returns    ERR_NONE - A slot has been allocated, the token represents an object within the pool.
 *              ERR_NULL_POINTER - A null pointer was found
 *              ERR_NOT_INITIALISED - The pool has not been initialised.
 *              ERR_NOOP - The token is invalid, no freeing.
 *              ERR_OUT_OF_BOUNDS - The token provided is out of bounds.
 *              ERR_INVALID_STATE - Token provided points to an unallocated object.
 */
error_t static_pool_deallocate(static_pool_t * pool, size_t * token);

/**
 *  @brief  Gets the number of remaining slots in the pool.
 * 
 *  @param[in] pool  - The pointer to the static pool object
 *  @param[inout] unused_count - The number of unused 
 * 
 *  @returns    ERR_NONE - Success
 *              ERR_NULL_POINTER - A null pointer was found
 *              ERR_NOT_INITIALISED - The pool has not been initialised.
 */
error_t static_pool_get_unused_count(static_pool_t * pool, size_t * unused_count);