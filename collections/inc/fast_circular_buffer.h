#pragma once

/**
 *  @file   Fast circular buffer, similar to the standard explicit buffer. 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "error_codes.h"

typedef struct s_fast_circular_buffer fast_circular_buffer_t;
typedef struct s_fast_circular_buffer_config fast_circular_buffer_config_t;

struct s_fast_circular_buffer_config 
{
    uint8_t * buffer;
    size_t buffer_size; /**< buffer must be a power of 2 and greater than 1*/
};

struct s_fast_circular_buffer
{
    uint8_t * m_buffer;
    size_t m_read_index;
    size_t m_write_index;
    size_t m_size_mask;
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
error_t fast_circular_buffer_init(fast_circular_buffer_t * buffer, fast_circular_buffer_config_t const * buffer_config);

/**
 *  @brief  Deinitialises the circular byte buffer to a safe state.
 * 
 *  @param[in] buffer - the circular buffer
 */
void fast_circular_buffer_deinit(fast_circular_buffer_t * buffer);

/**
 *  @brief  Pushes a value onto the circular buffer
 * 
 *  @param[in] buffer - the circular buffer
 *  @param[in] byte - the byte to push
 * 
 *  @returns    ERR_NONE
 *              ERR_NO_MEM - buffer has no space to push bytes (mostly likely because it has been deinitialised)
 */
error_t fast_circular_buffer_push_byte(fast_circular_buffer_t * buffer, uint8_t byte);

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
error_t fast_circular_buffer_pop_byte(fast_circular_buffer_t * buffer, uint8_t * byte);

/**
 *  @brief  Gets the number of items within the buffer.
 * 
 *  @param[in] buffer - the circular buffer
 * 
 *  @returns    Number of items in the buffer
 */
size_t fast_circular_buffer_get_count(fast_circular_buffer_t const * buffer);
