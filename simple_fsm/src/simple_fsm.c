#include "simple_fsm.h"

#include <string.h>

static error_t simple_fsm_resolve_transitions(simple_fsm_t * fsm, size_t next_state);

/**
 *  @brief  Will keep traversing the Exit/Entry states until we settle at a state. Uses the FSM max loop count to 
 *          prevent infinite transitioning (if required).
 */
static error_t simple_fsm_resolve_transitions(simple_fsm_t * fsm, size_t next_state)
{
    simple_fsm_state_delegates_t const * handler;
    error_t ret;
    size_t transitions_count = 0;
    bool is_transition_limit_reached = false;

    while ((next_state != fsm->m_state) && (!is_transition_limit_reached))
    {
        // We need to keep the on exit state, in case we transition elsewhere from here.
        simple_fsm_state_delegates_t const * handler = &fsm->m_config.state_delegates[fsm->m_state];
        size_t on_exit_next_state = handler->on_exit_handler(fsm, fsm->m_config.context);
        // set the state variable to the next state, whether we choose the original next state, or we use the newly requested state,
        size_t next_requested_state = (on_exit_next_state == fsm->m_state) ? next_state : on_exit_next_state;
        // we check here that the new state is within bounds
        if (next_requested_state >= fsm->m_config.state_count) return ERR_OUT_OF_BOUNDS;
        fsm->m_state = next_requested_state;
        handler = &fsm->m_config.state_delegates[fsm->m_state];
        next_state = handler->on_entry_handler(fsm, fsm->m_config.context);

        // we increment the loop cycle count (this loop is a sinlge transition)
        ++transitions_count;
        is_transition_limit_reached = (transitions_count >= fsm->m_config.max_transition_count);
    }

    return (is_transition_limit_reached) ? ERR_TIMEOUT : ERR_NONE;

}

/**
 *  @brief  Validates the contents of the config, ensuring everything is within bounds. THis function reads like a ladder.
 *          If you make it to the end, you must be valid.
 */
static error_t simple_fsm_validate_config(simple_fsm_config_t const * config)
{
    if (config->state_delegates == NULL) return ERR_NULL_POINTER;
    if (config->state_count == 0) return ERR_INVALID_ARG;
    if (config->max_transition_count == 0) return ERR_INVALID_ARG;
    if (config->initial_state >= config->state_count) return ERR_INVALID_ARG;
    for (size_t idx = 0; (idx < config->state_count); ++idx)
    {
        if (config->state_delegates[idx].on_entry_handler == NULL) return ERR_NULL_POINTER;
        if (config->state_delegates[idx].on_event_handler == NULL) return ERR_NULL_POINTER;
        if (config->state_delegates[idx].on_exit_handler == NULL) return ERR_NULL_POINTER;
    }
    return ERR_NONE;
}

size_t simple_fsm_get_current_state(simple_fsm_t * fsm)
{   
    return fsm->m_state;
}

error_t simple_fsm_init(simple_fsm_t * fsm, simple_fsm_config_t const * config)
{
    error_t ret;
    if (config == NULL) return ERR_NULL_POINTER;
    ret = simple_fsm_validate_config(config);
    if (ret != ERR_NONE) return ret;

    fsm->m_config = *config;
    fsm->m_state = fsm->m_config.initial_state;
    fsm->m_started = false;
    return ret;
}

void simple_fsm_deinit(simple_fsm_t * fsm)
{
    fsm->m_state = fsm->m_config.initial_state;
    fsm->m_started = false;
}

error_t simple_fsm_start(simple_fsm_t * fsm)
{
    error_t ret;
    if (fsm->m_started) return ERR_NOOP;
    fsm->m_started = true;
    fsm->m_state = fsm->m_config.initial_state;
    simple_fsm_state_delegates_t const * handler = &fsm->m_config.state_delegates[fsm->m_state];
    size_t next_state = handler->on_entry_handler(fsm, fsm->m_config.context);
    ret = simple_fsm_resolve_transitions(fsm, next_state);
    if (ret != ERR_NONE)
    {
        fsm->m_started = false;
    }
    return ret;
}

error_t simple_fsm_force_stop(simple_fsm_t * fsm)
{
     error_t ret;
    if (!fsm->m_started) return ERR_NOOP;
    simple_fsm_state_delegates_t const * handler = &fsm->m_config.state_delegates[fsm->m_state];
    size_t next_state = handler->on_exit_handler(fsm, fsm->m_config.context);
    // ensure future calls cannot be executed after we force astop.
    fsm->m_started = false;
    return (next_state == fsm->m_state) ? ERR_NONE : ERR_INCOMPLETE;
}

error_t simple_fsm_on_event(simple_fsm_t * fsm, void const * event)
{
     error_t ret;
    if (event == NULL) return ERR_NULL_POINTER;
    if (!fsm->m_started) return ERR_NOT_INITIALISED;
    simple_fsm_state_delegates_t const * handler = &fsm->m_config.state_delegates[fsm->m_state];
    size_t next_state = handler->on_event_handler(fsm, event, fsm->m_config.context);
    return simple_fsm_resolve_transitions(fsm, next_state);
}
