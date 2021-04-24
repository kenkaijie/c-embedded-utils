#pragma once

/**
 * @brief   Generic standardised event publisher system. This simple module just implements a rudimentary statically subscribed event. Useful for embedded systems as we can generally do the system bringup once only.
 * 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "error_codes.h"

typedef struct s_static_event_publisher static_event_publisher_t;
typedef struct s_static_event_publisher_entry static_event_publisher_entry_t;
typedef struct s_static_event_publisher_config static_event_publisher_config_t;
typedef void(*static_event_publisher_callback_t)(void const * event_data, void * context);

struct s_static_event_publisher_entry
{
    void * context;
    static_event_publisher_callback_t callback;
};

struct s_static_event_publisher_config
{
    static_event_publisher_entry_t const * subscribers; /**< Pointer to an object pool, this pool should be large enough to store event entries. Note this is only copied by reference, the caller MUST maintain livetime of the subscribers list. */
    size_t subscriber_count;
};

struct s_static_event_publisher
{
    static_event_publisher_config_t m_config;   
};

/**
 *  @brief  Initialises the event publisher with the provided storage. Note this object pool required an object pool to operate.
 * 
 *  @param[in] pub - Pointer to the publisher
 *  @param[in] config - The configuration for the event publisher
 * 
 *  @returns    ERR_NONE
 *              ERR_NULL_POINTER
 *              ERR_INVALID_ARG - Config provided is invalid.
 */
error_t static_event_publisher_init(static_event_publisher_t * pub, static_event_publisher_config_t const * config);

/**
 *  @brief  Deinitialises the event publisher.
 * 
 *  @param[in] pub - Pointer to the publisher
 */
void static_event_publisher_deinit(static_event_publisher_t * pub);

/**
 *  @brief  Notifies all subscribed entities of a new event. All notifiers are called in the same context as this call. Be warned.
 * 
 *  @param[in] pub - Pointer to the publisher
 *  @param[in] event_data - The event data to publish.
 */
void static_event_publisher_notify_subcribers(static_event_publisher_t * pub, void const * event_data);