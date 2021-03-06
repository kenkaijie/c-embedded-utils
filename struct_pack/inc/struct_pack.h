/**
 *  @brief  Structure packing for little and big endian. Provides interfacing prototypes for packable data structures.
 */
#pragma once

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

typedef size_t(*packable_encode_handler_t)(void const * element, size_t offset, uint8_t * buffer, size_t buffer_size);
typedef size_t(*packable_decode_handler_t)(void * element, size_t offset, uint8_t const * buffer, size_t buffer_size);

/**
 * @brief   Interface for packing and unpacking. Note to actually properly utilise this "interface", you will likely require 2 additional pieces of information:
 *          void * pointer to the element, as well as size_t for element size. See test example.
 */
typedef struct {
    packable_encode_handler_t encode;
    packable_decode_handler_t decode;
} packable_t;

/**
 *  @brief  Checks if the packable type can fit into the buffer provided, given the offset.
 */
bool pack_is_within_buffer(size_t offset, size_t req_size, size_t data_len);

size_t le_pack_raw(uint8_t const * raw_data, size_t raw_data_size, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_raw(uint8_t * raw_data, size_t raw_data_size, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_bool(bool const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_bool(bool * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_u8(uint8_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_u8(uint8_t * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_u16(uint16_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_u16(uint16_t * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_u32(uint32_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_u32(uint32_t * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_u64(uint64_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_u64(uint64_t * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_s8(int8_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_s8(int8_t * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_s16(int16_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_s16(int16_t * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_s32(int32_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_s32(int32_t * element, size_t offset, uint8_t const * data, size_t data_len);

size_t le_pack_s64(int64_t const * element, size_t offset, uint8_t * data, size_t data_len);
size_t le_unpack_s64(int64_t * element, size_t offset, uint8_t const * data, size_t data_len);
