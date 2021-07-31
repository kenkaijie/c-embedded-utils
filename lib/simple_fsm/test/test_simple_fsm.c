#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "simple_fsm.h"

#include "test_simple_fsm.h"
#include "mock_fsm.h"

static SimpleFSMStateDelegates_t state_delegates[MOCK_FSM_STATE_COUNT] = {
    {mock_fsm_state_a_on_entry, (SimpleFSMOnEventFunction_t)mock_fsm_state_a_on_event, mock_fsm_state_a_on_exit},
    {mock_fsm_state_b_on_entry, (SimpleFSMOnEventFunction_t)mock_fsm_state_b_on_event, mock_fsm_state_b_on_exit},
    {mock_fsm_state_c_on_entry, (SimpleFSMOnEventFunction_t)mock_fsm_state_c_on_event, mock_fsm_state_c_on_exit},
};

static SimpleFSMConfig_t const config = {
    .context = (void *)0x12345678U,
    .state_delegates = state_delegates,
    .state_count = MOCK_FSM_STATE_COUNT,
    .initial_state = MOCK_FSM_STATE_A,
    .max_transition_count = 100, /**< just some large number for now */
};

/**
 *  @brief  This is just a test to ensure no funny business on the deinit, for this object, deinit does nothing.
 */
static void test_deinit_does_not_fail()
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;

    // init
    ret = simple_fsm_init(&fsm, &config);
    assert_int_equal(ERR_NONE, ret);

    // deinit actually does nothing
    simple_fsm_deinit(&fsm);
}

/**
 *  @brief  Ensures correct return codes as per the API
 */
static void test_not_started_state_machine(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_A, &fsm, config.context, -1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_A, &fsm, config.context, -1);

    ignore_function_calls(mock_fsm_state_a_on_entry);
    ignore_function_calls(mock_fsm_state_a_on_exit);

    // init
    ret = simple_fsm_init(&fsm, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NOOP, ret);

    ret = simple_fsm_force_stop(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_force_stop(&fsm);
    assert_int_equal(ERR_NOOP, ret);

    uint32_t event = 1;
    ret = simple_fsm_on_event(&fsm, &event);
    assert_int_equal(ERR_NOT_INITIALISED, ret);
}

/**
 *  @brief  The test here is to ensure that if we have a state handler which calls a bad value, the start function complains. If the return value is bad, 
 *          we should at least ensure we call on exit to attempt a cleanup.
 */
static void test_start_bad_fsm_def(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_COUNT, &fsm, config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_A, &fsm, config.context, 1);
    
    // call order
    ignore_function_calls(mock_fsm_state_a_on_entry);
    ignore_function_calls(mock_fsm_state_a_on_exit);

    ret = simple_fsm_init(&fsm, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_OUT_OF_BOUNDS, ret);
}


/**
 *  @brief  Test bad state delegate callbacks
 */
static void test_invalid_config_no_states(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t test_config = config;
    // out of bounds initial state

    test_config.state_count = 0;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_INVALID_ARG, ret);
    test_config.state_count = MOCK_FSM_STATE_COUNT;

    test_config.initial_state = MOCK_FSM_STATE_COUNT;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_INVALID_ARG, ret);
}

/**
 *  @brief  Test bad state delegate callbacks
 */
static void test_invalid_config_bad_max_transition(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t test_config = config;
    // zero max transition count
    test_config.max_transition_count = 0;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_INVALID_ARG, ret);
}

/**
 *  @brief  Test a simple startup for A 
 */
static void test_simple_start(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_A, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_a_on_entry);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    size_t fsm_state = simple_fsm_get_current_state(&fsm);
    assert_int_equal(MOCK_FSM_STATE_A, fsm_state);
}

static void test_start_out_of_bounds_checking(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_COUNT, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_COUNT, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_a_on_entry);
    expect_function_call(mock_fsm_state_a_on_exit);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_OUT_OF_BOUNDS, ret);
}

static void test_on_event_out_of_bounds_checking(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;
    void * event = (void *)0xDEADBEEF;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_A, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_event_mock_with_count(mock_fsm_state_a_on_event, MOCK_FSM_STATE_COUNT, &fsm, event, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_A, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_a_on_entry);
    expect_function_call(mock_fsm_state_a_on_event);
    expect_function_call(mock_fsm_state_a_on_exit);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_on_event(&fsm, event);
    assert_int_equal(ERR_OUT_OF_BOUNDS, ret);
}


/**
 *  @brief  Test a simple startup for A, this time we fail on the (On Entry) transition of A, 
 *          so we expect A (En, Fail) -> A (Ex) -> B (En, Fail) -> B (Ex) -> C(En)
 */
static void test_simple_start_transitions(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_B, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_A, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_b_on_entry, MOCK_FSM_STATE_C, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_b_on_exit, MOCK_FSM_STATE_B, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_entry, MOCK_FSM_STATE_C, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_a_on_entry);
    expect_function_call(mock_fsm_state_a_on_exit);
    expect_function_call(mock_fsm_state_b_on_entry);
    expect_function_call(mock_fsm_state_b_on_exit);
    expect_function_call(mock_fsm_state_c_on_entry);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    size_t fsm_state = simple_fsm_get_current_state(&fsm);
    assert_int_equal(MOCK_FSM_STATE_C, fsm_state);
}

/**
 *  @brief  Test a simple infinite condition. We will set max recursion depth to ensure we stop after 3 loops.
 */
static void test_looped_start(void ** state)
{
    size_t cycles = 5;
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;

    fsm_config.max_transition_count = cycles * 2; // we are just going between 2 states

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_B, &fsm, fsm_config.context, cycles + 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_A, &fsm, fsm_config.context, cycles);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_b_on_entry, MOCK_FSM_STATE_A, &fsm, fsm_config.context, cycles);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_b_on_exit, MOCK_FSM_STATE_B, &fsm, fsm_config.context, cycles);

    // call order
    expect_function_call(mock_fsm_state_a_on_entry);
    for (size_t idx = 0; idx < cycles; ++idx)
    {
        expect_function_call(mock_fsm_state_a_on_exit);
        expect_function_call(mock_fsm_state_b_on_entry);
        expect_function_call(mock_fsm_state_b_on_exit);
        expect_function_call(mock_fsm_state_a_on_entry);
    }

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_TIMEOUT, ret);
}

/**
 *  @brief  Testing event correctly passes the event parameter to the correct state, and the state transitions occur. The scenario here is:
 *              - FSM starts in State A
 *              - FSM On Event called in State A, requests state C
 *              - FSM calls State A On Exit
 *              - FSM calls State C On Entry
 *              
 */
static void test_on_event_transitions(void ** state)
{
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    state_event_t const event = 0xBEEFCAFEU;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_A, &fsm, config.context, 1);
    mock_fsm_setup_on_event_mock_with_count(mock_fsm_state_a_on_event, MOCK_FSM_STATE_C, &fsm, &event, config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_A, &fsm, config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_entry, MOCK_FSM_STATE_C, &fsm, config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_a_on_entry);
    expect_function_call(mock_fsm_state_a_on_event);
    expect_function_call(mock_fsm_state_a_on_exit);
    expect_function_call(mock_fsm_state_c_on_entry);

    ret = simple_fsm_init(&fsm, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_on_event(&fsm, &event);
    assert_int_equal(ERR_NONE, ret);

    size_t final_state = simple_fsm_get_current_state(&fsm);
    assert_int_equal(MOCK_FSM_STATE_C, final_state);
}

/**
 *  @brief  This test is the loops transition equivalent for the event case. We start by entering state A, then we call an event which spawns an infinite
 *          transition. See the mock setup for the test case.
 */
static void test_looped_transitions(void ** state)
{
    size_t cycles = 5;
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;
    fsm_config.initial_state = MOCK_FSM_STATE_B;
    state_event_t event = 0xDEADBEEF;

    fsm_config.max_transition_count = (cycles * 2) + 1; // we cap the number of transitions (+1 for the initial transition into state B)

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_b_on_entry, MOCK_FSM_STATE_B, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_event_mock_with_count(mock_fsm_state_b_on_event, MOCK_FSM_STATE_A, &fsm, &event, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_b_on_exit, MOCK_FSM_STATE_B, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_entry, MOCK_FSM_STATE_C, &fsm, fsm_config.context, cycles + 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_a_on_exit, MOCK_FSM_STATE_A, &fsm, fsm_config.context, cycles);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_entry, MOCK_FSM_STATE_A, &fsm, fsm_config.context, cycles);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_exit, MOCK_FSM_STATE_C, &fsm, fsm_config.context, cycles);

    // call order
    expect_function_call(mock_fsm_state_b_on_entry);
    expect_function_call(mock_fsm_state_b_on_event);
    expect_function_call(mock_fsm_state_b_on_exit);
    expect_function_call(mock_fsm_state_a_on_entry);
    for (size_t idx = 0; idx < cycles; ++idx)
    {
        expect_function_call(mock_fsm_state_a_on_exit);
        expect_function_call(mock_fsm_state_c_on_entry);
        expect_function_call(mock_fsm_state_c_on_exit);
        expect_function_call(mock_fsm_state_a_on_entry);
    }

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_on_event(&fsm, &event);
    assert_int_equal(ERR_TIMEOUT, ret);
}

/**
 *  @brief  Tests that the request being the same will remain in the same state.
 */
static void test_event_stays_same_state(void ** state)
{
    size_t cycles = 5;
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;
    fsm_config.initial_state = MOCK_FSM_STATE_C;
    state_event_t event = 0xDEADBEEF;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_entry, MOCK_FSM_STATE_C, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_event_mock_with_count(mock_fsm_state_c_on_event, MOCK_FSM_STATE_C, &fsm, &event, fsm_config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_c_on_entry);
    expect_function_call(mock_fsm_state_c_on_event);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_on_event(&fsm, &event);
    assert_int_equal(ERR_NONE, ret);

    size_t final_state = simple_fsm_get_current_state(&fsm);
    assert_int_equal(MOCK_FSM_STATE_C, final_state);
}

static void test_force_stop_passes_through(void ** state)
{
    size_t cycles = 5;
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;
    fsm_config.initial_state = MOCK_FSM_STATE_C;
    state_event_t event = 0xDEADBEEF;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_entry, MOCK_FSM_STATE_C, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_exit, MOCK_FSM_STATE_C, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_c_on_entry);
    expect_function_call(mock_fsm_state_c_on_exit);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_force_stop(&fsm);
    assert_int_equal(ERR_NONE, ret);

    size_t final_state = simple_fsm_get_current_state(&fsm);
    assert_int_equal(MOCK_FSM_STATE_C, final_state);
}

static void test_force_stop_returns_error_if_state_fails_transition(void ** state)
{
    size_t cycles = 5;
    ErrorCode_t ret;
    SimpleFSM_t fsm;
    SimpleFSMConfig_t fsm_config = config;
    fsm_config.initial_state = MOCK_FSM_STATE_C;
    state_event_t event = 0xDEADBEEF;

    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_entry, MOCK_FSM_STATE_C, &fsm, fsm_config.context, 1);
    mock_fsm_setup_on_entry_exit_mock_with_count(mock_fsm_state_c_on_exit, MOCK_FSM_STATE_A, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(mock_fsm_state_c_on_entry);
    expect_function_call(mock_fsm_state_c_on_exit);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_force_stop(&fsm);
    assert_int_equal(ERR_INCOMPLETE, ret);

    size_t final_state = simple_fsm_get_current_state(&fsm);
    assert_int_equal(MOCK_FSM_STATE_C, final_state);
}

int test_simple_fsm_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_start_bad_fsm_def),
        cmocka_unit_test(test_deinit_does_not_fail),
        cmocka_unit_test(test_not_started_state_machine),
        cmocka_unit_test(test_invalid_config_no_states),
        cmocka_unit_test(test_invalid_config_bad_max_transition),
        cmocka_unit_test(test_simple_start),
        cmocka_unit_test(test_simple_start_transitions),
        cmocka_unit_test(test_looped_start),
        cmocka_unit_test(test_on_event_transitions),
        cmocka_unit_test(test_looped_transitions),
        cmocka_unit_test(test_event_stays_same_state),
        cmocka_unit_test(test_force_stop_passes_through),
        cmocka_unit_test(test_force_stop_returns_error_if_state_fails_transition),
        cmocka_unit_test(test_start_out_of_bounds_checking),
        cmocka_unit_test(test_on_event_out_of_bounds_checking),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}