#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "struct_pack.h"
#include "test_vector3_example.h"

/**
 *  @note   See bottom of file for a simple example using a made up vector 3 type
 */

/**
 *  @brief  Tests boundaries for struct pack system. This is used in all primative packs, so it is tested first.
 */
static void test_overrun_checker(void ** state)
{
    assert_true(pack_is_within_buffer(0, 0, 0));
    assert_true(pack_is_within_buffer(0, 0, 1));
    assert_false(pack_is_within_buffer(0, 1, 0));
    assert_true(pack_is_within_buffer(0, 1, 1));
    assert_false(pack_is_within_buffer(1, 0, 0));
    assert_true(pack_is_within_buffer(1, 0, 1));
    assert_false(pack_is_within_buffer(1, 1, 0));
    assert_false(pack_is_within_buffer(1, 1, 1));

    // edge case testing for SIZE_MAX overruns, SIZE_MAX defines the largest array possible.
    assert_false(pack_is_within_buffer(SIZE_MAX, 1, SIZE_MAX));
}

static void _test_le_test_rig(packable_t * packable, size_t expected_type_size, size_t test_offset, void const * expected_value, void * actual_value, uint8_t const * expected_buf, uint8_t * actual_buf, size_t buffer_size)
{
    // we set ret to initially 1, so it will fail unless we set it to 0
    size_t bytes_consumed = 1;
    bytes_consumed = packable->encode(expected_value, buffer_size, actual_buf, buffer_size);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = 1;
    bytes_consumed = packable->decode(actual_value, buffer_size, actual_buf, buffer_size);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = 1;
    bytes_consumed = packable->encode(expected_value, test_offset, actual_buf, expected_type_size-1);
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = 1;
    bytes_consumed = packable->decode(actual_value, test_offset, actual_buf, expected_type_size-1);
    assert_int_equal(0, bytes_consumed);

    // do single run

    bytes_consumed = 0;
    bytes_consumed = packable->encode(expected_value, test_offset, actual_buf, buffer_size);
    assert_int_equal(expected_type_size, bytes_consumed);

    bytes_consumed = 0;
    bytes_consumed = packable->decode(actual_value, test_offset, actual_buf, buffer_size);
    assert_int_equal(expected_type_size, bytes_consumed);
}

/**
 *  @brief  Each type tests for boundary overruns on pack, unpack, 
 *          as well as just a general check that it writes to the buffer properly.
 */
static void test_le_u8(void ** state)
{
    packable_t packer = {
        .encode = (packable_encode_handler_t)le_pack_u8,
        .decode = (packable_decode_handler_t)le_unpack_u8,
    };
    size_t expected_type_size = PACK_SIZE_UINT8_T;
    uint8_t expected_value = 0x12;
    uint8_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[5] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x12, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_u16(void ** state)
{
    packable_t packer = {
        .encode = (packable_encode_handler_t)le_pack_u16,
        .decode = (packable_decode_handler_t)le_unpack_u16,
    };
    size_t expected_type_size = PACK_SIZE_UINT16_T;
    uint16_t expected_value = 0x1234;
    uint16_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[5] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x34, 0x12};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_u32(void ** state)
{
    packable_t packer = {
        .encode = (packable_encode_handler_t)le_pack_u32,
        .decode = (packable_decode_handler_t)le_unpack_u32,
    };
    size_t expected_type_size = PACK_SIZE_UINT32_T;
    uint32_t expected_value = 0x12345678U;
    uint32_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x78, 0x56, 0x34, 0x12, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_s8(void ** state)
{
    packable_t packer = {
        .encode = (packable_encode_handler_t)le_pack_s8,
        .decode = (packable_decode_handler_t)le_unpack_s8,
    };
    size_t expected_type_size = PACK_SIZE_INT8_T;
    int8_t expected_value = 0x12;
    int8_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_s16(void ** state)
{
    packable_t packer = {
        .encode = (packable_encode_handler_t)le_pack_s16,
        .decode = (packable_decode_handler_t)le_unpack_s16,
    };
    size_t expected_type_size = PACK_SIZE_INT16_T;
    int16_t expected_value = 0x83FE;
    int16_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0xFE, 0x83, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_s32(void ** state)
{
    packable_t packer = {
        .encode = (packable_encode_handler_t)le_pack_s32,
        .decode = (packable_decode_handler_t)le_unpack_s32,
    };
    size_t expected_type_size = PACK_SIZE_INT32_T;
    int32_t expected_value = 0xFACD1234;
    int32_t actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x34, 0x12, 0xCD, 0xFA, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_int_equal(expected_value, actual_value);
}

static void test_le_bool(void ** state)
{
    packable_t packer = {
        .encode = (packable_encode_handler_t)le_pack_bool,
        .decode = (packable_decode_handler_t)le_unpack_bool,
    };
    size_t expected_type_size = PACK_SIZE_BOOL;
    bool expected_value = true;
    bool actual_value = expected_value - 1;
    size_t offset = 3;
    uint8_t actual_buffer[9] =                       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
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
    size_t bytes_consumed = 1;
    bytes_consumed = le_pack_raw(raw_buffer, sizeof(raw_buffer), 1, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = 1;
    bytes_consumed = le_unpack_raw(raw_buffer, sizeof(raw_buffer), 1, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(0, bytes_consumed);

    bytes_consumed = 0;
    bytes_consumed = le_pack_raw(raw_buffer, sizeof(raw_buffer), 0, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(sizeof(raw_buffer), bytes_consumed);
    assert_memory_equal(expected_buffer, actual_buffer, sizeof(actual_buffer));

    bytes_consumed = 0;
    bytes_consumed = le_unpack_raw(decoded_buffer, sizeof(raw_buffer), 0, actual_buffer, sizeof(actual_buffer));
    assert_int_equal(sizeof(raw_buffer), bytes_consumed);
    assert_memory_equal(expected_buffer, decoded_buffer, sizeof(decoded_buffer));
}

static void test_vector3_le_example(void ** state)
{
    packable_t packer;

    test_vector3_get_le_packable_interface(&packer);
    assert_ptr_equal(test_vector3_le_pack, packer.encode);
    assert_ptr_equal(test_vector3_le_unpack, packer.decode);

    size_t expected_type_size = PACK_SIZE_TEST_VECTOR3;
    test_vector3_t expected_value = {
        .x = 0xAABBCCDD,
        .y = 0x11223344,
        .z = 0xDEADBEEF,
    };
    test_vector3_t actual_value;
    size_t offset = 3;
    uint8_t actual_buffer[] =                        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t expected_buffer[sizeof(actual_buffer)] = {0xFF, 0xFF, 0xFF, 0xDD, 0xCC, 0xBB, 0xAA, 0x44, 0x33, 0x22, 0x11, 0xEF, 0xBE, 0xAD, 0xDE, 0xFF};
    size_t buffer_size = sizeof(actual_buffer);
    size_t bytes_consumed = 0;

    _test_le_test_rig(&packer, expected_type_size, offset, &expected_value, &actual_value, expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(expected_buffer, actual_buffer, buffer_size);
    assert_memory_equal(&expected_value, &actual_value, sizeof(actual_value));
}

int test_struct_pack_run_tests(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_overrun_checker),
        cmocka_unit_test(test_le_u8),
        cmocka_unit_test(test_le_u16),
        cmocka_unit_test(test_le_u32),
        cmocka_unit_test(test_le_s8),
        cmocka_unit_test(test_le_s16),
        cmocka_unit_test(test_le_s32),
        cmocka_unit_test(test_le_bool),
        cmocka_unit_test(test_le_raw),
        cmocka_unit_test(test_vector3_le_example),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}