#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "i_pool_allocator.h"
#include "mock_pool_allocator.h"

static void test_allocation(void ** state)
{
    void * object_pointer = NULL;
    ErrorCode_t ret;
    IPoolAllocator_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_pool_allocator_get_interface(&interface, context);

    mock_pool_allocator_setup_allocate_with_count(context, &object_pointer, ERR_NONE, (void *)0xDEADBEEF, 1);
    expect_function_call(mock_pool_allocator_allocate);

    ret = i_pool_allocator_allocate(&interface, &object_pointer);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void *)0xDEADBEEF, object_pointer);
};  

static void test_deallocate(void ** state)
{
    void * object_pointer = 0;
    ErrorCode_t ret;
    IPoolAllocator_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_pool_allocator_get_interface(&interface, context);

    mock_pool_allocator_setup_deallocate_with_count(context, &object_pointer, ERR_NONE, (void *)0xDEADBEEF, 1);
    expect_function_call(mock_pool_allocator_deallocate);

    ret = i_pool_allocator_deallocate(&interface, &object_pointer);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void*)0xDEADBEEF, object_pointer);
}

static void test_get_unused_count(void ** state)
{
    size_t unused_count = 0;
    ErrorCode_t ret;
    IPoolAllocator_t interface;
    void * context = (void *)0xDEADBEEF;

    mock_pool_allocator_get_interface(&interface, context);

    mock_pool_allocator_setup_get_unused_count_with_count(context, 5, 1);
    expect_function_call(mock_pool_allocator_get_unused_count);

    unused_count = i_pool_allocator_get_available_count(&interface);
    assert_int_equal(5, unused_count);
}

int test_i_pool_allocator_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_allocation),
        cmocka_unit_test(test_deallocate),
        cmocka_unit_test(test_get_unused_count),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}