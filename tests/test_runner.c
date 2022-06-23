#include "test_bit_ops.h"
#include "test_bounded_heap.h"
#include "test_bsearch_iter.h"
#include "test_circular_buffer.h"
#include "test_copy_queue.h"
#include "test_fast_circular_buffer.h"
#include "test_i_pool_allocator.h"
#include "test_le_pack.h"
#include "test_numeric_ops.h"
#include "test_pack.h"
#include "test_ptr_stack.h"
#include "test_simple_fsm.h"
#include "test_static_event_publisher.h"
#include "test_static_pool.h"


int main()
{
    int result = 0;

    result |= test_bit_ops_run_tests();
    result |= test_bounded_heap_run_tests();
    result |= test_bsearch_iter_tests();
    result |= test_circular_buffer_run_tests();
    result |= test_copy_queue_run_tests();
    result |= test_fast_circular_buffer_run_tests();
    result |= test_i_pool_allocator_run_tests();
    result |= test_le_pack_run_tests();
    result |= test_numeric_ops_run_tests();
    result |= test_pack_run_tests();
    result |= test_circular_buffer_run_tests();
    result |= test_ptr_stack_run_tests();
    result |= test_simple_fsm_run_tests();
    result |= test_static_event_publisher_run_tests();
    result |= test_static_pool_run_tests();

    return result;
}