#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "object_pool.h"
#include "mock_object_pool.h"

static void test_allocation(void ** state)
{
    void * object_pointer = NULL;
    error_t ret;
    object_pool_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_object_pool_get_interface(&interface, context);

    _setup_mock_object_pool_allocate_with_count(context, &object_pointer, ERR_NONE, (void *)0xDEADBEEF, 1);
    expect_function_call(mock_object_pool_allocate);

    ret = object_pool_allocate(&interface, &object_pointer);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void *)0xDEADBEEF, object_pointer);

    _setup_mock_object_pool_allocate_with_count(context, &object_pointer, ERR_NO_MEM, object_pointer, 1);
    expect_function_call(mock_object_pool_allocate);

    ret = object_pool_allocate(&interface, &object_pointer);
    assert_int_equal(ERR_NO_MEM, ret);
};  

static void test_deallocate(void ** state)
{
    void * object_pointer = 0;
    error_t ret;
    object_pool_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_object_pool_get_interface(&interface, context);

    _setup_mock_object_pool_deallocate_with_count(context, &object_pointer, ERR_NONE, (void *)0xDEADBEEF, 1);
    expect_function_call(mock_object_pool_deallocate);

    ret = object_pool_deallocate(&interface, &object_pointer);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void*)0xDEADBEEF, object_pointer);

    _setup_mock_object_pool_deallocate_with_count(context, &object_pointer, ERR_NO_MEM, (void *)0xBEEFCAFE, 1);
    expect_function_call(mock_object_pool_deallocate);

    ret = object_pool_deallocate(&interface, &object_pointer);
    assert_int_equal(ERR_NO_MEM, ret);
}

static void test_get_unused_count(void ** state)
{
    size_t unused_count = 0;
    error_t ret;
    object_pool_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_object_pool_get_interface(&interface, context);

    _setup_mock_object_pool_get_unused_count_with_count(context, 5, 1);
    expect_function_call(mock_object_pool_get_unused_count);

    unused_count = object_pool_get_available_count(&interface);
    assert_int_equal(5, unused_count);
}

int test_object_pool_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_allocation),
        cmocka_unit_test(test_deallocate),
        cmocka_unit_test(test_get_unused_count),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}