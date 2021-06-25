#include "circular_buffer.h"

error_t _validate_config(circular_buffer_config_t const * config)
{
    if (config->buffer == NULL) return ERR_NULL_POINTER;
    if (config->buffer_size == 0) return ERR_INVALID_ARG;
    return ERR_NONE;
}

error_t circular_buffer_init(circular_buffer_t * buffer, circular_buffer_config_t const * config)
{
    if (config == NULL) return ERR_NULL_POINTER;
    error_t result = _validate_config(config);
    if (result != ERR_NONE) return result;
    buffer->m_read_index = 0;
    buffer->m_write_index = 0;
    buffer->m_buffer_is_empty = true;
    buffer->m_buffer = config->buffer;
    buffer->m_buffer_max_size = config->buffer_size;

    return ERR_NONE;
}

void circular_buffer_deinit(circular_buffer_t * buffer)
{
    buffer->m_buffer_max_size = 0;
    buffer->m_read_index = 0;
    buffer->m_write_index = 0;
    buffer->m_buffer_is_empty = true;
}

error_t circular_buffer_push_byte(circular_buffer_t * buffer, uint8_t byte)
{
    if (buffer->m_buffer_max_size == 0) return ERR_NO_MEM;
    buffer->m_buffer[buffer->m_write_index] = byte;
    
    if ((buffer->m_write_index == buffer->m_read_index) && !buffer->m_buffer_is_empty)
    {
        buffer->m_read_index = (buffer->m_read_index + 1) % buffer->m_buffer_max_size;
    }

    buffer->m_write_index = (buffer->m_write_index + 1) % buffer->m_buffer_max_size;
    buffer->m_buffer_is_empty = false;

    return ERR_NONE;
}

error_t circular_buffer_pop_byte(circular_buffer_t * buffer, uint8_t * byte)
{
    if (byte == NULL) return ERR_NULL_POINTER;
    if (circular_buffer_get_count(buffer) == 0) return ERR_EMPTY;

    *byte = buffer->m_buffer[buffer->m_read_index];
    buffer->m_read_index = (buffer->m_read_index + 1) % buffer->m_buffer_max_size;
    buffer->m_buffer_is_empty = (buffer->m_read_index == buffer->m_write_index);

    return ERR_NONE;
}

size_t circular_buffer_get_count(circular_buffer_t const * buffer)
{
    size_t estimated_size;

    if(buffer->m_write_index > buffer->m_read_index)
    {
        estimated_size = buffer->m_write_index - buffer->m_read_index;
    }
    else if (buffer->m_write_index < buffer->m_read_index)
    {
        estimated_size = buffer->m_buffer_max_size - buffer->m_read_index + buffer->m_write_index;
    }
    else
    {
        estimated_size = (buffer->m_buffer_is_empty) ? 0 : buffer->m_buffer_max_size;
    }

    return estimated_size;
}
