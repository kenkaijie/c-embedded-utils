#pragma once

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "error_codes.h"
#include "object_pool.h"

/**
 *  @file   Mock for an object pool. Used for testing. Used to test object pools and devices relying on an object pool.
 *          Typically you would use the define macros to set up the calls, then provide the call ordering configuration in the test.
 */

void mock_object_pool_get_interface(object_pool_t * interface, void * context);

error_t mock_object_pool_allocate(void * pool, void ** object_pointer);
error_t mock_object_pool_deallocate(void * pool, void ** object_pointer);
size_t mock_object_pool_get_unused_count(void * pool);

#define _setup_mock_object_pool_allocate_with_count(with_pool_arg, with_object_pointer_arg, returns_val, sets_object_pointer_arg, num_times) \
do {\
    expect_value_count(mock_object_pool_allocate, pool, (uintptr_t)with_pool_arg, num_times);\
    expect_value_count(mock_object_pool_allocate, object_pointer, (uintptr_t)with_object_pointer_arg, num_times);\
    for (size_t idx = 0; idx < num_times; ++idx) \
    { \
        will_return(mock_object_pool_allocate, (uintptr_t)sets_object_pointer_arg); \
        will_return(mock_object_pool_allocate, returns_val); \
    } \
} while(false)

#define _setup_mock_object_pool_deallocate_with_count(with_pool_arg, with_object_pointer_arg, returns_val, sets_object_pointer_arg, num_times) \
do {\
    expect_value_count(mock_object_pool_deallocate, pool, (uintptr_t)with_pool_arg, num_times);\
    expect_value_count(mock_object_pool_deallocate, object_pointer, (uintptr_t)with_object_pointer_arg, num_times);\
    for (size_t idx = 0; idx < num_times; ++idx) \
    { \
        will_return(mock_object_pool_deallocate, (uintptr_t)sets_object_pointer_arg); \
        will_return(mock_object_pool_deallocate, returns_val); \
    } \
} while(false)

#define _setup_mock_object_pool_get_unused_count_with_count(with_pool_arg, returns_val, num_times) \
do {\
    expect_value_count(mock_object_pool_get_unused_count, pool, (uintptr_t)with_pool_arg, num_times);\
    for (size_t idx = 0; idx < num_times; ++idx) \
    { \
        will_return(mock_object_pool_get_unused_count, returns_val); \
    } \
} while(false)