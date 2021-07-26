#include "fast_circular_buffer.h"

#include "numeric_ops.h"

static error_t fast_circular_buffer_validate_config(fast_circular_buffer_config_t const * buffer_config)
{
    if (buffer_config->buffer == NULL) return ERR_NULL_POINTER;
    if (buffer_config->buffer_size < 2) return ERR_INVALID_ARG;
    if (!numeric_ops_is_power_2_sz(buffer_config->buffer_size)) return ERR_INVALID_ARG;
    return ERR_NONE;
}

error_t fast_circular_buffer_init(fast_circular_buffer_t * buffer, fast_circular_buffer_config_t const * buffer_config)
{
    if (buffer_config == NULL) return ERR_NULL_POINTER;
    error_t result = fast_circular_buffer_validate_config(buffer_config);
    if (result != ERR_NONE) return result;

    buffer->m_buffer = buffer_config->buffer;
    buffer->m_size_mask = buffer_config->buffer_size - 1;

    buffer->m_read_index = 0;
    buffer->m_write_index = 0;

    return ERR_NONE;
}

void fast_circular_buffer_deinit(fast_circular_buffer_t * buffer)
{
    buffer->m_size_mask = 0;
    buffer->m_read_index = 0;
    buffer->m_write_index = 0;
}

error_t fast_circular_buffer_push_byte(fast_circular_buffer_t * buffer, uint8_t byte)
{
    if (buffer->m_size_mask == 0) return ERR_NO_MEM;

    buffer->m_buffer[buffer->m_write_index & buffer->m_size_mask] = byte;
    if (fast_circular_buffer_get_count(buffer) > buffer->m_size_mask)
    {
        buffer->m_read_index = buffer->m_write_index - buffer->m_size_mask;
    }
    buffer->m_write_index++;
    return ERR_NONE;
}

error_t fast_circular_buffer_pop_byte(fast_circular_buffer_t * buffer, uint8_t * byte)
{
    if (byte == NULL) return ERR_NULL_POINTER;
    if (fast_circular_buffer_get_count(buffer) == 0) return ERR_EMPTY;

    *byte = buffer->m_buffer[buffer->m_read_index & buffer->m_size_mask];
    buffer->m_read_index++;
    return ERR_NONE;
}

size_t fast_circular_buffer_get_count(fast_circular_buffer_t const * buffer)
{
    return buffer->m_write_index - buffer->m_read_index;
}