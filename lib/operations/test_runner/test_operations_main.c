#include "test_bit_ops.h"
#include "test_numeric_ops.h"

int main(void)
{
    int result = 0;
    result |= test_bit_ops_run_tests();
    result |= test_numeric_ops_run_tests();
    return result;
}
