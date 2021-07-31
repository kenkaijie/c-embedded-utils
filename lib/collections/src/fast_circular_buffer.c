#include "fast_circular_buffer.h"

#include <assert.h>
#include "numeric_ops.h"

static ErrorCode_t fast_circular_buffer_validate_config(FastCircularBufferConfig_t const * buffer_config)
{
    assert(buffer_config->buffer);

    if (buffer_config->buffer_size < 2) return ERR_INVALID_ARG;
    if (!numeric_ops_is_power_2_sz(buffer_config->buffer_size)) return ERR_INVALID_ARG;
    return ERR_NONE;
}

ErrorCode_t fast_circular_buffer_init(FastCircularBuffer_t * buffer, FastCircularBufferConfig_t const * buffer_config)
{
    assert(buffer);
    assert(buffer_config);

    ErrorCode_t result = fast_circular_buffer_validate_config(buffer_config);
    if (result != ERR_NONE) return result;

    buffer->buffer = buffer_config->buffer;
    buffer->size_mask = buffer_config->buffer_size - 1;

    buffer->read_index = 0;
    buffer->write_index = 0;

    return ERR_NONE;
}

void fast_circular_buffer_deinit(FastCircularBuffer_t * buffer)
{
    assert(buffer);

    buffer->size_mask = 0;
    buffer->read_index = 0;
    buffer->write_index = 0;
}

ErrorCode_t fast_circular_buffer_push_byte(FastCircularBuffer_t * buffer, uint8_t byte)
{
    assert(buffer);

    if (buffer->size_mask == 0) return ERR_NO_MEM;

    buffer->buffer[buffer->write_index & buffer->size_mask] = byte;
    if (fast_circular_buffer_get_count(buffer) > buffer->size_mask)
    {
        buffer->read_index = buffer->write_index - buffer->size_mask;
    }
    buffer->write_index++;
    return ERR_NONE;
}

ErrorCode_t fast_circular_buffer_pop_byte(FastCircularBuffer_t * buffer, uint8_t * byte)
{
    assert(buffer);
    assert(byte);

    if (fast_circular_buffer_get_count(buffer) == 0) return ERR_EMPTY;

    *byte = buffer->buffer[buffer->read_index & buffer->size_mask];
    buffer->read_index++;
    return ERR_NONE;
}

size_t fast_circular_buffer_get_count(FastCircularBuffer_t const * buffer)
{
    assert(buffer);

    return buffer->write_index - buffer->read_index;
}