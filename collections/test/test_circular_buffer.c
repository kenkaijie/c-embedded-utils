#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "circular_buffer.h"
#include "test_circular_buffer.h"

#include <string.h>

#define ITEMS_IN_TEST_BUFFER (15)

static void test_interface_nulls(void ** state)
{
    error_t result;
    circular_buffer_t circular;

    result = circular_buffer_init(&circular, NULL);
    assert_int_equal(ERR_NULL_POINTER, result);

    result = circular_buffer_pop_byte(&circular, NULL);
    assert_int_equal(ERR_NULL_POINTER, result);
}

static void test_correct_ordering_with_overflow(void ** state)
{
    static uint8_t buffer[ITEMS_IN_TEST_BUFFER];
    circular_buffer_config_t config = {
        .buffer = buffer,
        .buffer_size = ITEMS_IN_TEST_BUFFER,
    };
    circular_buffer_t circular;
    error_t result;

    result = circular_buffer_init(&circular, &config);
    assert_int_equal(ERR_NONE, result);

    for (uint8_t idx = 0; idx < ITEMS_IN_TEST_BUFFER; ++idx)
    {
        result = circular_buffer_push_byte(&circular, idx);
        assert_int_equal(ERR_NONE, result);

        size_t buffer_size = circular_buffer_get_size(&circular);
        assert_int_equal(idx + 1, buffer_size);
    }

    // buffer should have 0...14

    uint8_t test_byte = ITEMS_IN_TEST_BUFFER;
    result = circular_buffer_push_byte(&circular, test_byte);
    assert_int_equal(ERR_OVERRIDE, result);

    result = circular_buffer_push_byte(&circular, test_byte + 1);
    assert_int_equal(ERR_OVERRIDE, result);

    // override 0 with 15, so the buffer is now 2...16

    // pop out 5 items, should be 2, 3, 4, 5, 6
    for (uint8_t idx = 0; idx < 5; ++idx)
    {
        uint8_t byte_out;
        result = circular_buffer_pop_byte(&circular, &byte_out);
        assert_int_equal(ERR_NONE, result);
        assert_int_equal(byte_out, idx + 2);
    }

    // push in 3 items 17.. 19
    for (uint8_t idx = 0; idx < 3; ++idx)
    {
        result = circular_buffer_push_byte(&circular, test_byte + 2 + idx);
        assert_int_equal(ERR_NONE, result);
    }

    // pop out the rest, 7 ... 19
    size_t items_in_buffer = circular_buffer_get_size(&circular);
    assert_int_equal(13, items_in_buffer);

    for (uint8_t idx = 0; idx < 13; ++idx)
    {
        uint8_t byte_out;
        result = circular_buffer_pop_byte(&circular, &byte_out);
        assert_int_equal(ERR_NONE, result);
        assert_int_equal(byte_out, idx + 7);
    }

    uint8_t byte_out;
    result = circular_buffer_pop_byte(&circular, &byte_out);
    assert_int_equal(ERR_EMPTY, result);
    
}

static void test_bad_config(void ** state)
{
    static uint8_t buffer[ITEMS_IN_TEST_BUFFER];
    circular_buffer_config_t config = {
        .buffer = buffer,
        .buffer_size = ITEMS_IN_TEST_BUFFER,
    };
    circular_buffer_t circular;
    error_t result;
    circular_buffer_config_t test_config = config;

    test_config = config;
    test_config.buffer = NULL;
    result = circular_buffer_init(&circular, &test_config);
    assert_int_equal(ERR_NULL_POINTER, result);

    test_config = config;
    test_config.buffer_size = 0;
    result = circular_buffer_init(&circular, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);
}

static void test_deinit_prevents_actions(void ** state)
{
    static uint8_t buffer[ITEMS_IN_TEST_BUFFER];
    circular_buffer_config_t config = {
        .buffer = buffer,
        .buffer_size = ITEMS_IN_TEST_BUFFER,
    };
    circular_buffer_t circular;
    error_t result;
    uint8_t dummy;

    circular_buffer_deinit(&circular);

    result = circular_buffer_push_byte(&circular, dummy);
    assert_int_equal(ERR_NO_MEM, result);

    // pop out the rest, 7 ... 19
    size_t items_in_buffer = circular_buffer_get_size(&circular);
    assert_int_equal(0, items_in_buffer);

    result = circular_buffer_pop_byte(&circular, &dummy);
    assert_int_equal(ERR_EMPTY, result);

}

int test_circular_buffer_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_interface_nulls),
        cmocka_unit_test(test_correct_ordering_with_overflow),
        cmocka_unit_test(test_bad_config),
        cmocka_unit_test(test_deinit_prevents_actions),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}