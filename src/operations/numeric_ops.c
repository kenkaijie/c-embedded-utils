#include <cemb/numeric_ops.h>

bool numeric_ops_is_power_2_sz(size_t i)
{
    return ((i & (i-1)) == 0) && (i != 0);
}