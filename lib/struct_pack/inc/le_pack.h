/**
 * @file
 * @brief File containing functions for serialising primitive data types.
 */
#pragma once

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "pack.h"

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
