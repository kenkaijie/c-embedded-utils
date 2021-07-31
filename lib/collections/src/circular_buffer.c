#include "circular_buffer.h"

#include <assert.h>

ErrorCode_t circular_buffer_validate_config(CircularBufferConfig_t const * config)
{
    assert(config->buffer);

    if (config->buffer_size == 0) return ERR_INVALID_ARG;
    return ERR_NONE;
}

ErrorCode_t circular_buffer_init(CircularBuffer_t * buffer, CircularBufferConfig_t const * config)
{
    assert(buffer);
    assert(config);

    ErrorCode_t result = circular_buffer_validate_config(config);
    if (result != ERR_NONE) return result;
    buffer->read_index = 0;
    buffer->write_index = 0;
    buffer->buffer_is_empty = true;
    buffer->buffer = config->buffer;
    buffer->buffer_max_size = config->buffer_size;

    return ERR_NONE;
}

void circular_buffer_deinit(CircularBuffer_t * buffer)
{
    assert(buffer);

    buffer->buffer_max_size = 0;
    buffer->read_index = 0;
    buffer->write_index = 0;
    buffer->buffer_is_empty = true;
}

ErrorCode_t circular_buffer_push_byte(CircularBuffer_t * buffer, uint8_t byte)
{
    assert(buffer);

    if (buffer->buffer_max_size == 0) return ERR_NO_MEM;
    buffer->buffer[buffer->write_index] = byte;
    
    if ((buffer->write_index == buffer->read_index) && !buffer->buffer_is_empty)
    {
        buffer->read_index = (buffer->read_index + 1) % buffer->buffer_max_size;
    }

    buffer->write_index = (buffer->write_index + 1) % buffer->buffer_max_size;
    buffer->buffer_is_empty = false;

    return ERR_NONE;
}

ErrorCode_t circular_buffer_pop_byte(CircularBuffer_t * buffer, uint8_t * byte)
{
    assert(buffer);
    assert(byte);

    if (circular_buffer_get_count(buffer) == 0) return ERR_EMPTY;

    *byte = buffer->buffer[buffer->read_index];
    buffer->read_index = (buffer->read_index + 1) % buffer->buffer_max_size;
    buffer->buffer_is_empty = (buffer->read_index == buffer->write_index);

    return ERR_NONE;
}

size_t circular_buffer_get_count(CircularBuffer_t const * buffer)
{
    assert(buffer);
    
    size_t estimated_size;

    if(buffer->write_index > buffer->read_index)
    {
        estimated_size = buffer->write_index - buffer->read_index;
    }
    else if (buffer->write_index < buffer->read_index)
    {
        estimated_size = buffer->buffer_max_size - buffer->read_index + buffer->write_index;
    }
    else
    {
        estimated_size = (buffer->buffer_is_empty) ? 0 : buffer->buffer_max_size;
    }

    return estimated_size;
}
