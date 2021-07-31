#include "test_i_pool_allocator.h"
#include "test_static_pool.h"

int main(void)
{
    int result = 0;
    result |= test_i_pool_allocator_run_tests();
    result |= test_static_pool_run_tests();
    return result;
}