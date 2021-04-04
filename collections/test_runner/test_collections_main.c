#include "test_object_pool.h"
#include "test_static_pool.h"
#include "test_ptr_stack.h"

int main(void)
{
    int result = 0;
    result |= test_object_pool_run_tests();
    result |= test_static_pool_run_tests();
    result |= test_ptr_stack_run_tests();
    return result;
}