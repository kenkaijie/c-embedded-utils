#include "simple_fsm.h"

#include <string.h>
#include <assert.h>

/**
 *  @brief  Will keep traversing the Exit/Entry states until we settle at a state. Uses the FSM max loop count to 
 *          prevent infinite transitioning (if required).
 */
static ErrorCode_t simple_fsm_resolve_transitions(SimpleFSM_t * fsm, size_t next_state)
{
    SimpleFSMStateDelegates_t const * handler;
    ErrorCode_t ret;
    size_t transitions_count = 0;
    bool is_transition_limit_reached = false;

    while ((next_state != fsm->state) && (!is_transition_limit_reached))
    {
        // We need to keep the on exit state, in case we transition elsewhere from here.
        SimpleFSMStateDelegates_t const * handler = &fsm->config.state_delegates[fsm->state];
        size_t on_exit_next_state = handler->on_exit_handler(fsm, fsm->config.context);
        // set the state variable to the next state, whether we choose the original next state, or we use the newly requested state,
        size_t next_requested_state = (on_exit_next_state == fsm->state) ? next_state : on_exit_next_state;
        // we check here that the new state is within bounds
        if (next_requested_state >= fsm->config.state_count) return ERR_OUT_OF_BOUNDS;
        fsm->state = next_requested_state;
        handler = &fsm->config.state_delegates[fsm->state];
        next_state = handler->on_entry_handler(fsm, fsm->config.context);

        // we increment the loop cycle count (this loop is a sinlge transition)
        ++transitions_count;
        is_transition_limit_reached = (transitions_count >= fsm->config.max_transition_count);
    }

    return (is_transition_limit_reached) ? ERR_TIMEOUT : ERR_NONE;

}

/**
 *  @brief  Validates the contents of the config, ensuring everything is within bounds. THis function reads like a ladder.
 *          If you make it to the end, you must be valid.
 */
static ErrorCode_t simple_fsm_validate_config(SimpleFSMConfig_t const * config)
{
    assert(config->state_delegates);


    if (config->state_count == 0) return ERR_INVALID_ARG;
    if (config->max_transition_count == 0) return ERR_INVALID_ARG;
    if (config->initial_state >= config->state_count) return ERR_INVALID_ARG;
    for (size_t idx = 0; (idx < config->state_count); ++idx)
    {
        assert(config->state_delegates[idx].on_entry_handler);
        assert(config->state_delegates[idx].on_event_handler);
        assert(config->state_delegates[idx].on_exit_handler);
    }
    return ERR_NONE;
}

size_t simple_fsm_get_current_state(SimpleFSM_t * fsm)
{   
    assert(fsm);

    return fsm->state;
}

ErrorCode_t simple_fsm_init(SimpleFSM_t * fsm, SimpleFSMConfig_t const * config)
{
    assert(fsm);
    assert(config);

    ErrorCode_t ret;
    ret = simple_fsm_validate_config(config);
    if (ret != ERR_NONE) return ret;

    fsm->config = *config;
    fsm->state = fsm->config.initial_state;
    fsm->started = false;
    return ret;
}

void simple_fsm_deinit(SimpleFSM_t * fsm)
{
    assert(fsm);

    fsm->state = fsm->config.initial_state;
    fsm->started = false;
}

ErrorCode_t simple_fsm_start(SimpleFSM_t * fsm)
{
    assert(fsm);

    ErrorCode_t ret;
    if (fsm->started) return ERR_NOOP;
    fsm->started = true;
    fsm->state = fsm->config.initial_state;
    SimpleFSMStateDelegates_t const * handler = &fsm->config.state_delegates[fsm->state];
    size_t next_state = handler->on_entry_handler(fsm, fsm->config.context);
    ret = simple_fsm_resolve_transitions(fsm, next_state);
    if (ret != ERR_NONE)
    {
        fsm->started = false;
    }
    return ret;
}

ErrorCode_t simple_fsm_force_stop(SimpleFSM_t * fsm)
{
    assert(fsm);

    ErrorCode_t ret;
    if (!fsm->started) return ERR_NOOP;
    SimpleFSMStateDelegates_t const * handler = &fsm->config.state_delegates[fsm->state];
    size_t next_state = handler->on_exit_handler(fsm, fsm->config.context);
    // ensure future calls cannot be executed after we force astop.
    fsm->started = false;
    return (next_state == fsm->state) ? ERR_NONE : ERR_INCOMPLETE;
}

ErrorCode_t simple_fsm_on_event(SimpleFSM_t * fsm, void const * event)
{
    assert(fsm);

    ErrorCode_t ret;
    if (!fsm->started) return ERR_NOT_INITIALISED;
    SimpleFSMStateDelegates_t const * handler = &fsm->config.state_delegates[fsm->state];
    size_t next_state = handler->on_event_handler(fsm, event, fsm->config.context);
    return simple_fsm_resolve_transitions(fsm, next_state);
}
