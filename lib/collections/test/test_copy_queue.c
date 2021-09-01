#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "copy_queue.h"
#include "test_copy_queue.h"

#include <string.h>

#define ITEMS_IN_TEST_QUEUE (15)

static void test_correct_fifo_ordering(void ** state)
{
    (void)state;

    static int queue_buffer[ITEMS_IN_TEST_QUEUE];

    CopyQueueConfig_t config = {
        .element_count = ITEMS_IN_TEST_QUEUE,
        .element_size = sizeof(int),
        .queue_buffer = (uint8_t *)queue_buffer,
        .queue_size = sizeof(queue_buffer),
    };

    CopyQueue_t queue;

    ErrorCode_t result;

    result = copy_queue_init(&queue, &config);
    assert_int_equal(ERR_NONE, result);

    // we inject in 15 items, starting from 0, to 15, note the variables to use is scoped

    for (int i = 0; i < ITEMS_IN_TEST_QUEUE; ++i)
    {
        result = copy_queue_enqueue(&queue, &i);
        assert_int_equal(ERR_NONE, result);

        size_t items_in_queue = copy_queue_get_size(&queue);
        size_t items_remaining = copy_queue_get_remaining(&queue);

        assert_int_equal(i+1, items_in_queue);
        assert_int_equal(ITEMS_IN_TEST_QUEUE-(i+1), items_remaining);
    }

    // just allocate a large array of items here, just to be sure
    {
        int queue_buffer[ITEMS_IN_TEST_QUEUE];
        memset(queue_buffer, 0xFF, sizeof(queue_buffer));
    }

    int dummy = 16;
    result = copy_queue_enqueue(&queue, &dummy);
    assert_int_equal(ERR_NO_MEM, result);

    // to check if it is copied, we ensure the values are retained even after we move out of scope

    for (int i = 0; i < ITEMS_IN_TEST_QUEUE; ++i)
    {
        int output = i+1;
        result = copy_queue_peek(&queue, &output);
        assert_int_equal(ERR_NONE, result);
        assert_int_equal(i, output);

        result = copy_queue_dequeue(&queue, &output);
        assert_int_equal(ERR_NONE, result);
        assert_int_equal(i, output);
    }

    result = copy_queue_peek(&queue, &dummy);
    assert_int_equal(ERR_EMPTY, result);

    result = copy_queue_dequeue(&queue, &dummy);
    assert_int_equal(ERR_EMPTY, result);

}

static void test_bad_config(void ** state)
{
    (void)state;

    static int queue_buffer[ITEMS_IN_TEST_QUEUE];
    CopyQueueConfig_t base_config = {
        .element_count = ITEMS_IN_TEST_QUEUE,
        .element_size = sizeof(int),
        .queue_buffer = (uint8_t *)queue_buffer,
        .queue_size = sizeof(queue_buffer),
    };
    CopyQueueConfig_t test_config = base_config;
    CopyQueue_t queue;
    ErrorCode_t result;

    // we define as having 1 more item in the queue than the buffer will allow
    test_config = base_config;
    test_config.element_count = ITEMS_IN_TEST_QUEUE + 1;
    result = copy_queue_init(&queue, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

    test_config = base_config;
    test_config.queue_size = 1;
    result = copy_queue_init(&queue, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

    test_config = base_config;
    test_config.element_size = sizeof(int) + 1;
    result = copy_queue_init(&queue, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

    test_config = base_config;
    test_config.queue_size = 0;
    result = copy_queue_init(&queue, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

    test_config = base_config;
    test_config.element_size = 0;
    result = copy_queue_init(&queue, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

    test_config = base_config;
    test_config.element_count = 0;
    result = copy_queue_init(&queue, &test_config);
    assert_int_equal(ERR_INVALID_ARG, result);

}

static void test_deinit_prevents_actions(void ** state)
{
    (void)state;
    
    CopyQueue_t queue;
    int dummy;
    ErrorCode_t result;
    copy_queue_deinit(&queue);

    result = copy_queue_enqueue(&queue, &dummy);
    assert_int_equal(ERR_NO_MEM, result);

    result = copy_queue_peek(&queue, &dummy);
    assert_int_equal(ERR_EMPTY, result);

    result = copy_queue_dequeue(&queue, &dummy);
    assert_int_equal(ERR_EMPTY, result);

    size_t count = copy_queue_get_remaining(&queue);
    assert_int_equal(0, count);

    count = copy_queue_get_size(&queue);
    assert_int_equal(0, count);
}

int test_copy_queue_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_correct_fifo_ordering),
        cmocka_unit_test(test_bad_config),
        cmocka_unit_test(test_deinit_prevents_actions),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}