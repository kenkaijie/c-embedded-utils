#include "critical_section.h"

#include <stddef.h>

error_t critical_section_enter(critical_section_t * interface)
{
    if (interface == NULL) return ERR_NULL_POINTER;
    // we check both because we don't want the exit to fail after we enter.
    if (interface->enter == NULL) return ERR_NULL_POINTER;
    if (interface->exit == NULL) return ERR_NULL_POINTER;

    interface->enter(interface->context);

    return ERR_NONE;
}

error_t critical_section_exit(critical_section_t * interface)
{
    if (interface == NULL) return ERR_NULL_POINTER;

    // The interface should have both pointers defined.
    if (interface->enter == NULL) return ERR_NULL_POINTER;
    if (interface->exit == NULL) return ERR_NULL_POINTER;

    interface->exit(interface->context);

    return ERR_NONE;
}