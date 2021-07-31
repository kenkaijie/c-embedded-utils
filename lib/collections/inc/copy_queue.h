/**
 * @file
 * @brief Contains implementations for a shallow copied queue.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include "error_codes.h"

typedef struct CopyQueue CopyQueue_t;
typedef struct CopyQueueConfig CopyQueueConfig_t;

/**
 * @class CopyQueueConfig
 */
struct CopyQueueConfig
{
    uint8_t * queue_buffer; /**< Additional stack internally needed to store the memory */
    size_t queue_size; /**< the total size of the queue in bytes, should be equal to the value element_count * element_size*/
    size_t element_count; /**< The size of an element in the queue */
    size_t element_size; /**< The number of elements in the queue */
};

/**
 * @brief A queue which copies values into itself (removing the need to maintain the lifecycle).
 * 
 * @note This does a shallow copy only, for a deep copy use a different queue which provides a deep copy function. ( or
 *       allows passing in of a copy function).
 */
struct CopyQueue
{
    size_t count;
    size_t max_size;
    size_t element_size;
    uint8_t * buffer;
    size_t tail_idx;
    size_t head_idx;
};

/**
 * @brief Initialises a copy queue, which copies elements into the queue
 * 
 * @param[in] queue - the queue to use
 * @param[in] config - the queue's configuration
 * 
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG
 * 
 * @memberof CopyQueue
 */
ErrorCode_t copy_queue_init(CopyQueue_t * queue, CopyQueueConfig_t const * config);

/**
 * @brief  Deinitialises a queue.
 * 
 * @param[in] queue - the queue to use
 * 
 * @memberof CopyQueue
 */
void copy_queue_deinit(CopyQueue_t * queue);

/**
 * @brief Places an element in the queue, note the element will be copied in.
 * 
 * @param[in] queue - the queue to use
 * @param[in] element - The element to copy into the queue. Note this element is not checked, but should be the same type as the queue was initialised.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NO_MEM - queue is full
 * 
 * @memberof CopyQueue
 */
ErrorCode_t copy_queue_enqueue(CopyQueue_t * queue, void const * element);

/**
 * @brief Removes the next item that can be popped from the queue.
 * 
 * @param[in] queue - the queue to use
 * @param[inout] element - the element to save the queue to. Must be of the type the queue was initialised to.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_EMPTY - The queue has no elements
 * 
 * @memberof CopyQueue
 */
ErrorCode_t copy_queue_dequeue(CopyQueue_t * queue, void * element);

/**
 * @brief  Peeks at the next item that can be popped from the queue.
 * 
 * @param[in] queue - the queue to use
 * @param[inout] element - the element to save the queue to. Must be of the type the queue was initialised to.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_EMPTY - The queue has no elements
 * 
 * @memberof CopyQueue
 */
ErrorCode_t copy_queue_peek(CopyQueue_t * queue, void * element);

/**
 *  @brief Gets the number of free items in the queue.
 * 
 *  @param[in] queue - the queue to use
 * 
 *  @returns The number of items in the queue, aka the count.
 * 
 * @memberof CopyQueue
 */
size_t copy_queue_get_remaining(CopyQueue_t const * queue);

/**
 *  @brief Gets the number of items in the queue.
 * 
 *  @param[in] queue - the queue to use
 * 
 *  @returns The number of items in the queue, aka the count.
 * 
 * @memberof CopyQueue
 */
size_t copy_queue_get_size(CopyQueue_t const * queue);