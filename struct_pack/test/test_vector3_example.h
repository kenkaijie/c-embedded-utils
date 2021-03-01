#pragma once

#include "struct_pack.h"

typedef struct 
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
} test_vector3_t;

#define PACK_SIZE_TEST_VECTOR3 (12)

void test_vector3_get_le_packable_interface(packable_t * packable);

size_t test_vector3_le_pack(test_vector3_t const * element, size_t offset, uint8_t * buffer, size_t buffer_size);
size_t test_vector3_le_unpack(test_vector3_t * element, size_t offset, uint8_t const * buffer, size_t buffer_size);