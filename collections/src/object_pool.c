#include "object_pool.h"

inline error_t object_pool_allocate(object_pool_t const * interface, void ** object_ptr)
{
    return interface->allocate(interface->context, object_ptr);
}

inline error_t object_pool_deallocate(object_pool_t const * interface, void ** object_ptr)
{
    return interface->deallocate(interface->context, object_ptr);
}

inline size_t object_pool_get_available_count(object_pool_t const * interface)
{
    return interface->get_available_count(interface->context);
}
