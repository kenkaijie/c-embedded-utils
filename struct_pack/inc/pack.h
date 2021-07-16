/**
 * @file
 * @brief High level packing system header, contains the common functions to provide generic serialisation.
 */
#pragma once

/**
 * @brief   Structure packing for little and big endian. Provides interfacing prototypes for packable data structures.
 * 
 * @todo    Implement Big Endian
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define PACK_SIZE_UINT64_T (8)
#define PACK_SIZE_UINT32_T (4)
#define PACK_SIZE_UINT16_T (2)
#define PACK_SIZE_UINT8_T (1)
#define PACK_SIZE_INT32_T (4)
#define PACK_SIZE_INT64_T (8)
#define PACK_SIZE_INT16_T (2)
#define PACK_SIZE_INT8_T (1)
#define PACK_SIZE_BOOL (1)

/**
 * @brief Serialisation function to pack a given value into the buffer.
 *
 * @param[in] element - Pointer to the item to place
 * @param[in] offset - The offset from the buffer to place this item
 * @param[in] buffer - buffer to place the element in
 * @param[in] buffer_size - size of the buffer to place
 *
 * @returns The number of bytes written to the buffer, or 0 if the buffer is too small or an error has occurred
 */
typedef size_t(*pack_serialise_func_t)(void const * element, size_t offset, uint8_t * buffer, size_t buffer_size);

/**
 * @brief De-serialisation function to extract a specific value from the buffer.
 *
 * @param[in] element - Pointer to the item to place
 * @param[in] offset - The offset from the buffer to place this item
 * @param[in] buffer - buffer to place the element in
 * @param[in] buffer_size - size of the buffer to place
 *
 * @returns The number of bytes written to the buffer, or 0 if the buffer is too small or an error has occurred
 */
typedef size_t(*pack_deserialise_func_t)(void * element, size_t offset, uint8_t const * buffer, size_t buffer_size);

/**
 *  @brief  Checks if the packable type can fit into the buffer provided, given the offset.
 *
 *  @param[in] offset - the offset in the buffer to pack the element
 *  @param[in] req_size - the required size
 *  @param[in] data_len - the size of the buffer (excluding the offset)
 *
 *  @returns True if it fits.
 */
bool pack_is_within_buffer(size_t offset, size_t req_size, size_t data_len);