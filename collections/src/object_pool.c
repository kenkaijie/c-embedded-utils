#include "object_pool.h"

extern inline error_t object_pool_allocate(object_pool_t const * interface, void ** token);
extern inline error_t object_pool_deallocate(object_pool_t const * interface, void ** token);
extern inline size_t object_pool_get_available_count(object_pool_t const * interface);