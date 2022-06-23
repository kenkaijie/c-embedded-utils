/**
 * @file
 * @brief Contains items related to the circular buffer.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "error_codes.h"

typedef struct CircularBuffer CircularBuffer_t;
typedef struct CircularBufferConfig CircularBufferConfig_t;

/**
 * @brief Configuration values for #CircularBuffer
 */
struct CircularBufferConfig
{
    uint8_t * buffer;
    size_t buffer_size;
};

/**
 * @brief Circular buffer for storing singular bytes.
 * 
 * Supports queueing multiple bytes or single bytes. This is essentially just a queue with override for the oldest
 * values.
 *
 * Supports single consumer single producer applications. (like ISR to main thread, etc.)
 *
 * Similar to any other interface, but has size attached in the event of sending multiple bytes.
 */
struct CircularBuffer
{
    uint8_t * buffer;
    size_t buffer_max_size;
    size_t read_index;
    size_t write_index;
    bool buffer_is_empty; /**< We use this method of determining between FULL and Empty */
};

/**
 * @brief  Initialises a circular byte buffer.
 * 
 * @param[in] buffer - the circular buffer
 * @param[in] buffer_config - the configuration parameters to initialise with
 * 
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG
 * 
 * @memberof CircularBuffer
 */
ErrorCode_t circular_buffer_init(CircularBuffer_t * buffer, CircularBufferConfig_t const * buffer_config);

/**
 * @brief  De-initialises the circular byte buffer to a safe state.
 * 
 * @param[in] buffer - the circular buffer
 * 
 * @memberof CircularBuffer
 */
void circular_buffer_deinit(CircularBuffer_t * buffer);

/**
 * @brief  Pushes a value onto the circular buffer
 * 
 * @param[in] buffer - the circular buffer
 * @param[in] byte - the byte to push
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NO_MEM - buffer has no space to push bytes (mostly likely because it has been deinitialised)
 * 
 * @memberof CircularBuffer
 */
ErrorCode_t circular_buffer_push_byte(CircularBuffer_t * buffer, uint8_t byte);

/**
 * @brief Pops a value from the circular buffer.
 * 
 * @param[in] buffer - the circular buffer
 * @param[inout] byte - the pointer to the location to store the popped byte
 * 
 * @retval #ERR_NONE
 * @retval #ERR_EMPTY - buffer is empty
 * 
 * @memberof CircularBuffer
 */
ErrorCode_t circular_buffer_pop_byte(CircularBuffer_t * buffer, uint8_t * byte);

/**
 * @brief Gets the number of items within the buffer.
 * 
 * @param[in] buffer - the circular buffer
 * 
 * @returns Number of items in the buffer
 * 
 * @memberof CircularBuffer
 */
size_t circular_buffer_get_count(CircularBuffer_t const * buffer);
