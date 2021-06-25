#pragma once

/**
 *  @file   List of result and error codes used in all libraries that are part of this collection of utilities.
 *          As a rule for this library, we use up to the first chunk of error codes in a particular application, with the remaining codes
 *          to be used for application specific error codes.
 */

#include <stdint.h>

enum e_error_codes
{
    ERR_NONE = 0, /**< Success */
    ERR_GENERIC_ERROR, /**< Use this if nothing else describes and the error is too specific to justify an entry. */
    ERR_NOT_INITIALISED, /**< Function calls are performed before an intialise. */
    ERR_INVALID_STATE, /**< The function was called at an invalid state. */
    ERR_NO_MEM, /**< There is insufficient memeory to process the request. */
    ERR_INVALID_ARG, /**< A provided argument is not within the acceptable domain */
    ERR_TIMEOUT, /**< A timed request took longer than usual */
    ERR_OUT_OF_BOUNDS, /**< A requested value is not within the acceptable bounds (typically for arrays, or addressable values) */
    ERR_NULL_POINTER, /**< A null pointer was seen where it was not expected. */
    ERR_NOOP, /**< Used to signal that an optionally triggered function did not do anything, not actually an error */
    ERR_INCOMPLETE, /**< Function had to return early, but would have done more (used for indicating if an abort stopped prematurely) */
    ERR_EMPTY, /**< Typically for data structures, indicates there is no values within the collection. */
    ERR_NOT_FOUND, /**< Typically used for search functions to indicate an item was not found. */
    ERR_OVERRIDE, /**< Indicates a value in a queue of buffer has been overidden, not an error, but may be of interest to the user. */
    ERR_BUSY, /**< System is busy at this point to process the request (typically for non blocking functions) */
    ERR_NOT_IMPLEMENTED, /**< Operation is not supported */
};

typedef uint32_t error_t;

