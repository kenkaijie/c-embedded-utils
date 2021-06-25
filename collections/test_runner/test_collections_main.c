#include "test_bounded_heap.h"
#include "test_ptr_stack.h"
#include "test_copy_queue.h"
#include "test_circular_buffer.h"
#include "test_fast_circular_buffer.h"

int main(void)
{
    int result = 0;
    result |= test_ptr_stack_run_tests();
    result |= test_bounded_heap_run_tests();
    result |= test_copy_queue_run_tests();
    result |= test_circular_buffer_run_tests();
    result |= test_fast_circular_buffer_run_tests();
    return result;
}
