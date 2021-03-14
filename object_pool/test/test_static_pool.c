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

static void test_deinit_invalid_states(void ** state)
{
    error_t ret;
    size_t const object_size = sizeof(test_static_pool_dummy_data_t);
    size_t const object_count = 16;
    uint8_t buffer[object_size * object_count];
    static_metadata_t metadata_buffer[object_count];
    size_t token;
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

static void test_allocation_fills(void ** state)
{
    error_t ret;
    size_t const object_size = sizeof(test_static_pool_dummy_data_t);
    size_t const object_count = 16;
    uint8_t buffer[object_size * object_count];
    static_metadata_t metadata_buffer[object_count];
    size_t tokens[object_count];
    size_t overrun_token;
    for (size_t idx = 0; idx < object_count; ++idx)
    {
        tokens[idx] = OBJECT_POOL_TOKEN_INVALID;
    }

    static_pool_config_t config = {
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .use_lock = false,
        .metadata_buffer = metadata_buffer,
        .object_count = object_count,
        .object_size = object_size, 
    };

    static_pool_t pool;

    ret = static_pool_init(&pool, &config);
    assert_int_equal(ERR_NONE, ret);

    for(size_t idx = 0; idx < object_count; ++idx)
    {
        ret = static_pool_allocate(&pool, &tokens[idx]);
        assert_int_equal(ERR_NONE, ret);
    }

    ret = static_pool_allocate(&pool, &overrun_token);
    assert_int_equal(ERR_NO_MEM, ret);
}

int test_static_pool_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_deinit_invalid_states),
        cmocka_unit_test(test_allocation_fills),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}