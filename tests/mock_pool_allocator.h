/**
 * @file   
 * @brief Mock object implementing a IPoolAllocator.
 * 
 * Used for testing. Used to test object pools and devices relying on an object pool.
 * Typically the calls should be setup using the mock_pool_allocator_setup_* functions instead of the functions directly.
 */

#pragma once

#include <cemb/error_codes.h>
#include <cemb/i_pool_allocator.h>

void mock_pool_allocator_get_interface(IPoolAllocator_t * interface, void * context);

ErrorCode_t mock_pool_allocator_allocate(void * pool, void ** object_pointer);
ErrorCode_t mock_pool_allocator_deallocate(void * pool, void ** object_pointer);
size_t mock_pool_allocator_get_unused_count(void * pool);

void mock_pool_allocator_setup_allocate_with_count(void * with_pool_arg, void ** with_object_pointer_arg, ErrorCode_t returns_val, void * sets_object_pointer_arg, size_t num_times);
void mock_pool_allocator_setup_deallocate_with_count(void * with_pool_arg, void **  with_object_pointer_arg, ErrorCode_t returns_val, void * sets_object_pointer_arg, size_t num_times);
void mock_pool_allocator_setup_get_unused_count_with_count(void * with_pool_arg, size_t returns_val, size_t num_times);
