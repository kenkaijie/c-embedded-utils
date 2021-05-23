#pragma once

/**
 *  @file   Circular buffer for storing singular bytes. Supports queueing multiple bytes or 
 *          single bytes. This is essentially just a queue with override for the oldest values.
 * 
 *          Supports single consumer single producer applications. (like ISR to main thread, etc.)
 * 
 *          Similar to any other interface, but has size attached in the event of sending multiple bytes.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "error_codes.h"

typedef struct s_circular_buffer circular_buffer_t;
typedef struct s_circular_buffer_config circular_buffer_config_t;

struct s_circular_buffer_config 
{
    uint8_t * buffer;
    size_t buffer_size;
};

struct s_circular_buffer
{
    uint8_t * m_buffer;
    size_t m_buffer_max_size;
    size_t m_read_index;
    size_t m_write_index;
    bool m_buffer_is_empty; /**< We use this method of determinig between FULL and Empty */
};

/**
 *  @brief  Initialises a circular byte buffer.
 * 
 *  @param[in] buffer - the circular buffer
 *  @param[in] buffer_config - the configuration parameters to initialise with
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_INVALID_ARG
 */
error_t circular_buffer_init(circular_buffer_t * buffer, circular_buffer_config_t const * buffer_config);

/**
 *  @brief  Deinitialises the circular byte buffer to a safe state.
 * 
 *  @param[in] buffer - the circular buffer
 */
void circular_buffer_deinit(circular_buffer_t * buffer);

/**
 *  @brief  Pushes a value onto the circular buffer
 * 
 *  @param[in] buffer - the circular buffer
 *  @param[in] byte - the byte to push
 * 
 *  @returns    ERR_NONE
 *              ERR_OVERRIDE - value pushed onto the queue popped the oldest value out
 *              ERR_NO_MEM - buffer has no space to push bytes (mostly likely because it has been deinitialised)
 */
error_t circular_buffer_push_byte(circular_buffer_t * buffer, uint8_t byte);

/**
 *  @brief  Pops a value from the circular buffer.
 * 
 *  @param[in] buffer - the circular buffer
 *  @param[inout] byte - the pointer to the location to store the popped byte
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_EMPTY - buffer is empty
 */
error_t circular_buffer_pop_byte(circular_buffer_t * buffer, uint8_t * byte);

/**
 *  @brief  Gets the number of items within the buffer.
 * 
 *  @param[in] buffer - the circular buffer
 * 
 *  @returns    Number of items in the buffer
 */
size_t circular_buffer_get_size(circular_buffer_t const * buffer);
