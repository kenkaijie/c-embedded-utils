#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_numeric_ops.h"
#include "numeric_ops.h"

static void test_power_of_2(void ** state)
{
    assert_false(numeric_ops_is_power_2_sz(0));
    assert_true(numeric_ops_is_power_2_sz(1));
    assert_true(numeric_ops_is_power_2_sz(2));
    assert_false(numeric_ops_is_power_2_sz(3));
    assert_true(numeric_ops_is_power_2_sz(4));
}

int test_numeric_ops_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_power_of_2),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}