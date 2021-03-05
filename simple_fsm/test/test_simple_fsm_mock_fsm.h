/**
 *  @file   This file serves as a mock state to unit test the simple fsm component. It can also serve as an example of how to implement a state machine.
 */
#pragma once

#include "simple_fsm.h"
#include <stddef.h>

/**
 *  @brief  Typically states are defined with an enum with positive only components starting from 0. The last state is a helpful constant which tracks the total number of states.
 */
enum e_simple_states
{
    SIMPLE_STATE_A = 0,
    SIMPLE_STATE_B,
    SIMPLE_STATE_C,
    SIMPLE_STATE_COUNT
};

typedef uint32_t state_event_t;

size_t state_a_on_entry(simple_fsm_t * fsm, void * context);
size_t state_a_on_event(simple_fsm_t * fsm, state_event_t const * event, void * context);
size_t state_a_on_exit(simple_fsm_t * fsm, void * context);

size_t state_b_on_entry(simple_fsm_t * fsm, void * context);
size_t state_b_on_event(simple_fsm_t * fsm, state_event_t const * event, void * context);
size_t state_b_on_exit(simple_fsm_t * fsm, void * context);

size_t state_c_on_entry(simple_fsm_t * fsm, void * context);
size_t state_c_on_event(simple_fsm_t * fsm, state_event_t const * event, void * context);
size_t state_c_on_exit(simple_fsm_t * fsm, void * context);