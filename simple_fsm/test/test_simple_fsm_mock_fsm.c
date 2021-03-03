#include "test_simple_fsm_mock_fsm.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

size_t state_a_on_entry(simple_fsm_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t state_a_on_event(simple_fsm_t * fsm, void const * event, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(event);
    check_expected_ptr(context);
    return mock();
}

size_t state_a_on_exit(simple_fsm_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t state_b_on_entry(simple_fsm_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t state_b_on_event(simple_fsm_t * fsm, void const * event, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(event);
    check_expected_ptr(context);
    return mock();
}

size_t state_b_on_exit(simple_fsm_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t state_c_on_entry(simple_fsm_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t state_c_on_event(simple_fsm_t * fsm, void const * event, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(event);
    check_expected_ptr(context);
    return mock();
}

size_t state_c_on_exit(simple_fsm_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}