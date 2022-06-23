#include "mock_pool_allocator.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

void mock_pool_allocator_get_interface(IPoolAllocator_t * interface, void * context)
{
    interface->context = context;
    interface->allocate = (i_pool_allocate_allocate_t)mock_pool_allocator_allocate;
    interface->deallocate = (i_pool_allocate_deallocate_t)mock_pool_allocator_deallocate;
    interface->get_available_count = (i_pool_allocate_get_available_count_t)mock_pool_allocator_get_unused_count;
}

ErrorCode_t mock_pool_allocator_allocate(void * pool, void ** object_pointer)
{
    function_called();
    check_expected_ptr(pool);
    check_expected_ptr(object_pointer);
    *object_pointer = mock_ptr_type(void *);
    return mock_type(ErrorCode_t);
}

ErrorCode_t mock_pool_allocator_deallocate(void * pool, void ** object_pointer)
{
    function_called();
    check_expected_ptr(pool);
    check_expected_ptr(object_pointer);
    *object_pointer = mock_ptr_type(void *);
    return mock_type(ErrorCode_t);
}

size_t mock_pool_allocator_get_unused_count(void * pool)
{
    function_called();
    check_expected_ptr(pool);
    return mock_type(size_t);
}

void mock_pool_allocator_setup_allocate_with_count(void * with_pool_arg, void ** with_object_pointer_arg, ErrorCode_t returns_val, void * sets_object_pointer_arg, size_t num_times)
{
    expect_value_count(mock_pool_allocator_allocate, pool, (uintptr_t)with_pool_arg, num_times);
    expect_value_count(mock_pool_allocator_allocate, object_pointer, (uintptr_t)with_object_pointer_arg, num_times);
    for (size_t idx = 0; idx < num_times; ++idx) 
    { 
        will_return(mock_pool_allocator_allocate, (uintptr_t)sets_object_pointer_arg); 
        will_return(mock_pool_allocator_allocate, returns_val); 
    } 
}

void mock_pool_allocator_setup_deallocate_with_count(void * with_pool_arg, void **  with_object_pointer_arg, ErrorCode_t returns_val, void * sets_object_pointer_arg, size_t num_times)
{
    expect_value_count(mock_pool_allocator_deallocate, pool, (uintptr_t)with_pool_arg, num_times);
    expect_value_count(mock_pool_allocator_deallocate, object_pointer, (uintptr_t)with_object_pointer_arg, num_times);
    for (size_t idx = 0; idx < num_times; ++idx) 
    { 
        will_return(mock_pool_allocator_deallocate, (uintptr_t)sets_object_pointer_arg); 
        will_return(mock_pool_allocator_deallocate, returns_val); 
    }
}

void mock_pool_allocator_setup_get_unused_count_with_count(void * with_pool_arg, size_t returns_val, size_t num_times)
{
    expect_value_count(mock_pool_allocator_get_unused_count, pool, (uintptr_t)with_pool_arg, num_times);
    for (size_t idx = 0; idx < num_times; ++idx) 
    { 
        will_return(mock_pool_allocator_get_unused_count, returns_val); 
    } 
}