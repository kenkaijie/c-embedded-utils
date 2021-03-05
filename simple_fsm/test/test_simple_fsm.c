#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "simple_fsm.h"

#include "test_simple_fsm_mock_fsm.h"

static simple_fsm_state_delegates_t state_delegates[SIMPLE_STATE_COUNT] = {
    {state_a_on_entry, (simple_fsm_on_event_handler_t)state_a_on_event, state_a_on_exit},
    {state_b_on_entry, (simple_fsm_on_event_handler_t)state_b_on_event, state_b_on_exit},
    {state_c_on_entry, (simple_fsm_on_event_handler_t)state_c_on_event, state_c_on_exit},
};

static simple_fsm_config_t const config = {
    .context = (void *)0x12345678U,
    .state_delegates = state_delegates,
    .state_count = SIMPLE_STATE_COUNT,
    .initial_state = SIMPLE_STATE_A,
    .max_transition_count = 100, /**< just some large number for now */
};

/**
 * @brief   Helper function to populate a On Entry/On Exit mock call.
 */
#define _setup_on_entry_exit_mock_with_count(function, will_return, with_fsm_val, with_context_val, num_times) \
do { \
    will_return_count(function, will_return, num_times); \
    expect_value_count(function, fsm, (intptr_t)with_fsm_val, num_times); \
    expect_value_count(function, context, (intptr_t)with_context_val, num_times); \
} while(false) \

/**
 * @brief   Helper function to populate a On Event mock call.
 */
#define _setup_on_event_mock_with_count(function, will_return, with_fsm_val, with_event_val, with_context_val, num_times) \
do { \
    will_return_count(function, will_return, num_times); \
    expect_value_count(function, fsm, (intptr_t)with_fsm_val, num_times); \
    expect_value_count(function, event, (intptr_t)with_event_val, num_times); \
    expect_value_count(function, context, (intptr_t)with_context_val, num_times); \
} while(false) \

/**
 *  @brief  Test to ensure that the public interface responds correctly to all bad inputs.
 */
static void test_interface_nulls(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t test_config = config;

    // init
    ret = simple_fsm_init(NULL, &config);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = simple_fsm_init(&fsm, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    // deinit
    ret = simple_fsm_deinit(NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    size_t state_val;
    // simple_fsm_get_current_state
    ret = simple_fsm_get_current_state(NULL, &state_val);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = simple_fsm_get_current_state(&fsm, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    // start, as we never initialise the fsm, this should fail
    ret = simple_fsm_start(NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    // on event, as we never initialise the fsm, this should fail
    state_event_t const event = 0x5423U;
    ret = simple_fsm_on_event(NULL, &event);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = simple_fsm_on_event(&fsm, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);
}

static void test_post_deinit_calls_fail(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;
    size_t state_val = 0;
    state_event_t const event = 0x1234U; 

    ret = simple_fsm_init(&fsm, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_deinit(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_get_current_state(&fsm, &state_val);
    assert_int_equal(ERR_NOT_INITIALISED, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NOT_INITIALISED, ret);

    ret = simple_fsm_on_event(&fsm, &event);
    assert_int_equal(ERR_NOT_INITIALISED, ret);

}

/**
 *  @brief  The test here is to ensure that if we have a state handler which calls a bad value, the start function complains. If the return value is bad, 
 *          we should at least ensure we call on exit to attempt a cleanup.
 */
static void test_start_bad_fsm_def(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;

    _setup_on_entry_exit_mock_with_count(state_a_on_entry, SIMPLE_STATE_COUNT, &fsm, config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_a_on_exit, SIMPLE_STATE_A, &fsm, config.context, 1);
    
    // call order
    ignore_function_calls(state_a_on_entry);
    ignore_function_calls(state_a_on_exit);

    ret = simple_fsm_init(&fsm, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_OUT_OF_BOUNDS, ret);
}

/**
 *  @brief  Test bad state delegate
 */
static void test_invalid_config_no_state_delegate(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t test_config = config;

    // bad state delegate
    test_config.state_delegates = NULL;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_NULL_POINTER, ret);
}

/**
 *  @brief  Test bad state delegate callbacks internal handlers
 */
static void test_invalid_config_no_state_delegate_callbacks(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t test_config = config;
    simple_fsm_state_delegates_t test_state_delegate = {
        .on_entry_handler = state_a_on_entry,
        .on_event_handler = (simple_fsm_on_event_handler_t)state_a_on_event,
        .on_exit_handler = state_a_on_exit,
    };
    test_config.state_delegates = &test_state_delegate;
    test_config.state_count = 1;

    test_state_delegate.on_entry_handler = NULL;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    test_state_delegate.on_entry_handler = state_a_on_entry;

    test_state_delegate.on_event_handler = NULL;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    test_state_delegate.on_event_handler = (simple_fsm_on_event_handler_t)state_a_on_event;

    test_state_delegate.on_exit_handler = NULL;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_NULL_POINTER, ret);
    test_state_delegate.on_exit_handler = state_a_on_exit;
}

/**
 *  @brief  Test bad state delegate callbacks
 */
static void test_invalid_config_no_states(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t test_config = config;
    // out of bounds initial state

    test_config.state_count = 0;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_INVALID_ARG, ret);
    test_config.state_count = SIMPLE_STATE_COUNT;

    test_config.initial_state = SIMPLE_STATE_COUNT;
    ret = simple_fsm_init(&fsm, &test_config);
    assert_int_equal(ERR_INVALID_ARG, ret);
}

/**
 *  @brief  Test bad state delegate callbacks
 */
static void test_invalid_config_bad_max_transition(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t test_config = config;
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
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t fsm_config = config;

    _setup_on_entry_exit_mock_with_count(state_a_on_entry, SIMPLE_STATE_A, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(state_a_on_entry);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    size_t fsm_state;
    ret = simple_fsm_get_current_state(&fsm, &fsm_state);
    assert_int_equal(ERR_NONE, ret);
    assert_int_equal(SIMPLE_STATE_A, fsm_state);
}

/**
 *  @brief  Test a simple startup for A, this time we fail on the (On Entry) transition of A, 
 *          so we expect A (En, Fail) -> A (Ex) -> B (En, Fail) -> B (Ex) -> C(En)
 */
static void test_simple_start_transitions(void ** state)
{
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t fsm_config = config;

    _setup_on_entry_exit_mock_with_count(state_a_on_entry, SIMPLE_STATE_B, &fsm, fsm_config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_a_on_exit, SIMPLE_STATE_A, &fsm, fsm_config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_b_on_entry, SIMPLE_STATE_C, &fsm, fsm_config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_b_on_exit, SIMPLE_STATE_B, &fsm, fsm_config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_c_on_entry, SIMPLE_STATE_C, &fsm, fsm_config.context, 1);

    // call order
    expect_function_call(state_a_on_entry);
    expect_function_call(state_a_on_exit);
    expect_function_call(state_b_on_entry);
    expect_function_call(state_b_on_exit);
    expect_function_call(state_c_on_entry);

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    size_t fsm_state;
    ret = simple_fsm_get_current_state(&fsm, &fsm_state);
    assert_int_equal(ERR_NONE, ret);
    assert_int_equal(SIMPLE_STATE_C, fsm_state);
}

/**
 *  @brief  Test a simple infinite condition. We will set max recursion depth to ensure we stop after 3 loops.
 */
static void test_looped_start(void ** state)
{
    size_t cycles = 5;
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t fsm_config = config;

    fsm_config.max_transition_count = cycles * 2; // we are just going between 2 states

    _setup_on_entry_exit_mock_with_count(state_a_on_entry, SIMPLE_STATE_B, &fsm, fsm_config.context, cycles + 1);
    _setup_on_entry_exit_mock_with_count(state_a_on_exit, SIMPLE_STATE_A, &fsm, fsm_config.context, cycles);
    _setup_on_entry_exit_mock_with_count(state_b_on_entry, SIMPLE_STATE_A, &fsm, fsm_config.context, cycles);
    _setup_on_entry_exit_mock_with_count(state_b_on_exit, SIMPLE_STATE_B, &fsm, fsm_config.context, cycles);

    // call order
    expect_function_call(state_a_on_entry);
    for (size_t idx = 0; idx < cycles; ++idx)
    {
        expect_function_call(state_a_on_exit);
        expect_function_call(state_b_on_entry);
        expect_function_call(state_b_on_exit);
        expect_function_call(state_a_on_entry);
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
    error_t ret;
    simple_fsm_t fsm;
    state_event_t const event = 0xBEEFCAFEU;

    _setup_on_entry_exit_mock_with_count(state_a_on_entry, SIMPLE_STATE_A, &fsm, config.context, 1);
    _setup_on_event_mock_with_count(state_a_on_event, SIMPLE_STATE_C, &fsm, &event, config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_a_on_exit, SIMPLE_STATE_A, &fsm, config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_c_on_entry, SIMPLE_STATE_C, &fsm, config.context, 1);

    // call order
    expect_function_call(state_a_on_entry);
    expect_function_call(state_a_on_event);
    expect_function_call(state_a_on_exit);
    expect_function_call(state_c_on_entry);

    ret = simple_fsm_init(&fsm, &config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_on_event(&fsm, &event);
    assert_int_equal(ERR_NONE, ret);
}

/**
 *  @brief  This test is the loops transition equivalent for the event case. We start by entering state A, then we call an event which spawns an infinite
 *          transition. See the mock setup for the test case.
 */
static void test_looped_transitions(void ** state)
{
    size_t cycles = 5;
    error_t ret;
    simple_fsm_t fsm;
    simple_fsm_config_t fsm_config = config;
    state_event_t event = 0xDEADBEEF;

    fsm_config.max_transition_count = (cycles * 2) + 1; // we cap the number of transitions (+1 for the initial transition into state B)

    _setup_on_entry_exit_mock_with_count(state_a_on_entry, SIMPLE_STATE_A, &fsm, fsm_config.context, 1);
    _setup_on_event_mock_with_count(state_a_on_event, SIMPLE_STATE_B, &fsm, &event, fsm_config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_a_on_exit, SIMPLE_STATE_A, &fsm, fsm_config.context, 1);
    _setup_on_entry_exit_mock_with_count(state_b_on_entry, SIMPLE_STATE_C, &fsm, fsm_config.context, cycles + 1);
    _setup_on_entry_exit_mock_with_count(state_b_on_exit, SIMPLE_STATE_B, &fsm, fsm_config.context, cycles);
    _setup_on_entry_exit_mock_with_count(state_c_on_entry, SIMPLE_STATE_B, &fsm, fsm_config.context, cycles);
    _setup_on_entry_exit_mock_with_count(state_c_on_exit, SIMPLE_STATE_C, &fsm, fsm_config.context, cycles);

    // call order
    expect_function_call(state_a_on_entry);
    expect_function_call(state_a_on_event);
    expect_function_call(state_a_on_exit);
    expect_function_call(state_b_on_entry);
    for (size_t idx = 0; idx < cycles; ++idx)
    {
        expect_function_call(state_b_on_exit);
        expect_function_call(state_c_on_entry);
        expect_function_call(state_c_on_exit);
        expect_function_call(state_b_on_entry);
    }

    ret = simple_fsm_init(&fsm, &fsm_config);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_start(&fsm);
    assert_int_equal(ERR_NONE, ret);

    ret = simple_fsm_on_event(&fsm, &event);
    assert_int_equal(ERR_TIMEOUT, ret);
}

int test_simple_fsm_run_tests(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_interface_nulls),
        cmocka_unit_test(test_post_deinit_calls_fail),
        cmocka_unit_test(test_start_bad_fsm_def),
        cmocka_unit_test(test_invalid_config_no_state_delegate),
        cmocka_unit_test(test_invalid_config_no_state_delegate_callbacks),
        cmocka_unit_test(test_invalid_config_no_states),
        cmocka_unit_test(test_invalid_config_bad_max_transition),
        cmocka_unit_test(test_simple_start),
        cmocka_unit_test(test_simple_start_transitions),
        cmocka_unit_test(test_looped_start),
        cmocka_unit_test(test_on_event_transitions),
        cmocka_unit_test(test_looped_transitions),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}