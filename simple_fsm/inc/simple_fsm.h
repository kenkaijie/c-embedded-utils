#pragma once

/**
 * @brief   A simple finite state machine runner. Uses functional callbacks to perform the transitions. This simple state machine only supports
 *          on entry/on exit and a special on event.
 * 
 * @note    Simple FSM cannot fail. To report a failure on simple FSM, you should provide a global catch all error state that all states can transfer
 *          into.
 * 
 *          All state handlers are called within the context of the start or on_event functions. Ideally the FSM should be run on a single thread.
 * 
 *          The flow fo the simple FSM gurantees that the On Entry and On Exits are always called. In the event of the On Entry failing, the On Exit
 *          will be called before the new state is transitioned. It is advisible to ensure that if the On Entry has premature return paths, the matching
 *          On Exit can still gracefully deinitialise. 
 * 
 *          This framework does not stop mistakes. Ensure that each state machine's entry and exit error conditions results in the same error state. The engine
 *          transitions to the newest received state.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "error_codes.h"

typedef struct s_simple_fsm_state_delegates simple_fsm_state_delegates_t;
typedef struct s_simple_fsm simple_fsm_t;
typedef struct s_simple_fsm_config simple_fsm_config_t;

/**
 * Function that is called during On Entry or On Exit. Return the current state to indicate success, or a different state to request a transition.
 */
typedef size_t(*simple_fsm_on_entry_exit_handler_t)(simple_fsm_t * fsm, void * context);

/**
 * Function called when the fsm is given an event. Return the next state. If the current state is the next state, no additional transitions will occur.
 */
typedef size_t(*simple_fsm_on_event_handler_t)(simple_fsm_t * fsm, void const * event, void * context);

struct s_simple_fsm_state_delegates {
    simple_fsm_on_entry_exit_handler_t on_entry_handler;
    simple_fsm_on_event_handler_t on_event_handler;
    simple_fsm_on_entry_exit_handler_t on_exit_handler;
};

struct s_simple_fsm_config {
    void * context; /**< Context to call the state functions by. */
    simple_fsm_state_delegates_t const * state_delegates; /**< The pointer to the array of state function pointers. Must all be non null. */
    size_t state_count; /**< The number of states. */
    size_t initial_state; /**< The initial state whose On Entry will be called when the FSM starts */
    size_t max_transition_count; /**< The maximum transitions any single event can trigger before returning. This prevents endless transitioning between the On Entry/On Exit states in the event of some catastrophic failure on a state machine with connected/strongly connected states. */
};

struct s_simple_fsm {
    bool m_initialised;
    simple_fsm_config_t m_config;
    size_t m_state;
    size_t m_loop_count;
};

/**
 *  @brief  Returns the current state of the fsm
 * 
 *  @param[in]  fsm - the fsm
 *  @param[out]  state - the state, only valid to use if function returns ERR_NONE.
 *                  
 *  @returns    ERR_NONE - get success
 *              ERR_NULL_POINTER - a null pointer was found
 *              ERR_NOT_INITIALISED - fsm not initialised, state will not have any meaning
 */
error_t simple_fsm_get_current_state(simple_fsm_t * fsm, size_t * state);

/**
 *  @brief   Initialises the fsm based on config. If an already initialised fsm calls this, it will force a reset and may cause undefined behaviour.
 * 
 *  @param[in]  fsm - the fsm
 *  @param[in]  config - the configuration for this fsm
 * 
 *  @returns    ERR_NONE - success
 *              ERR_NULL_POINTER - a null pointer was found
 *              ERR_INVALID_ARG - Config was invalid (if a unexpected NULL was found, returns ERR_NULL_POINTER instead)
 */
error_t simple_fsm_init(simple_fsm_t * fsm, simple_fsm_config_t const * config);

/**
 *  @brief  Deinits the FSM. This invalidates the configuration and ensures that subsequent calls
 *          to any non init/deinit functions. This function cannot fail.
 * 
 *  @param[in]  fsm - the fsm
 * 
 *  @returns    ERR_NONE - success  
 *              ERR_NULL_POINTER - a null pointer was found    
 */
error_t simple_fsm_deinit(simple_fsm_t * fsm);

/**
 *  @brief  Runs the FSM, note that any immediate state transitions are run in this context.
 *          This function essentially calls the On Entry for the initial state and propagates any states from there.
 *  
 *  @note   Note this function may transition MANY states depending on the FSM provided.
 * 
 *  @param[in]  fsm - the fsm
 * 
 *  @returns    ERR_NONE - success
 *              ERR_NOT_INITIALISED - fsm not initialised
 *              ERR_TIMEOUT - Max transitions reached, FSM may be in an unknown state and should be reset. (Something bad happened)
 *              ERR_NULL_POINTER - a null pointer was found
 *              ERR_OUT_OF_BOUNDS - a state requested a state outside of the bounds of the defined state machine
 */
error_t simple_fsm_start(simple_fsm_t * fsm);

/**
 *  @brief   Passes the event to the fsm, which will act on the fsm.
 *  
 *  @note   Note this function may transition MANY states depending on the FSM provided. Limited by the FSM's max transition setting.
 * 
 *  @param[in]  fsm - the fsm
 *  @param[in]  event - the event to pass through as a pointer
 * 
 *  @returns    ERR_NONE - Everything went well
 *              ERR_NOT_INITIALISED - fsm was not initialised
 *              ERR_TIMEOUT - Max transitions reached, FSM may be in an unknown state and should be reset. (Something bad happened)
 *              ERR_NULL_POINTER - a null pointer was found
 *              ERR_OUT_OF_BOUNDS - a state requested a state outside of the bounds of the defined state machine
 */
error_t simple_fsm_on_event(simple_fsm_t * fsm, void const * event);
