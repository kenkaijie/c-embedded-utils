/**
 *  @file   A mock implementation of a FSM. Used to test the FSM engine functions as expected.
 */
#pragma once

#include "simple_fsm.h"
#include <stddef.h>

/**
 *  @brief  Typically states are defined with an enum with positive only components starting from 0. The last state is a helpful constant which tracks the total number of states.
 */
enum e_simple_states
{
    MOCK_FSM_STATE_A = 0,
    MOCK_FSM_STATE_B,
    MOCK_FSM_STATE_C,
    MOCK_FSM_STATE_COUNT
};

typedef uint32_t state_event_t;

size_t mock_fsm_state_a_on_entry(simple_fsm_t * fsm, void * context);
size_t mock_fsm_state_a_on_event(simple_fsm_t * fsm, state_event_t const * event, void * context);
size_t mock_fsm_state_a_on_exit(simple_fsm_t * fsm, void * context);

size_t mock_fsm_state_b_on_entry(simple_fsm_t * fsm, void * context);
size_t mock_fsm_state_b_on_event(simple_fsm_t * fsm, state_event_t const * event, void * context);
size_t mock_fsm_state_b_on_exit(simple_fsm_t * fsm, void * context);

size_t mock_fsm_state_c_on_entry(simple_fsm_t * fsm, void * context);
size_t mock_fsm_state_c_on_event(simple_fsm_t * fsm, state_event_t const * event, void * context);
size_t mock_fsm_state_c_on_exit(simple_fsm_t * fsm, void * context);

/**
 * @brief   Helper function to populate a On Entry/On Exit mock call.
 */
#define mock_fsm_setup_on_entry_exit_mock_with_count(function, will_return, with_fsm_val, with_context_val, num_times) \
do { \
    will_return_count(function, will_return, num_times); \
    expect_value_count(function, fsm, (uintptr_t)with_fsm_val, num_times); \
    expect_value_count(function, context, (uintptr_t)with_context_val, num_times); \
} while(false)

/**
 * @brief   Helper function to populate a On Event mock call.
 */
#define mock_fsm_setup_on_event_mock_with_count(function, will_return, with_fsm_val, with_event_val, with_context_val, num_times) \
do { \
    will_return_count(function, will_return, num_times); \
    expect_value_count(function, fsm, (uintptr_t)with_fsm_val, num_times); \
    expect_value_count(function, event, (uintptr_t)with_event_val, num_times); \
    expect_value_count(function, context, (uintptr_t)with_context_val, num_times); \
} while(false)
