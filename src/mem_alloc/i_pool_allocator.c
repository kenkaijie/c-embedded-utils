#include <cemb/i_pool_allocator.h>

#include <assert.h>

inline ErrorCode_t i_pool_allocator_allocate(IPoolAllocator_t const * interface, void ** object_ptr)
{
    assert(interface);
    assert(object_ptr);

    return interface->allocate(interface->context, object_ptr);
}

inline ErrorCode_t i_pool_allocator_deallocate(IPoolAllocator_t const * interface, void ** object_ptr)
{
    assert(interface);
    assert(object_ptr);

    return interface->deallocate(interface->context, object_ptr);
}

inline size_t i_pool_allocator_get_available_count(IPoolAllocator_t const * interface)
{
    assert(interface);

    return interface->get_available_count(interface->context);
}
