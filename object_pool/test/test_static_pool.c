#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "static_pool.h"
#include "mock_critical_section.h"

int test_static_pool_run_tests(void)
{
    const struct CMUnitTest tests[] = {
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}