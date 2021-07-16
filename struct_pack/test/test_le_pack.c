#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_le_pack.h"
#include "le_pack.h"

static void test_le_test_rig(pack_serialise_func_t encode, pack_deserialise_func_t decode, size_t expected_type_size, size_t test_offset, void const * expected_value, void * actual_value, uint8_t const * expected_buf, uint8_t * actual_buf, size_t buffer_size)
{
    // we set ret to initially 1, so it will fail unless we set it to 0
    size_t bytes_consumed;
    bytes_consumed = encode(expected_value, buffer_size, actual_buf, buffer_size);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = decode(actual_value, buffer_size, actual_buf, buffer_size);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = encode(expected_value, test_offset, actual_buf, expected_type_size-1);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = decode(actual_value, test_offset, actual_buf, expected_type_size-1);
    assert_int_equal(0, bytes_consumed);

    // do single run
    bytes_consumed = encode(expected_value, test_offset, actual_buf, buffer_size);
    assert_int_equal(expected_type_size, bytes_consumed);

    bytes_consumed = decode(actual_value, test_offset, actual_buf, buffer_size);
    assert_int_equal(expected_type_size, bytes_consumed);
}

/**
 *  @brief  Each type tests for boundary overruns on pack, unpack, 
 *          as well as just a general check that it writes to the buffer properly.
 */
static void test_le_u8(void ** state)
{
    size_t expected_type_size = PACK_SIZE_UINT8_T;
    uint8_t expected_value = 0x12;
    uint8_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[5] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x12, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_u8, (pack_deserialise_func_t)le_unpack_u8, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_u16(void ** state)
{
    size_t expected_type_size = PACK_SIZE_UINT16_T;
    uint16_t expected_value = 0x1234;
    uint16_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[5] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x34, 0x12};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_u16, (pack_deserialise_func_t)le_unpack_u16, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_u32(void ** state)
{
    size_t expected_type_size = PACK_SIZE_UINT32_T;
    uint32_t expected_value = 0x12345678U;
    uint32_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x78, 0x56, 0x34, 0x12, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_u32, (pack_deserialise_func_t)le_unpack_u32, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_u64(void ** state)
{
    size_t expected_type_size = PACK_SIZE_UINT64_T;
    uint64_t expected_value = 0x12345678ABCDEF92UL;
    uint64_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[15] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] =  {0xFF, 0xFF, 0xFF, 0x92, 0xEF, 0xCD, 0xAB, 0x78, 0x56, 0x34, 0x12, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_u64, (pack_deserialise_func_t)le_unpack_u64, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}


static void test_le_s8(void ** state)
{
    size_t expected_type_size = PACK_SIZE_INT8_T;
    int8_t expected_value = 0x12;
    int8_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_s8, (pack_deserialise_func_t)le_unpack_s8, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_s16(void ** state)
{
    size_t expected_type_size = PACK_SIZE_INT16_T;
    int16_t expected_value = 0x83FE;
    int16_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0xFE, 0x83, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_s16, (pack_deserialise_func_t)le_unpack_s16, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_s32(void ** state)
{
    size_t expected_type_size = PACK_SIZE_INT32_T;
    int32_t expected_value = 0xFACD1234;
    int32_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x34, 0x12, 0xCD, 0xFA, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_s32, (pack_deserialise_func_t)le_unpack_s32, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_s64(void ** state)
{
    size_t expected_type_size = PACK_SIZE_INT64_T;
    int64_t expected_value = (int64_t)0x12345678ABCDEF92UL;
    int64_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[15] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] =  {0xFF, 0xFF, 0xFF, 0x92, 0xEF, 0xCD, 0xAB, 0x78, 0x56, 0x34, 0x12, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_s64, (pack_deserialise_func_t)le_unpack_s64, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_bool(void ** state)
{
    size_t expected_type_size = PACK_SIZE_BOOL;
    bool expected_value = true;
    bool actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    test_le_test_rig((pack_serialise_func_t)le_pack_bool, (pack_deserialise_func_t)le_unpack_bool, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_raw(void ** state)
{
    uint8_t raw_buffer[7] =                         {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE};
    uint8_t actual_buffer[sizeof(raw_buffer)] =     {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(raw_buffer)] =   {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE};
    uint8_t decoded_buffer[sizeof(raw_buffer)] =    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    // this should fail, as the buffers are exact sizes
    size_t bytes_consumed;
    bytes_consumed = le_pack_raw(raw_buffer, sizeof(raw_buffer), 1, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = le_unpack_raw(raw_buffer, sizeof(raw_buffer), 1, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = le_pack_raw(raw_buffer, sizeof(raw_buffer), 0, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(sizeof(raw_buffer), bytes_consumed);
    assert_memory_equal(expected_buffer, actual_buffer, sizeof(actual_buffer));

    bytes_consumed = le_unpack_raw(decoded_buffer, sizeof(raw_buffer), 0, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(sizeof(raw_buffer), bytes_consumed);
    assert_memory_equal(expected_buffer, decoded_buffer, sizeof(decoded_buffer));

    // buffer too small
    bytes_consumed = le_pack_raw(decoded_buffer, sizeof(raw_buffer), 0, actual_buffer, 0);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = le_unpack_raw(decoded_buffer, sizeof(raw_buffer), 0, actual_buffer, 0);
    assert_int_equal(0, bytes_consumed);

    // raw data size is 0
    bytes_consumed = le_pack_raw(raw_buffer, 0, 0, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = le_unpack_raw(decoded_buffer, 0, 0, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(0, bytes_consumed);

    // both
    bytes_consumed = le_pack_raw(raw_buffer, 0, 0, actual_buffer, 0);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = le_unpack_raw(decoded_buffer, 0, 0, actual_buffer, 0);
    assert_int_equal(0, bytes_consumed);
}

int test_le_pack_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_le_u8),
        cmocka_unit_test(test_le_u16),
        cmocka_unit_test(test_le_u32),
        cmocka_unit_test(test_le_u64),
        cmocka_unit_test(test_le_s8),
        cmocka_unit_test(test_le_s16),
        cmocka_unit_test(test_le_s32),
        cmocka_unit_test(test_le_s64),
        cmocka_unit_test(test_le_bool),
        cmocka_unit_test(test_le_raw),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}