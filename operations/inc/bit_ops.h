/**
 * @file
 * @brief Operations that focus on bits in a value. Assorted bit based operations for now.
 */

#pragma once

#include <stdint.h>

/**
 * @brief Finds the hamming weight of the numeric type. (The number of '1' bits).
 * 
 * For example, 0b11100 has a hamming weight of 3.
 * 
 * @param i
 *
 * @returns The hamming weight of the i argument.
 */
uint32_t bit_ops_hamming_weight_u32(uint32_t i);

