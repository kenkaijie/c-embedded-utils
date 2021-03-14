#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "static_pool.h"
#include "mock_critical_section.h"

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

static void test_null_checks(void ** state)
{
    error_t ret;
    size_t const object_size = sizeof(test_static_pool_dummy_data_t);
    size_t const object_count = 16;
    uint8_t buffer[object_size * object_count];
    static_metadata_t metadata_buffer[object_count];
    size_t token = OBJECT_POOL_TOKEN_INVALID;
    test_static_pool_dummy_data_t * object_pointer;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .use_lock = false,
        .metadata_buffer = metadata_buffer,
        .object_count = object_count,
        .object_size = object_size, 
    };

    static_pool_t pool;

    ret = static_pool_init(NULL, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_init(&pool, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_deinit(NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_allocate(NULL, &token);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_allocate(&pool, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_fetch(NULL, token, (void **) &object_pointer);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_fetch(&pool, token, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_deallocate(NULL, &token);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = static_pool_deallocate(&pool, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

}

static void test_token_out_of_bounds(void ** state)
{

}

static void test_invalid_configs(void ** state)
{
    error_t ret;
    uint8_t buffer[TEST_OBJECT_SIZE_BYTES * TEST_OBJECT_COUNT] = {};
    static_metadata_t metadata_buffer[TEST_OBJECT_COUNT] = {};
    size_t token = OBJECT_POOL_TOKEN_INVALID;
    test_static_pool_dummy_data_t * object_pointer;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .use_lock = false,
        .metadata_buffer = metadata_buffer,
        .object_count = TEST_OBJECT_COUNT,
        .object_size = TEST_OBJECT_SIZE_BYTES, 
    };

    static_pool_t pool;

    config.buffer = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    config.buffer = buffer;

    config.metadata_buffer = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    config.metadata_buffer = metadata_buffer;

    config.buffer = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);

    config.buffer = NULL;
    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);
}

static void test_deinit_invalid_states(void ** state)
{
    error_t ret;
    uint8_t buffer[TEST_OBJECT_SIZE_BYTES * TEST_OBJECT_COUNT] = {};
    static_metadata_t metadata_buffer[TEST_OBJECT_COUNT] = {};
    size_t token = OBJECT_POOL_TOKEN_INVALID;
    test_static_pool_dummy_data_t * object_pointer = NULL;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .use_lock = false,
        .metadata_buffer = metadata_buffer,
        .object_count = TEST_OBJECT_COUNT,
        .object_size = TEST_OBJECT_SIZE_BYTES, 
    };

    static_pool_t pool;

    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = static_pool_deinit(&pool);
    assert_int_equal(ERR_NONE, ret);

    ret = static_pool_allocate(&pool, &token);
    assert_int_equal(ERR_NOT_INITIALISED, ret);

    ret = static_pool_fetch(&pool, token, (void **)&object_pointer);
    assert_int_equal(ERR_NOT_INITIALISED, ret);

    ret = static_pool_deallocate(&pool, &token);
    assert_int_equal(ERR_NOT_INITIALISED, ret);

}

static void test_deallocate_twice_fails(void ** state)
{
 error_t ret;
    uint8_t buffer[TEST_OBJECT_SIZE_BYTES * TEST_OBJECT_COUNT] = {};
    static_metadata_t metadata_buffer[TEST_OBJECT_COUNT] = {};
    size_t token = OBJECT_POOL_TOKEN_INVALID;
    size_t backup_token = OBJECT_POOL_TOKEN_INVALID;
    test_static_pool_dummy_data_t * object_pointer = NULL;

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .use_lock = false,
        .metadata_buffer = metadata_buffer,
        .object_count = TEST_OBJECT_COUNT,
        .object_size = TEST_OBJECT_SIZE_BYTES, 
    };

    static_pool_t pool;

    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = static_pool_allocate(&pool, &token);
    assert_int_equal(ERR_NONE, ret);

    backup_token = token;

    ret = static_pool_deallocate(&pool, &token);
    assert_int_equal(ERR_NONE, ret);

    ret = static_pool_deallocate(&pool, &backup_token);
    assert_int_equal(ERR_INVALID_STATE, ret);
}

static void test_allocation_fills_and_unfills_correctly(void ** state)
{
    error_t ret;
    uint8_t buffer[TEST_OBJECT_SIZE_BYTES * TEST_OBJECT_COUNT] = {};
    static_metadata_t metadata_buffer[TEST_OBJECT_COUNT] = {};
    size_t tokens[TEST_OBJECT_COUNT] = {};
    size_t overrun_token = OBJECT_POOL_TOKEN_INVALID;
    test_static_pool_dummy_data_t * object_pointers[TEST_OBJECT_COUNT];
    size_t slots_remaining = 0;
    for (size_t idx = 0; idx < TEST_OBJECT_COUNT; ++idx)
    {
        tokens[idx] = OBJECT_POOL_TOKEN_INVALID;
        object_pointers[idx] = NULL;
    }

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .use_lock = false,
        .metadata_buffer = metadata_buffer,
        .object_count = TEST_OBJECT_COUNT,
        .object_size = TEST_OBJECT_SIZE_BYTES, 
    };

    static_pool_t pool;

    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NONE, ret);

    for(size_t idx = 0; idx < TEST_OBJECT_COUNT; ++idx)
    {
        ret = static_pool_allocate(&pool, &tokens[idx]);
        assert_int_equal(ERR_NONE, ret);

        ret = static_pool_fetch(&pool, tokens[idx], (void **)&object_pointers[idx]);
        assert_int_equal(ERR_NONE, ret);

        ret = static_pool_get_unused_count(&pool, &slots_remaining);
        assert_int_equal(ERR_NONE, ret);
        assert_int_equal(TEST_OBJECT_COUNT - 1 - idx, slots_remaining);
    }

    ret = static_pool_allocate(&pool, &overrun_token);
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
        ret = static_pool_deallocate(&pool, &tokens[idx]);
        assert_int_equal(ERR_NONE, ret);
        assert_int_equal(OBJECT_POOL_TOKEN_INVALID, tokens[idx]);

        ret = static_pool_get_unused_count(&pool, &slots_remaining);
        assert_int_equal(ERR_NONE, ret);
        assert_int_equal(idx + 1, slots_remaining);
    }

}

int test_static_pool_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_null_checks),
        cmocka_unit_test(test_token_out_of_bounds),
        cmocka_unit_test(test_deinit_invalid_states),
        cmocka_unit_test(test_allocation_fills_and_unfills_correctly),
        cmocka_unit_test(test_deallocate_twice_fails),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}