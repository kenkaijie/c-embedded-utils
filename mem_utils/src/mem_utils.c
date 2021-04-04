#include "mem_utils.h"

void mem_utils_fill_deadbeef(void * memory, size_t size)
{
    static const uint8_t fingerprint[] = {0xDE, 0xAD, 0xBE, 0xEF};
    for (size_t idx = 0; idx < size; ++idx)
    {
        ((uint8_t *)memory)[idx] = fingerprint[idx & 0x3];
    }
}