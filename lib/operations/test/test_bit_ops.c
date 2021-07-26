#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_bit_ops.h"
#include "bit_ops.h"

static void test_hamming_weight(void ** state)
{
    uint32_t weight_0 = 0;
    uint32_t weight_1 = 0b100000;
    uint32_t weight_32 = UINT32_MAX;
    uint32_t weight_5 = 0b100010010110;

    assert_int_equal(0, bit_ops_hamming_weight_u32(weight_0));
    assert_int_equal(1, bit_ops_hamming_weight_u32(weight_1));
    assert_int_equal(32, bit_ops_hamming_weight_u32(weight_32));
    assert_int_equal(5, bit_ops_hamming_weight_u32(weight_5));
}

int test_bit_ops_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hamming_weight),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}