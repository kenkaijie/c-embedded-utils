#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_pack.h"
#include "pack.h"

/**
 *  @brief  Tests boundaries for struct pack system. This is used in all primative packs, so it is tested first.
 */
static void test_overrun_checker(void ** state)
{
    assert_true(pack_is_within_buffer(0, 0, 0));
    assert_true(pack_is_within_buffer(0, 0, 1));
    assert_false(pack_is_within_buffer(0, 1, 0));
    assert_true(pack_is_within_buffer(0, 1, 1));
    assert_false(pack_is_within_buffer(1, 0, 0));
    assert_true(pack_is_within_buffer(1, 0, 1));
    assert_false(pack_is_within_buffer(1, 1, 0));
    assert_false(pack_is_within_buffer(1, 1, 1));

    // edge case testing for SIZE_MAX overruns, SIZE_MAX defines the largest array possible.
    assert_false(pack_is_within_buffer(SIZE_MAX, 1, SIZE_MAX));
}

int test_pack_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_overrun_checker)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}