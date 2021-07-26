/**
 * @file
 * @brief Generic standardized event publisher system.
 *
 * This simple module just implements a rudimentary statically subscribed event. Useful for embedded systems as we can
 * generally do the system bring up once and generally subscribers are statically allocated. See the dynamic event
 * object for a runtime subscriber feature.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "error_codes.h"

typedef struct static_event_publisher static_event_publisher_t;
typedef struct static_event_publisher_entry static_event_publisher_entry_t;
typedef struct static_event_publisher_config static_event_publisher_config_t;
typedef void(*static_event_publisher_callback_t)(void * context, void const * event_data);

struct static_event_publisher_entry
{
    void * context;
    static_event_publisher_callback_t callback;
};

struct static_event_publisher_config
{
    static_event_publisher_entry_t const * subscribers; /**< Pool of subscribers */
    size_t subscriber_count; /**< Number of subscribers */
};

struct static_event_publisher
{
    static_event_publisher_config_t m_config;   
};

/**
 * @brief  Initialises the event publisher with the provided storage. Note this object pool required an object pool to operate.
 * 
 * @param[in] pub - Pointer to the publisher
 * @param[in] config - The configuration for the event publisher
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_INVALID_ARG - Config provided is invalid.
 */
error_t static_event_publisher_init(static_event_publisher_t * pub, static_event_publisher_config_t const * config);

/**
 * @brief  De-initialises the event publisher.
 * 
 * @param[in] pub - Pointer to the publisher
 */
void static_event_publisher_deinit(static_event_publisher_t * pub);

/**
 * @brief  Notifies all subscribed entities of a new event. All notifiers are called in the same context as this call. Be warned.
 * 
 * @param[in] pub - Pointer to the publisher
 * @param[in] event_data - The event data to publish.
 */
void static_event_publisher_notify_subscribers(static_event_publisher_t * pub, void const * event_data);