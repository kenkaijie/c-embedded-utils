#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "error_codes.h"

/**
 * @brief   A simple finite state machine runner. Uses functional callbacks to perform the transitions. This simple state machine only supports
 *          on entry/on exit and a special on event.
 * 
 * @note    Simple FSM cannot fail. To report a failure on simple FSM, you should provide a global catch all error state that all states can transfer
 *          into.
 * 
 *          All state handlers are called within the context of the start or on_event functions. Ideally the FSM should be run on a single thread.
 */
typedef struct s_simple_fsm_state simple_fsm_state_t;
typedef struct s_simple_fsm simple_fsm_t;
typedef struct s_simple_fsm_config simple_fsm_config_t;

typedef size_t(*simple_fsm_on_entry_exit_handler_t)(simple_fsm_t * fsm, void * context);
typedef size_t(*simple_fsm_on_event_handler_t)(simple_fsm_t * fsm, void const * event, void * context);

struct s_simple_fsm_state {
    simple_fsm_on_entry_exit_handler_t on_entry_handler;
    simple_fsm_on_entry_exit_handler_t on_event_handler;
    simple_fsm_on_entry_exit_handler_t on_exit_handler;
};

struct s_simple_fsm_config {
    void * context; /**< Context to call the state functions by. */
    simple_fsm_state_t * states;
    size_t stateCount;
    size_t initialState;
};

struct s_simple_fsm {
    bool mInitialised;
    simple_fsm_config_t mConfig;
    size_t mState;
};

/**
 *  @brief  Returns the current state of the fsm
 * 
 *  @param fsm - the fsm
 *  @param result - result of the get, pass NULL if you do not care about the possible failures
 *                  ERR_NONE - get success
 *                  ERR_NOT_INITIALISED - fsm not initialised, state will not have any meaning
 *  @returns The current state.
 */
size_t simple_fsm_get_current_state(simple_fsm_t * fsm, error_t * result);

/**
 *  @brief   Initialises the fsm based on config
 * 
 *  @param fsm - the fsm
 *  @param config - the configuration for this fsm
 * 
 *  @returns    ERR_NONE - success
 *              ERR_INVALID_ARG - Config was invalid or NULL was passed.
 */
error_t simple_fsm_init(simple_fsm_t * fsm, simple_fsm_config_t const * config);

/**
 *  @brief  Deinits the FSM. This invalidates the configuration and ensures that subsequent calls
 *          to any non init/deinit functions. This function cannot fail.
 * 
 *  @param fsm - the fsm
 * 
 *  @returns    ERR_NONE - success           
 */
error_t simple_fsm_deinit(simple_fsm_t * fsm);

/**
 *  @brief  Runs the FSM, note that any immediate state transitions are run in this context.
 *          This function essentially calls the On Entry for the initial state and propagates any states from there.
 *  
 *  @note   Note this function may transition MANY states depending on the FSM provided.
 * 
 *  @param fsm - the fsm
 * 
 *  @returns    ERR_NONE - success
 *              ERR_NOT_INITIALISED - fsm not initialised
 */
error_t simple_fsm_start(simple_fsm_t * fsm);

/**
 *  @brief   Passes the event to the fsm, which will act on the fsm.
 *  
 *  @note   Note this function may transition MANY states depending on the FSM provided.
 * 
 *  @param fsm - the fsm
 *  @param event - the event to pass through as a pointer
 * 
 *  @returns    ERR_NONE - Everything went well
 *              ERR_NOT_INITIALISED - fsm was not initialised
 */
error_t simple_fsm_on_event(simple_fsm_t * fsm, void const * event);
