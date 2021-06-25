#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "static_pool.h"

typedef struct s_test_static_pool_dummy_data test_static_pool_dummy_data_t;

struct s_test_static_pool_dummy_data
{
    uint8_t param_a;
    uint32_t param_b;
    uint8_t param_c;
    uint16_t param_d;
};

#define TEST_OBJECT_COUNT (16)
#define TEST_OBJECT_SIZE_BYTES (sizeof(test_static_pool_dummy_data_t))

static void test_correct_interface(void ** state)
{
    error_t ret;
    size_t const object_size = sizeof(test_static_pool_dummy_data_t);
    size_t const object_count = 16;
    uint8_t buffer[object_size * object_count];
    void * allocation_stack[object_count];
    test_static_pool_dummy_data_t * object_pointer;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .allocation_stack = allocation_stack,
        .object_count = object_count,
        .object_size = object_size, 
    };

    static_pool_t pool;
    object_pool_t interface;

    ret = static_pool_as_object_pool(&pool, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NONE, ret);
    
    ret = static_pool_as_object_pool(&pool, &interface);
    assert_int_equal(ERR_NONE, ret);

    assert_ptr_equal(interface.context, &pool);
    assert_ptr_equal(interface.allocate, static_pool_allocate);
    assert_ptr_equal(interface.deallocate, static_pool_deallocate);
    assert_ptr_equal(interface.get_available_count, static_pool_get_available_count);
}


static void test_null_checks(void ** state)
{
    error_t ret;
    size_t const object_size = sizeof(test_static_pool_dummy_data_t);
    size_t const object_count = 16;
    uint8_t buffer[object_size * object_count];
    void * allocation_stack[object_count];
    test_static_pool_dummy_data_t * object_pointer;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .allocation_stack = allocation_stack,
        .object_count = object_count,
        .object_size = object_size, 
    };

    static_pool_t pool;

    ret = static_pool_init(&pool, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_allocate(&pool, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_deallocate(&pool, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);
}

static void test_invalid_configs(void ** state)
{
    error_t ret;
    uint8_t buffer[TEST_OBJECT_SIZE_BYTES * TEST_OBJECT_COUNT] = {};
    void * allocation_stack[TEST_OBJECT_COUNT] = {};
    test_static_pool_dummy_data_t * object_pointer;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .allocation_stack = allocation_stack,
        .object_count = TEST_OBJECT_COUNT,
        .object_size = TEST_OBJECT_SIZE_BYTES,
    };

    static_pool_t pool;

    config.buffer = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    config.buffer = buffer;

    config.allocation_stack = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    config.allocation_stack = allocation_stack;

    config.buffer = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    config.buffer = buffer;

    config.buffer = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    config.buffer = buffer;

    // check sizing requirements (buffer too small, a buffer biugger than the memory size is ok.)
    config.buffer_size = TEST_OBJECT_COUNT * TEST_OBJECT_SIZE_BYTES - 1;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_INVALID_ARG, ret);
    config.buffer_size = sizeof(buffer);
}

static void test_deallocate_twice_fails(void ** state)
{
 error_t ret;
    uint8_t buffer[TEST_OBJECT_SIZE_BYTES * TEST_OBJECT_COUNT] = {};
    void * allocation_stack[TEST_OBJECT_COUNT] = {};
    void * token1 = NULL;
    void * token2 = NULL;
    test_static_pool_dummy_data_t * object_pointer = NULL;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .allocation_stack = allocation_stack,
        .object_count = TEST_OBJECT_COUNT,
        .object_size = TEST_OBJECT_SIZE_BYTES, 
    };

    static_pool_t pool;

    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = static_pool_allocate(&pool, &token1);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_not_equal(NULL, token1);

    ret = static_pool_allocate(&pool, &token2);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_not_equal(NULL, token2);

    ret = static_pool_deallocate(&pool, &token1);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal(NULL, token1);

    ret = static_pool_deallocate(&pool, &token1);
    assert_int_equal(ERR_OUT_OF_BOUNDS, ret);

    ret = static_pool_deallocate(&pool, &token2);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal(NULL, token2);

    ret = static_pool_deallocate(&pool, &token2);
    assert_int_equal(ERR_OUT_OF_BOUNDS, ret);
}

static void test_allocation_fills_and_unfills_correctly(void ** state)
{
    error_t ret;
    uint8_t buffer[TEST_OBJECT_SIZE_BYTES * TEST_OBJECT_COUNT] = {};
    void * allocation_stack[TEST_OBJECT_COUNT] = {};
    void * object_pointers[TEST_OBJECT_COUNT] = {};
    void * overrun_obj_ptr = NULL;
    size_t slots_remaining = 0;
    for (size_t idx = 0; idx < TEST_OBJECT_COUNT; ++idx)
    {
        object_pointers[idx] = NULL;
    }

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .allocation_stack = allocation_stack,
        .object_count = TEST_OBJECT_COUNT,
        .object_size = TEST_OBJECT_SIZE_BYTES, 
    };

    static_pool_t pool;

    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NONE, ret);

    for(size_t idx = 0; idx < TEST_OBJECT_COUNT; ++idx)
    {
        ret = static_pool_allocate(&pool, &object_pointers[idx]);
        assert_int_equal(ERR_NONE, ret);

        slots_remaining = static_pool_get_available_count(&pool);
        assert_int_equal(TEST_OBJECT_COUNT - 1 - idx, slots_remaining);
    }

    ret = static_pool_allocate(&pool, &overrun_obj_ptr);
    assert_int_equal(ERR_NO_MEM, ret);

    // we validate the positions of all the object pointers. To pass this, we just check if all the pointers are at least sizeof(test_static_pool_dummy_data_t) bytes apart
    // for now it is a very inefficient check. We also check if all tokens are within the array space of the buffer provided (we don't allocate outside the provided buffer space)
    for (size_t idx_i = 0; idx_i < TEST_OBJECT_COUNT; ++idx_i)
    {
        assert_true(object_pointers[idx_i] != NULL);
        assert_true((void *)&buffer[0] <= (void *)object_pointers[idx_i]);
        assert_true((void *)object_pointers[idx_i] <= ((void *)buffer + sizeof(buffer) - TEST_OBJECT_SIZE_BYTES));
        for (size_t idx_j = 0; idx_j < TEST_OBJECT_COUNT; ++idx_j)
        {
            if (idx_i != idx_j)
            {
                assert_true((ptrdiff_t)object_pointers[idx_i] - (ptrdiff_t)object_pointers[idx_j] >= sizeof(test_static_pool_dummy_data_t));
            }
        }
    }

    for(size_t idx = 0; idx < TEST_OBJECT_COUNT; ++idx)
    {
        ret = static_pool_deallocate(&pool, &object_pointers[idx]);
        assert_int_equal(ERR_NONE, ret);
        assert_ptr_equal(NULL, object_pointers[idx]);

        slots_remaining = static_pool_get_available_count(&pool);
        assert_int_equal(idx + 1, slots_remaining);
    }

    static_pool_deinit(&pool);
}

int test_static_pool_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_correct_interface),
        cmocka_unit_test(test_null_checks),
        cmocka_unit_test(test_invalid_configs),
        cmocka_unit_test(test_allocation_fills_and_unfills_correctly),
        cmocka_unit_test(test_deallocate_twice_fails),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}