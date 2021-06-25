/**
 * @file numeric_ops.h
 * @brief handy numeric operations and general reusable stuff focused on numerical values.
 */

#include "stddef.h"
#include "stdbool.h"

/**
 * @brief Finds if the provided size_t value is a power of 2
 * 
 * @param i 
 * @return true if it's a power of 2
 */
bool numeric_ops_is_power_2_sz(size_t i);