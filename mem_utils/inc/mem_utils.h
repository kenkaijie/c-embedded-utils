#pragma once

#include <stdint.h>
#include <stddef.h>
/**
 *  @file   Helper functions for memory based operations.
 */

/**
 *  @brief  Fills byte based memory blocks with the uint32_t pattern, typically used with 0xDEADBEEF. Note if the size of
 *          the block is not a multiple of 4, the last byte may be truncated. For example, if we fill 3 bytes with the fingerprint
 *          we will get 0xDEADBE.
 * 
 *  @param[in] memory - Pointer to the memory block, must not be NULL
 *  @param[in] size - The amount of memory to fill, must not be 0.
 */
void mem_utils_fill_deadbeef(void * memory, size_t size);
