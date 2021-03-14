#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "object_pool.h"
#include "mock_object_pool.h"

static void test_null_checks(void ** state)
{
    error_t ret;
    size_t token = OBJECT_POOL_TOKEN_INVALID;
    size_t unused_count = 0;
    void * object_pointer = NULL;

    ret = object_pool_allocate(NULL, &token);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = object_pool_fetch(NULL, token, &object_pointer);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = object_pool_deallocate(NULL, &token);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = object_pool_get_unused_count(NULL, &unused_count);
    assert_int_equal(ERR_NULL_POINTER, ret);
}

static void test_interface_validation(void ** state)
{
    object_pool_t interface = {
        .context = (void *)0xDEADBEEF,
        .allocate = mock_object_pool_allocate,
        .fetch = mock_object_pool_fetch,
        .deallocate = mock_object_pool_deallocate,
    };
    object_pool_t interface_temp = interface; 
    error_t ret = ERR_NONE;
    size_t token = OBJECT_POOL_TOKEN_INVALID;
    size_t unused_count = 0;
    void * object_pointer = NULL;

    for (size_t idx = 0; idx < 4; ++idx)
    {
        interface_temp = interface;
        switch(idx)
        {
            case 0:
                interface.allocate = NULL;
                break;
            case 1:
                interface.fetch = NULL;
                break;
            case 2:
                interface.deallocate = NULL;  
                break;
            case 3:
                interface.get_unused_count = NULL;
                break;
            default:
                break;
        }

        ret = object_pool_allocate(&interface, &token);
        assert_int_equal(ERR_NULL_POINTER, ret);

        ret = object_pool_allocate(&interface, &token);
        assert_int_equal(ERR_NULL_POINTER, ret);

        ret = object_pool_fetch(&interface, token, &object_pointer);
        assert_int_equal(ERR_NULL_POINTER, ret);

        ret = object_pool_deallocate(&interface, &token);
        assert_int_equal(ERR_NULL_POINTER, ret);

        ret = object_pool_get_unused_count(&interface, &unused_count);
        assert_int_equal(ERR_NULL_POINTER, ret);
    }

}

static void test_allocation(void ** state)
{
    size_t token = 0;
    error_t ret;
    object_pool_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_object_pool_get_interface(&interface, context);

    _setup_mock_object_pool_allocate_with_count(context, &token, ERR_NONE, 5, 1);
    expect_function_call(mock_object_pool_allocate);

    ret = object_pool_allocate(&interface, &token);
    assert_int_equal(ERR_NONE, ret);
    assert_int_equal(5, token);

    _setup_mock_object_pool_allocate_with_count(context, &token, ERR_NO_MEM, token, 1);
    expect_function_call(mock_object_pool_allocate);

    ret = object_pool_allocate(&interface, &token);
    assert_int_equal(ERR_NO_MEM, ret);
};  

static void test_fetch(void ** state)
{
    size_t token = 0xDEAD;
    error_t ret;
    object_pool_t interface;
    void * object_pointer = NULL;
    void * context = (void *)0xDEADBEEF;

    mock_object_pool_get_interface(&interface, context);

    _setup_mock_object_pool_fetch_with_count(context, token, &object_pointer, ERR_NONE, (uintptr_t)0xCAFEBEEF, 1);
    expect_function_call(mock_object_pool_fetch);

    ret = object_pool_fetch(&interface, token, &object_pointer);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((uintptr_t)0xCAFEBEEF, object_pointer);

    _setup_mock_object_pool_fetch_with_count(context, token, &object_pointer, ERR_NO_MEM, 0xCAFEBEEF, 1);
    expect_function_call(mock_object_pool_fetch);

    ret = object_pool_fetch(&interface, token, &object_pointer);
    assert_int_equal(ERR_NO_MEM, ret);
}

static void test_deallocate(void ** state)
{
    size_t token = 0;
    error_t ret;
    object_pool_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_object_pool_get_interface(&interface, context);

    _setup_mock_object_pool_deallocate_with_count(context, &token, ERR_NONE, 5, 1);
    expect_function_call(mock_object_pool_deallocate);

    ret = object_pool_deallocate(&interface, &token);
    assert_int_equal(ERR_NONE, ret);
    assert_int_equal(5, token);

    _setup_mock_object_pool_deallocate_with_count(context, &token, ERR_NO_MEM, token, 1);
    expect_function_call(mock_object_pool_deallocate);

    ret = object_pool_deallocate(&interface, &token);
    assert_int_equal(ERR_NO_MEM, ret);
}

static void test_get_unused_count(void ** state)
{
    size_t unused_count = 0;
    error_t ret;
    object_pool_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_object_pool_get_interface(&interface, context);

    _setup_mock_object_pool_get_unused_count_with_count(context, &unused_count, ERR_NONE, 5, 1);
    expect_function_call(mock_object_pool_get_unused_count);

    ret = object_pool_get_unused_count(&interface, &unused_count);
    assert_int_equal(ERR_NONE, ret);
    assert_int_equal(5, unused_count);

    _setup_mock_object_pool_get_unused_count_with_count(context, &unused_count, ERR_GENERIC_ERROR, unused_count, 1);
    expect_function_call(mock_object_pool_get_unused_count);

    ret = object_pool_get_unused_count(&interface, &unused_count);
    assert_int_equal(ERR_GENERIC_ERROR, ret);
}

int test_object_pool_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_null_checks),
        cmocka_unit_test(test_interface_validation),
        cmocka_unit_test(test_allocation),
        cmocka_unit_test(test_fetch),
        cmocka_unit_test(test_deallocate),
        cmocka_unit_test(test_get_unused_count),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}