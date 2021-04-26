#include "pack.h"

bool pack_is_within_buffer(size_t offset, size_t req_size, size_t data_len)
{
    size_t required_index = offset + req_size;
    // if the required index is less than the offset, we've looped and this MUST fail
    return (required_index < offset) ? false : required_index <= data_len;
}
