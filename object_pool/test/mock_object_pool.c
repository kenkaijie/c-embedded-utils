#include "mock_object_pool.h"

void mock_object_pool_get_interface(object_pool_t * interface, void * context)
{
    interface->context = context;
    interface->allocate = (object_pool_allocate_t)mock_object_pool_allocate;
    interface->fetch = (object_pool_fetch_t)mock_object_pool_fetch;
    interface->deallocate = (object_pool_deallocate_t)mock_object_pool_deallocate;
    interface->get_unused_count = (object_pool_get_unused_count_t)mock_object_pool_get_unused_count;
}

error_t mock_object_pool_allocate(void * pool, size_t * token)
{
    function_called();
    check_expected_ptr(pool);
    check_expected_ptr(token);
    *token = mock_type(size_t);
    return mock_type(error_t);
}

error_t mock_object_pool_fetch(void * pool, size_t token, void ** object_pointer)
{
    function_called();
    check_expected_ptr(pool);
    check_expected(token);
    check_expected_ptr(object_pointer);
    *object_pointer = mock_ptr_type(void *);
    return mock_type(error_t);
}

error_t mock_object_pool_deallocate(void * pool, size_t * token)
{
    function_called();
    check_expected_ptr(pool);
    check_expected_ptr(token);
    *token = mock_type(size_t);
    return mock_type(error_t);
}

error_t mock_object_pool_get_unused_count(void * pool, size_t * unused_count)
{
    function_called();
    check_expected_ptr(pool);
    check_expected_ptr(unused_count);
    *unused_count = mock_type(size_t);
    return mock_type(error_t);
}