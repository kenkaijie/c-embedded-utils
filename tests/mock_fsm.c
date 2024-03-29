#include "mock_fsm.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

size_t mock_fsm_state_a_on_entry(SimpleFSM_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_a_on_event(SimpleFSM_t * fsm, state_event_t const * event, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(event);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_a_on_exit(SimpleFSM_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_b_on_entry(SimpleFSM_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_b_on_event(SimpleFSM_t * fsm, state_event_t const * event, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(event);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_b_on_exit(SimpleFSM_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_c_on_entry(SimpleFSM_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_c_on_event(SimpleFSM_t * fsm, state_event_t const * event, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(event);
    check_expected_ptr(context);
    return mock();
}

size_t mock_fsm_state_c_on_exit(SimpleFSM_t * fsm, void * context)
{
    function_called();
    check_expected_ptr(fsm);
    check_expected_ptr(context);
    return mock();
}