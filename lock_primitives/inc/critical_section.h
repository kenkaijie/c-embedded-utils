#pragma once

/**
 *  @brief  An interface for a critical section call. Doing this allows us to set atomic like functions by ensuring context switching in multi threaded applications
 *          cannot occur within the confines of this function.
 */

#include "error_codes.h"
#include <stddef.h>

typedef struct s_critical_section critical_section_t;

/**
 *  @brief      Used to enter a critical section. Can be nested if the underlying platform supports it. 
 * 
 *  @param[in] context - The context provided to the interface.
 * 
 */
typedef void(*critical_section_enter_t)(void * context);

/**
 *  @brief      Releases a previously acquired lock.
 * 
 *  @param[in] lock - The context provided to the interface.
 * 
 */
typedef void(*critical_section_exit_t)(void * context);

/**
 *  @brief  An interafce for a locakble property. This is a synchronisation primative.
 */
struct s_critical_section
{
    void * context;
    critical_section_enter_t enter;
    critical_section_exit_t exit;
};

/**
 *  @brief   Validates that the interface is valid. This is basically just a null checker for the interface's members.
 * 
 *  @param[in] interface - Pointer to the interface to check
 * 
 *  @returns    ERR_NONE - interface is valid
 *              ERR_NULL_POINTER - interface null pointer is found
 */
inline error_t critical_section_validate_interface(critical_section_t const * interface)
{
    if ((interface == NULL) || (interface->enter == NULL) || (interface->exit == NULL)) return ERR_NULL_POINTER;
    return ERR_NONE;
}

/**
 *  @brief  Enter a critical section. 
 * 
 *  @param[in] interface
 * 
 *  @returns    ERR_NONE - Success
 *              ERR_NULL_POINTER - Null pointer detected
 */
inline error_t critical_section_enter(critical_section_t const * interface)
{
    error_t ret;
    ret = critical_section_validate_interface(interface);
    if (ret == ERR_NONE)
    {
        interface->enter(interface->context);
    }
    return ret;
}

/**
 *  @brief  Enter a critical section.
 * 
 *  @returns    ERR_NONE - Success
 *              ERR_NULL_POINTER - Null pointer detected
 */
inline error_t critical_section_exit(critical_section_t const * interface)
{
    error_t ret;
    ret = critical_section_validate_interface(interface);
    if (ret == ERR_NONE)
    {
        interface->exit(interface->context);
    }
    return ret;
}
