#include "test_vector3_example.h"

void test_vector3_get_le_packable_interface(packable_t * packable)
{
    if (packable != NULL)
    {
        packable->encode = (packable_encode_handler_t)test_vector3_le_pack;
        packable->decode = (packable_decode_handler_t)test_vector3_le_unpack;
    }
}

size_t test_vector3_le_pack(test_vector3_t const * element, size_t offset, uint8_t * buffer, size_t buffer_size)
{
    size_t running_offset = offset; 

    if (pack_is_within_buffer(offset, PACK_SIZE_TEST_VECTOR3, buffer_size))
    {
        running_offset += le_pack_u32(&element->x, running_offset, buffer, buffer_size);
        running_offset += le_pack_u32(&element->y, running_offset, buffer, buffer_size);
        running_offset += le_pack_u32(&element->z, running_offset, buffer, buffer_size);
    }
    return ((running_offset - offset) == PACK_SIZE_TEST_VECTOR3) ? PACK_SIZE_TEST_VECTOR3 : 0;
}

size_t test_vector3_le_unpack(test_vector3_t * element, size_t offset, uint8_t const * buffer, size_t buffer_size)
{
    size_t running_offset = offset; 

    if (pack_is_within_buffer(offset, PACK_SIZE_TEST_VECTOR3, buffer_size))
    {
        running_offset += le_unpack_u32(&element->x, running_offset, buffer, buffer_size);
        running_offset += le_unpack_u32(&element->y, running_offset, buffer, buffer_size);
        running_offset += le_unpack_u32(&element->z, running_offset, buffer, buffer_size);
    }
    return ((running_offset - offset) == PACK_SIZE_TEST_VECTOR3) ? PACK_SIZE_TEST_VECTOR3 : 0;
}
