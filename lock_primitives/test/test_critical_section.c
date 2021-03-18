#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_critical_section.h"
#include "mock_critical_section.h"

#include "critical_section.h"

static void test_interface(void ** state)
{
    error_t ret;

    critical_section_t interface = {
        .context = (void *)0xBEEFDEAD,
        .enter = mock_critical_section_enter,
        .exit = mock_critical_section_exit,
    };

    ret = critical_section_enter(NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    interface.enter = (critical_section_enter_t) 0x1U;
    interface.exit = NULL;
    ret = critical_section_enter(&interface);
    assert_int_equal(ERR_NULL_POINTER, ret);
    
    interface.enter = NULL;
    interface.exit = (critical_section_exit_t) 0x1U;
    ret = critical_section_enter(&interface);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = critical_section_exit(NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    interface.enter = (critical_section_enter_t) 0x1U;
    interface.exit = NULL;
    ret = critical_section_exit(&interface);
    assert_int_equal(ERR_NULL_POINTER, ret);

    interface.enter = NULL;
    interface.exit = (critical_section_exit_t) 0x1U;
    ret = critical_section_exit(&interface);
    assert_int_equal(ERR_NULL_POINTER, ret);

    interface.enter = (critical_section_enter_t) 0x1U;
    interface.exit = NULL;
    ret = critical_section_validate_interface(&interface);
    assert_int_equal(ERR_NULL_POINTER, ret);
    
    interface.enter = NULL;
    interface.exit = (critical_section_exit_t) 0x1U;
    ret = critical_section_validate_interface(&interface);
    assert_int_equal(ERR_NULL_POINTER, ret);
}

static void test_validation_of_interface(void ** state)
{
    error_t ret;

    critical_section_t interface = {
        .context = (void *)0xBEEFDEAD,
        .enter = mock_critical_section_enter,
        .exit = mock_critical_section_exit,
    };

    ret = critical_section_validate_interface(&interface);
    assert_int_equal(ERR_NONE, ret);

}

static void test_calls_pass_through(void ** state)
{
    error_t ret;
    void * context_val = (void *)0xFADEADCE;

    _setup_mock_critical_section_enter_with_count(context_val, 1);
    _setup_mock_critical_section_exit_with_count(context_val, 1);

    // call ordering
    expect_function_call(mock_critical_section_enter);  
    expect_function_call(mock_critical_section_exit);  

    critical_section_t interface;
    mock_critical_section_get_interface(&interface, context_val);

    ret = critical_section_enter(&interface);
    assert_int_equal(ERR_NONE, ret);

    ret = critical_section_exit(&interface);
    assert_int_equal(ERR_NONE, ret);

}

int test_critical_section_run_tests(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_interface),
        cmocka_unit_test(test_validation_of_interface),
        cmocka_unit_test(test_calls_pass_through),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}