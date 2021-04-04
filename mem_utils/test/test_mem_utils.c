#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_mem_utils.h"
#include "mem_utils.h"

static void test_correct_mem_fill()
{
    uint8_t buffer[7] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t expected_buffer[7] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE};
    mem_utils_fill_deadbeef(buffer, sizeof(buffer));
    assert_memory_equal(expected_buffer, buffer, sizeof(buffer));
}

int test_mem_utils_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_correct_mem_fill),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}