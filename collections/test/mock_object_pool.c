#include "mock_object_pool.h"

void mock_object_pool_get_interface(object_pool_t * interface, void * context)
{
    interface->context = context;
    interface->allocate = (object_pool_allocate_t)mock_object_pool_allocate;
    interface->deallocate = (object_pool_deallocate_t)mock_object_pool_deallocate;
    interface->get_available_count = (object_pool_get_available_count_t)mock_object_pool_get_unused_count;
}

error_t mock_object_pool_allocate(void * pool, void ** object_pointer)
{
    function_called();
    check_expected_ptr(pool);
    check_expected_ptr(object_pointer);
    *object_pointer = mock_ptr_type(void *);
    return mock_type(error_t);
}

error_t mock_object_pool_deallocate(void * pool, void ** object_pointer)
{
    function_called();
    check_expected_ptr(pool);
    check_expected_ptr(object_pointer);
    *object_pointer = mock_ptr_type(void *);
    return mock_type(error_t);
}

size_t mock_object_pool_get_unused_count(void * pool)
{
    function_called();
    check_expected_ptr(pool);
    return mock_type(size_t);
}