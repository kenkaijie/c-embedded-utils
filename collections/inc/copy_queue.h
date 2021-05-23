#pragma once

/**
 *  @file   A queue with elements copied into the queue, Used for any application that needs queuing to prevent resource blocking.
 *          Similar to the a circular buffer, but with a capped size. Does not loop around.
 * 
 *          Uses a static memory pool under the hood, suitable mostly for larger objects.
 */
#include <stddef.h>
#include <stdint.h>
#include "error_codes.h"

typedef struct s_copy_queue copy_queue_t;
typedef struct s_copy_queue_config copy_queue_config_t;

struct s_copy_queue_config
{
    uint8_t * queue_buffer; /**< Additional stack internally needed to store the memory */
    size_t queue_size; /**< the total size of the queue in bytes, should be equal to the value element_count * element_size*/
    size_t element_count; /**< The size of an element in the queue */
    size_t element_size; /**< The number of elements in the queue */
};

struct s_copy_queue
{
    size_t m_count;
    size_t m_max_size;
    size_t m_element_size;
    uint8_t * m_buffer;
    size_t m_tail_idx;
    size_t m_head_idx;
};

/**
 *  @brief  Initialises a copy queue, which copies elements into the queue
 * 
 *  @param[in] queue - the queue to use
 *  @param[in] config - the queue's configuration
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_INVALID_ARG
 */
error_t copy_queue_init(copy_queue_t * queue, copy_queue_config_t const * config);

/**
 *  @brief  Deinitialises a queue.
 * 
 *  @param[in] queue - the queue to use
 */
void copy_queue_deinit(copy_queue_t * queue);

/**
 *  @brief  Places an element in the queue, note the element will be copied in.
 * 
 *  @param[in] queue - the queue to use
 *  @param[in] element - The element to copy into the queue. Note this element is not checked, but should be the same type as the queue was initialised.
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_NO_MEM - queue is full
 */
error_t copy_queue_enqueue(copy_queue_t * queue, void const * element);

/**
 *  @brief  Removes the next item that can be popped from the queue.
 * 
 *  @param[in] queue - the queue to use
 *  @param[inout] element - the element to save the queue to. Must be of the type the queue was initialised to.
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_EMPTY - The queue has no elements
 */
error_t copy_queue_dequeue(copy_queue_t * queue, void * element);

/**
 *  @brief  Peeks at the next item that can be popped from the queue.
 * 
 *  @param[in] queue - the queue to use
 *  @param[inout] element - the element to save the queue to. Must be of the type the queue was initialised to.
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_EMPTY - The queue has no elements
 */
error_t copy_queue_peek(copy_queue_t * queue, void * element);

/**
 *  @brief  Gets the number of free items in the queue.
 * 
 *  @param[in] queue - the queue to use
 * 
 *  @returns    The number of items in the queue, aka the count.
 */
size_t copy_queue_get_remaining(copy_queue_t const * queue);

/**
 *  @brief  Gets the number of items in the queue.
 * 
 *  @param[in] queue - the queue to use
 * 
 *  @returns    The number of items in the queue, aka the count.
 */
size_t copy_queue_get_size(copy_queue_t const * queue);