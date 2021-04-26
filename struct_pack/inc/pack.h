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

/* Helper typedefs used to specify encoding and decoding functions */
typedef size_t(*pack_encode_func_t)(void const * element, size_t offset, uint8_t * buffer, size_t buffer_size);
typedef size_t(*pack_decode_func_t)(void * element, size_t offset, uint8_t const * buffer, size_t buffer_size);

/**
 *  @brief  Checks if the packable type can fit into the buffer provided, given the offset.
 */
bool pack_is_within_buffer(size_t offset, size_t req_size, size_t data_len);