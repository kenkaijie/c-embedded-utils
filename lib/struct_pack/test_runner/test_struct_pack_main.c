#include "test_le_pack.h"
#include "test_pack.h"

int main(void)
{
    int result = 0;
    result |= test_le_pack_run_tests();
    result |= test_pack_run_tests();
    return result;
}