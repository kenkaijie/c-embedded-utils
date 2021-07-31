/**
 * @file
 * @brief A statically allocated object pool (no expansion possible).
 *
 * Does not take into account threaded operations, as this pool does not lock. For thread safety, a shim must be applied
 * over this to ensure correct operation.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "i_pool_allocator.h"
#include "error_codes.h"
#include "ptr_stack.h"

typedef struct StaticPool StaticPool_t;
typedef struct StaticPoolConfig StaticPoolConfig_t;

/**
 * @brief Config parameters for the #StaticPool.
 * @class StaticPoolConfig
 */
struct StaticPoolConfig
{
    void ** allocation_stack; /**< An allocation stack needed to for keeping track of the free spaces, should have object_size items. */
    uint8_t * buffer; /**< The size of the buffer to manage */
    size_t buffer_size; /**< Size of the buffer in bytes, note the object size must be an integer multiple of the object size. */
    size_t object_size; /**< Size of each object in bytes. */
    size_t object_count; /**< Number of objects this pool manages. Note that (object_size * object_count == buffer_size) */
};

/**
 * @brief A statically allocated pool of member blocks.
 * 
 * Each block in this pool has the same size, and is designed for allowing for "dynamic" like memory use in a static 
 * only style environment. Useful for allocating and freeing some run time data if needed.
 * 
 * @class StaticPool
 */
struct StaticPool
{
    StaticPoolConfig_t config;
    PtrStack_t free_stack;
};

/**
 * @brief  Gets the pool allocator interface corresponding to this static pool.
 * 
 * @param[in] pool - The pool to create the interface for
 * @param[in] interface - The interface to configure
 * 
 * @retval #ERR_NONE
 * 
 * @memberof StaticPool
 */
ErrorCode_t static_pool_as_i_pool_allocator(StaticPool_t * pool, IPoolAllocator_t * interface);

/**
 * @brief  Initialises a static pool with the provided config.
 * 
 * @param[in] pool - The pointer to the static pool object
 * 
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG - The provided configuration is invalid
 * 
 * @memberof StaticPool
 */
ErrorCode_t static_pool_init(StaticPool_t * pool, StaticPoolConfig_t const * config);

/**
 * @brief  De-initialises a static pool.
 * 
 * @param[in] pool - The pointer to the static pool object
 * 
 * @memberof StaticPool
 */
void static_pool_deinit(StaticPool_t * pool);

/**
 * @brief  Requests an object from the pool. If successful, this function will return the token that can be used to fetch the object.
 * 
 * @param[in] pool - The pointer to the static pool object
 * @param[inout] object_ptr - The pointer to the object acquired.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NO_MEM - No slots are available
 * 
 * @memberof StaticPool
 */
ErrorCode_t static_pool_allocate(StaticPool_t * pool, void ** object_ptr);

/**
 * @brief  Frees an object for reuse in the future. Note objects may not be zeroed when freed.
 *
 * @copydoc object_pool_allocate
 *
 * @param[in] pool - The pointer to the static pool object
 * @param[inout] object_ptr - The object to free back into the pool. The pointer provided will return to NULL after freeing. Note the object provided should be
 * 
 * @retval #ERR_NONE
 * @retval #ERR_OUT_OF_BOUNDS - The token provided is out of bounds.
 * 
 * @memberof StaticPool
 */
ErrorCode_t static_pool_deallocate(StaticPool_t * pool, void ** object_ptr);

/**
 * @brief  Gets the number of remaining slots in the pool.
 * 
 * @param[in] pool  - The pointer to the static pool object
 * 
 * @returns The number of unused slots in the pool
 * 
 * @memberof StaticPool
 */
size_t static_pool_get_available_count(StaticPool_t * pool);
