#pragma once

enum e_error_codes
{
    ERR_NONE = 0, /**< Success */
    ERR_GENERIC_ERROR, /**< Use this if nothing else describes and the error is too specific to justify an entry. */
    ERR_NOT_INITIALISED, /**< Function calls are performed before an intialise. */
    ERR_INVALID_STATE, /**< The function was called at an invalid state. */
    ERR_NO_MEM, /**< There is insufficient memeory to process the request. */
    ERR_INVALID_ARG, /**< A provided argument is not within the acceptable domain */
    ERR_TIMEOUT, /**< A timed request took longer than usual */
};

typedef uint32_t error_t;

