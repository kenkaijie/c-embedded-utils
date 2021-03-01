#include "simple_fsm.h"

static error_t _validate_config(simple_fsm_config_t const * config)
{
    bool is_valid = true;
    is_valid &= (config != NULL);
    // can only do these after the null checking
    if (is_valid)
    {
        is_valid &= (config->states != NULL);
        is_valid &= (config->stateCount != 0);
    }
    return (is_valid) ? ERR_NONE : ERR_INVALID_ARG;
}

size_t simple_fsm_get_current_state(simple_fsm_t * fsm, error_t * result)
{
    *result = (fsm->mInitialised) ? ERR_NONE : ERR_NOT_INITIALISED;
    return fsm->mState;
}

error_t simple_fsm_init(simple_fsm_t * fsm, simple_fsm_config_t const * config)
{
    error_t ret = _validate_config(config);
    if (ret != ERR_NONE) { return ret; }

    fsm->mConfig = *config;
    fsm->mState = fsm->mConfig.initialState;
    fsm->mInitialised = true;
    return ret;
}

error_t simple_fsm_deinit(simple_fsm_t * fsm)
{
    fsm->mInitialised = false;
    return ERR_NONE;
}

error_t simple_fsm_start(simple_fsm_t * fsm)
{
    if (!fsm->mInitialised) return ERR_NOT_INITIALISED;

    return ERR_NONE;
}

error_t simple_fsm_on_event(simple_fsm_t * fsm, void const * event)
{
    if (!fsm->mInitialised) return ERR_NOT_INITIALISED;

    return ERR_NONE;
}