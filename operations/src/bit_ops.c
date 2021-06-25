#include "bit_ops.h"

/**
 * We use the implmentation observed here. This will optimise to the intrinsic machine instruction if available (apparently).
 * @see https://stackoverflow.com/a/109025
 */
uint32_t bit_ops_hamming_weight_u32(uint32_t i)
{
    i = i - ((i >> 1) & 0x55555555);        // add pairs of bits
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);  // quads
    i = (i + (i >> 4)) & 0x0F0F0F0F;        // groups of 8
    return (i * 0x01010101) >> 24;          // horizontal sum of bytes
}