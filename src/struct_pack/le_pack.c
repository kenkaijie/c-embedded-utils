#include <cemb/le_pack.h>

size_t le_pack_raw(uint8_t const * raw_data, size_t raw_data_size, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, raw_data_size, data_len) && raw_data_size > 0)
    {
        memcpy(data+offset, raw_data, raw_data_size);
        return raw_data_size;
    }
    return 0;
}

size_t le_unpack_raw(uint8_t * raw_data, size_t raw_data_size, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, raw_data_size, data_len) && raw_data_size > 0)
    {
        memcpy(raw_data, data+offset, raw_data_size);
        return raw_data_size;
    }
    return 0;
}

size_t le_pack_bool(bool const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_BOOL, data_len))
    {
        // for booleans, we will normalise to 0 or 1, and store in 8 bytes
        uint8_t value = (*element) ? 1 : 0;
        data[offset + 0] = (uint8_t)((value >> 0) & 0xFF);
        return PACK_SIZE_BOOL;
    }
    return 0;
}

size_t le_unpack_bool(bool * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_BOOL, data_len))
    {
        *element = ((uint8_t)(data[offset + 0] << 0) == 1);
        return PACK_SIZE_BOOL;
    }
    return 0;
}

size_t le_pack_u8(uint8_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT8_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        return PACK_SIZE_UINT8_T;
    }
    return 0;
}

size_t le_unpack_u8(uint8_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT8_T, data_len))
    {
        *element = ((uint8_t)data[offset + 0] << 0);
        return PACK_SIZE_UINT8_T;
    }
    return 0;
}

size_t le_pack_u16(uint16_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT16_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        data[offset + 1] = (uint8_t)((*element >> 8) & 0xFF);
        return PACK_SIZE_UINT16_T;
    }
    return 0;
}

size_t le_unpack_u16(uint16_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT16_T, data_len))
    {
        *element = ((uint16_t)data[offset + 0] << 0) + 
                   ((uint16_t)data[offset + 1] << 8);
        return PACK_SIZE_UINT16_T;
    }
    return 0;
}

size_t le_pack_u32(uint32_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT32_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        data[offset + 1] = (uint8_t)((*element >> 8) & 0xFF);
        data[offset + 2] = (uint8_t)((*element >> 16) & 0xFF);
        data[offset + 3] = (uint8_t)((*element >> 24) & 0xFF);
        return PACK_SIZE_UINT32_T;
    }
    return 0;
}

size_t le_unpack_u32(uint32_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT32_T, data_len))
    {
        *element = ((uint32_t)data[offset + 0] << 0) + 
                   ((uint32_t)data[offset + 1] << 8) +
                   ((uint32_t)data[offset + 2] << 16) +
                   ((uint32_t)data[offset + 3] << 24);
        return PACK_SIZE_UINT32_T;
    }
    return 0;
}

size_t le_pack_u64(uint64_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT64_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        data[offset + 1] = (uint8_t)((*element >> 8) & 0xFF);
        data[offset + 2] = (uint8_t)((*element >> 16) & 0xFF);
        data[offset + 3] = (uint8_t)((*element >> 24) & 0xFF);
        data[offset + 4] = (uint8_t)((*element >> 32) & 0xFF);
        data[offset + 5] = (uint8_t)((*element >> 40) & 0xFF);
        data[offset + 6] = (uint8_t)((*element >> 48) & 0xFF);
        data[offset + 7] = (uint8_t)((*element >> 56) & 0xFF);
        return PACK_SIZE_UINT64_T;
    }
    return 0;
}

size_t le_unpack_u64(uint64_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_UINT64_T, data_len))
    {
        *element = ((uint64_t)data[offset + 0] << 0) + 
                   ((uint64_t)data[offset + 1] << 8) +
                   ((uint64_t)data[offset + 2] << 16) +
                   ((uint64_t)data[offset + 3] << 24) +
                   ((uint64_t)data[offset + 4] << 32) +
                   ((uint64_t)data[offset + 5] << 40) +
                   ((uint64_t)data[offset + 6] << 48) +
                   ((uint64_t)data[offset + 7] << 56);
        return PACK_SIZE_UINT64_T;
    }
    return 0;
}

size_t le_pack_s8(int8_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT8_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        return PACK_SIZE_INT8_T;
    }
    return 0;
}

size_t le_unpack_s8(int8_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT8_T, data_len))
    {
        *element = ((int8_t)data[offset + 0] << 0);
        return PACK_SIZE_INT8_T;
    }
    return 0;
}

size_t le_pack_s16(int16_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT16_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        data[offset + 1] = (uint8_t)((*element >> 8) & 0xFF);
        return PACK_SIZE_INT16_T;
    }
    return 0;
}

size_t le_unpack_s16(int16_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT16_T, data_len))
    {
        *element = ((int16_t)data[offset + 0] << 0) + 
                   ((int16_t)data[offset + 1] << 8);
        return PACK_SIZE_INT16_T;
    }
    return 0;
}

size_t le_pack_s32(int32_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT32_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        data[offset + 1] = (uint8_t)((*element >> 8) & 0xFF);
        data[offset + 2] = (uint8_t)((*element >> 16) & 0xFF);
        data[offset + 3] = (uint8_t)((*element >> 24) & 0xFF);
        return PACK_SIZE_INT32_T;
    }
    return 0;
}

size_t le_unpack_s32(int32_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT32_T, data_len))
    {
        *element = ((int32_t)data[offset + 0] << 0) + 
                   ((int32_t)data[offset + 1] << 8) +
                   ((int32_t)data[offset + 2] << 16) +
                   ((int32_t)data[offset + 3] << 24);
        return PACK_SIZE_INT32_T;
    }
    return 0;
}

size_t le_pack_s64(int64_t const * element, size_t offset, uint8_t * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT64_T, data_len))
    {
        data[offset + 0] = (uint8_t)((*element >> 0) & 0xFF);
        data[offset + 1] = (uint8_t)((*element >> 8) & 0xFF);
        data[offset + 2] = (uint8_t)((*element >> 16) & 0xFF);
        data[offset + 3] = (uint8_t)((*element >> 24) & 0xFF);
        data[offset + 4] = (uint8_t)((*element >> 32) & 0xFF);
        data[offset + 5] = (uint8_t)((*element >> 40) & 0xFF);
        data[offset + 6] = (uint8_t)((*element >> 48) & 0xFF);
        data[offset + 7] = (uint8_t)((*element >> 56) & 0xFF);
        return PACK_SIZE_INT64_T;
    }
    return 0;
}

size_t le_unpack_s64(int64_t * element, size_t offset, uint8_t const * data, size_t data_len)
{
    if (pack_is_within_buffer(offset, PACK_SIZE_INT64_T, data_len))
    {
        *element = ((int64_t)data[offset + 0] << 0) + 
                   ((int64_t)data[offset + 1] << 8) +
                   ((int64_t)data[offset + 2] << 16) +
                   ((int64_t)data[offset + 3] << 24) +
                   ((int64_t)data[offset + 4] << 32) +
                   ((int64_t)data[offset + 5] << 40) +
                   ((int64_t)data[offset + 6] << 48) +
                   ((int64_t)data[offset + 7] << 56);
        return PACK_SIZE_INT64_T;
    }
    return 0;
}