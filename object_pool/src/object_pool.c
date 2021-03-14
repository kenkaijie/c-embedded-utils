#include "object_pool.h"

extern inline error_t object_pool_validate_interface(object_pool_t const * interface);
extern inline error_t object_pool_allocate(object_pool_t const * interface, size_t * token);
extern inline error_t object_pool_fetch(object_pool_t const * interface, size_t token, void ** object_pointer);
extern inline error_t object_pool_deallocate(object_pool_t const * interface, size_t * token);
extern inline error_t object_pool_get_unused_count(object_pool_t const * interface, size_t * unused_count);