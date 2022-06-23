/**
 * @file
 * @brief Contains implementation of a fast arithmetic circular buffer.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "error_codes.h"

typedef struct FastCircularBuffer FastCircularBuffer_t;
typedef struct FastCircularBufferConfig FastCircularBufferConfig_t;

/**
 * @brief Configurations for the #FastCircularBuffer
 */
struct FastCircularBufferConfig
{
    uint8_t * buffer;
    size_t buffer_size; /**< Buffer size must be a power of 2 and greater than 1. */
};

/**
 * @brief Fast circular buffer, similar to the standard explicit buffer.
 * 
 * @note Uses buffer sizes to a power of 2 to speed up arithmetic. Therefore the configured size must be a power of 2.
 */
struct FastCircularBuffer
{
    uint8_t * buffer;
    size_t read_index;
    size_t write_index;
    size_t size_mask;
};

/**
 * @brief  Initialises a circular byte buffer.
 * 
 * @param[in] buffer - the circular buffer
 * @param[in] buffer_config - the configuration parameters
 * 
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG
 * 
 * @memberof FastCircularBuffer
 */
ErrorCode_t fast_circular_buffer_init(FastCircularBuffer_t * buffer, FastCircularBufferConfig_t const * buffer_config);

/**
 * @brief  Deinitialises the circular byte buffer to a safe state.
 * 
 * @param[in] buffer - the circular buffer
 * 
 * @memberof FastCircularBuffer
 */
void fast_circular_buffer_deinit(FastCircularBuffer_t * buffer);

/**
 * @brief  Pushes a value onto the circular buffer
 * 
 * @param[in] buffer - the circular buffer
 * @param[in] byte - the byte to push
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NO_MEM - buffer has no space to push bytes (mostly likely because it has been deinitialised)
 * 
 * @memberof FastCircularBuffer
 */
ErrorCode_t fast_circular_buffer_push_byte(FastCircularBuffer_t * buffer, uint8_t byte);

/**
 * @brief Pops a value from the circular buffer.
 * 
 * @param[in] buffer - the circular buffer
 * @param[inout] byte - the pointer to the location to store the popped byte
 * 
 * @retval #ERR_NONE
 * @retval #ERR_EMPTY - buffer is empty
 * 
 * @memberof FastCircularBuffer
 */
ErrorCode_t fast_circular_buffer_pop_byte(FastCircularBuffer_t * buffer, uint8_t * byte);

/**
 *  @brief  Gets the number of items within the buffer.
 * 
 *  @param[in] buffer - the circular buffer
 * 
 *  @returns    Number of items in the buffer
 * 
 * @memberof FastCircularBuffer
 */
size_t fast_circular_buffer_get_count(FastCircularBuffer_t const * buffer);
