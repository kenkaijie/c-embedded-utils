#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <cemb/fast_circular_buffer.h>
#include "test_fast_circular_buffer.h"

#include <string.h>

#define ITEMS_IN_TEST_BUFFER (16)
#define ITEMS_IN_RIPPLE_INSERT (16)

static void test_ripple_insert(void ** state)
{
    static uint8_t buffer[ITEMS_IN_RIPPLE_INSERT];
    FastCircularBufferConfig_t config = {
        .buffer = buffer,
        .buffer_size = ITEMS_IN_RIPPLE_INSERT,
    };
    FastCircularBuffer_t circular;
    ErrorCode_t result;

    result = fast_circular_buffer_init(&circular, &config);
    assert_int_equal(ERR_NONE, result);

    // we push 1, and pop 2 (second should always fail)
    for (uint8_t cycle = 0; cycle < 30; ++cycle)
    {
        result = fast_circular_buffer_push_byte(&circular, cycle);
        assert_int_equal(ERR_NONE, result);

        uint8_t byte_out;
        result = fast_circular_buffer_pop_byte(&circular, &byte_out);
        assert_int_equal(ERR_NONE, result);
        assert_int_equal(byte_out, cycle);

        result = fast_circular_buffer_pop_byte(&circular, &byte_out);
        assert_int_equal(ERR_EMPTY, result);
    }
}

/**
 * We push at least a full buffer, then we check that any subsequent writes will override the oldest value.
 */
static void test_correct_ordering_with_overflow(void ** state)
{
    static uint8_t buffer[ITEMS_IN_TEST_BUFFER];
    FastCircularBufferConfig_t config = {
        .buffer = buffer,
        .buffer_size = ITEMS_IN_TEST_BUFFER,
    };
    FastCircularBuffer_t circular;
    ErrorCode_t result;

    result = fast_circular_buffer_init(&circular, &config);
    assert_int_equal(ERR_NONE, result);

    /* Populate buffer to full */
    for (uint8_t idx = 0; idx < ITEMS_IN_TEST_BUFFER; ++idx)
    {
        result = fast_circular_buffer_push_byte(&circular, idx);
        assert_int_equal(ERR_NONE, result);

        size_t buffer_size = fast_circular_buffer_get_count(&circular);
        assert_int_equal(idx + 1, buffer_size);
    }

    size_t buffer_size = fast_circular_buffer_get_count(&circular);
    assert_int_equal(ITEMS_IN_TEST_BUFFER, buffer_size);

    for (uint8_t idx = ITEMS_IN_TEST_BUFFER; idx < (ITEMS_IN_TEST_BUFFER * 2); ++idx)
    {
        result = fast_circular_buffer_push_byte(&circular, idx);
        assert_int_equal(ERR_NONE, result);

        buffer_size = fast_circular_buffer_get_count(&circular);
        assert_int_equal(ITEMS_IN_TEST_BUFFER, buffer_size);
    }

    // buffer should have ITEMS_IN_TEST_BUFFER ... (ITEMS_IN_TEST_BUFFER * 2) -1

    // pop out 5 items, should be ITEMS_IN_TEST_BUFFER + 0, 1, 2, 3, 4 (will have ITEMS_IN_TEST_BUFFER - 5 left)
    for (uint8_t idx = 0; idx < 5; ++idx)
    {
        uint8_t byte_out;
        result = fast_circular_buffer_pop_byte(&circular, &byte_out);
        assert_int_equal(ERR_NONE, result);
        assert_int_equal(byte_out, idx + ITEMS_IN_TEST_BUFFER);
    }

    size_t items_in_buffer = fast_circular_buffer_get_count(&circular);
    assert_int_equal(ITEMS_IN_TEST_BUFFER - 5, items_in_buffer);

    /* Pop the rest, which should be  (ITEMS_IN_TEST_BUFFER + 5) ... (ITEMS_IN_TEST_BUFFER * 2 - 1)*/
    for (uint8_t idx = (ITEMS_IN_TEST_BUFFER + 5); idx < (ITEMS_IN_TEST_BUFFER * 2); ++idx)
    {
        uint8_t byte_out;
        result = fast_circular_buffer_pop_byte(&circular, &byte_out);
        assert_int_equal(ERR_NONE, result);
        assert_int_equal(byte_out, idx);
    }

    uint8_t byte_out;
    result = fast_circular_buffer_pop_byte(&circular, &byte_out);
    assert_int_equal(ERR_EMPTY, result);
    
}

static void test_bad_config(void ** state)
{
    static uint8_t buffer[ITEMS_IN_TEST_BUFFER];
    FastCircularBufferConfig_t config = {
        .buffer = buffer,
        .buffer_size = ITEMS_IN_TEST_BUFFER,
    };
    FastCircularBuffer_t circular;
    ErrorCode_t result;
    FastCircularBufferConfig_t test_config = config;

    test_config = config;
    test_config.buffer_size = 0;
    result = fast_circular_buffer_init(&circular, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

    test_config = config;
    test_config.buffer_size = 23; /* not a power of 2 */
    result = fast_circular_buffer_init(&circular, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

    test_config = config;
    test_config.buffer_size = 1; /* must be greater or equal to 2 */
    result = fast_circular_buffer_init(&circular, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);
}

static void test_deinit_prevents_actions(void ** state)
{
    static uint8_t buffer[ITEMS_IN_TEST_BUFFER];
    FastCircularBufferConfig_t config = {
        .buffer = buffer,
        .buffer_size = ITEMS_IN_TEST_BUFFER,
    };
    FastCircularBuffer_t circular;
    ErrorCode_t result;
    uint8_t dummy = 0;

    fast_circular_buffer_deinit(&circular);

    result = fast_circular_buffer_push_byte(&circular, dummy);
    assert_int_equal(ERR_NO_MEM, result);

    // pop out the rest, 7 ... 19
    size_t items_in_buffer = fast_circular_buffer_get_count(&circular);
    assert_int_equal(0, items_in_buffer);

    result = fast_circular_buffer_pop_byte(&circular, &dummy);
    assert_int_equal(ERR_EMPTY, result);

}

int test_fast_circular_buffer_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ripple_insert),
        cmocka_unit_test(test_correct_ordering_with_overflow),
        cmocka_unit_test(test_bad_config),
        cmocka_unit_test(test_deinit_prevents_actions),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}