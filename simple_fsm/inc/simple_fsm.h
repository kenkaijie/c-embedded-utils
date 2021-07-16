/**
 * @file
 * @brief   A simple finite state machine runner.
 *
 * Uses functional callbacks to perform the transitions. This simple state machine only supports on entry/on exit and a
 * special on event.
 *
 * This FSM engine relies on the developer to properly encode each transition within the return values of the
 * callbacks.
 * 
 * All state handlers are called within the context of the start or on_event functions. Ideally the FSM should be run on
 * a single thread.
 * 
 * The flow fo the simple FSM guarantees that the On Entry and On Exits are always called. In the event of the On Entry
 * failing, the On Exit will be called before the new state is transitioned. It is advisable to ensure that if the
 * On Entry has premature return paths, the matching On Exit can still gracefully de-initialise.
 * 
 * This framework does not stop mistakes. Ensure that each state machine's entry and exit error conditions results in
 * the same error state. The engine transitions to the newest received state.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "error_codes.h"

typedef struct s_simple_fsm_state_delegates simple_fsm_state_delegates_t;
typedef struct s_simple_fsm simple_fsm_t;
typedef struct s_simple_fsm_config simple_fsm_config_t;

/**
 * @brief Function prototype for On Entry and On Exit functions used by the FSM.
 *
 * Each function is responsible for its own transitions, typically the entry and exit handlers will either transition to
 * themselves (indicating no issue) or transition to a defined error state.
 *
 * For the case of the On Exit state, passing in it's own state indicates everything is successful.
 *
 * @param[in] fsm - The fsm
 * @param[in] context - User supplied context
 *
 * @returns The next state to transition to, or the value of the current state to indicate execution without errors.
 */
typedef size_t(*simple_fsm_on_entry_exit_handler_t)(simple_fsm_t * fsm, void * context);

/**
 * @brief Function called when the fsm is given an event.
 *
 * @param[in] fsm - The fsm
 * @param[in] event - The event that was dispatched through
 * @param[in] context - User supplied context
 *
 * @returns The next state to transition to, or the value of the current state to stay.
 */
typedef size_t(*simple_fsm_on_event_handler_t)(simple_fsm_t * fsm, void const * event, void * context);

struct s_simple_fsm_state_delegates
{
    simple_fsm_on_entry_exit_handler_t on_entry_handler;
    simple_fsm_on_event_handler_t on_event_handler;
    simple_fsm_on_entry_exit_handler_t on_exit_handler;
};

struct s_simple_fsm_config
{
    void * context; /**< Context to call the state functions by. */
    simple_fsm_state_delegates_t const * state_delegates; /**< The pointer to the array of state function pointers. Must all be non null. */
    size_t state_count; /**< The number of states. */
    size_t initial_state; /**< The initial state whose On Entry will be called when the FSM starts */
    size_t max_transition_count; /**< The maximum transitions any single event can trigger before returning. This prevents endless transitioning between the On Entry/On Exit states in the event of some catastrophic failure on a state machine with connected/strongly connected states. */
};

struct s_simple_fsm
{
    bool m_started;
    simple_fsm_config_t m_config;
    size_t m_state;
    size_t m_loop_count;
};

/**
 * @brief  Returns the current state of the fsm
 * 
 * @param[in] fsm - the fsm
 * @param[out] state - the state, only valid to use if function returns ERR_NONE.
 *                  
 * @returns The current state of the system. If the state machine is not running, this will return the last known state.
 */
size_t simple_fsm_get_current_state(simple_fsm_t * fsm);

/**
 * @brief   Initialises the fsm based on config. If an already initialised fsm calls this, it will force a reset and may cause undefined behaviour.
 * 
 * @param[in] fsm - The fsm
 * @param[in] config - the configuration for this fsm
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_INVALID_ARG - Config was invalid (if a unexpected NULL was found, returns ERR_NULL_POINTER instead)
 */
error_t simple_fsm_init(simple_fsm_t * fsm, simple_fsm_config_t const * config);

/**
 * @brief  Deinits the FSM. Note this does not call the on exit of the current state. Sudden stops must be handled by the user.
 * 
 * @param[in] fsm - The fsm
 */
void simple_fsm_deinit(simple_fsm_t * fsm);

/**
 * @brief Runs the FSM, note that any immediate state transitions are run in this context.
 *
 * This function essentially calls the On Entry for the initial state and propagates any states from there.
 * This function should be called before any calls to on_event.
 * Function is idempotent.
 *  
 * @note This function may transition MANY states depending on the FSM provided.
 * 
 * @param[in] fsm - the fsm
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NOOP - Already started
 * @retval #ERR_TIMEOUT - Max transitions reached, FSM may be in an unknown state and should be reset. (Something bad happened)
 * @retval #ERR_OUT_OF_BOUNDS - a state requested a state outside of the bounds of the defined state machine
 */
error_t simple_fsm_start(simple_fsm_t * fsm);

/**
 * @brief Stops the FSM and attempts to call the on exit of the current state. This will not propagate transitions if the exit handler requests a transition.
 *
 * This is not part of the typical flow of the state machine, but may assist in helping with cleanup in the event of a shutdown. Note a force stop will deinitialise
 * the state machine.
 * Function is idempotent.
 * 
 * @param[in] fsm - the fsm
 *
 * @retval #ERR_NONE
 * @retval #ERR_NOOP - fsm was not started, so this function does nothing
 * @retval #ERR_INCOMPLETE - The on exit handler requested another state other than itself.
 */
error_t simple_fsm_force_stop(simple_fsm_t * fsm);

/**
 * @brief Passes the event to the fsm, which will act on the fsm.
 *  
 * @note Note this function may transition MANY states depending on the FSM provided. Limited by the FSM's max transition setting.
 * 
 * @param[in] fsm - the fsm
 * @param[in] event - the event to pass through as a pointer
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_NOT_INITIALISED - fsm was not started
 * @retval #ERR_TIMEOUT - Max transitions reached, FSM may be in an unknown state and should be reset. (Something bad happened)
 * @retval #ERR_OUT_OF_BOUNDS - a state requested a state outside of the bounds of the defined state machine
 */
error_t simple_fsm_on_event(simple_fsm_t * fsm, void const * event);
