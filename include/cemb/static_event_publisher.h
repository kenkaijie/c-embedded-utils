/**
 * @file Implementation of a statically allocated, statically declared, event publishing system.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "error_codes.h"

typedef struct StaticEventPublisher StaticEventPublisher_t;
typedef struct StaticEventPublisherEntry StaticEventPublisherEntry_t;
typedef struct StaticEventPublisherConfig StaticEventPublisherConfig_t;
typedef void(*StaticEventPublisherCallback_t)(void * context, void const * event_data);

struct StaticEventPublisherEntry
{
    void * context;
    StaticEventPublisherCallback_t callback;
};

struct StaticEventPublisherConfig
{
    StaticEventPublisherEntry_t const * subscribers; /**< Pool of subscribers */
    size_t subscriber_count; /**< Number of subscribers */
};

/**
 * @brief Generic standardized event publisher system.
 *
 * This simple module just implements a rudimentary statically subscribed event. Useful for embedded systems as we can
 * generally do the system bring up once and generally subscribers are statically allocated. See the dynamic event
 * object for a runtime subscriber feature.
 */
struct StaticEventPublisher
{
    StaticEventPublisherConfig_t config;   
};

/**
 * @brief Initialises the event publisher with the provided storage. Note this object pool required an object pool to operate.
 * 
 * @param[in] pub - Pointer to the publisher
 * @param[in] config - The configuration for the event publisher
 * 
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG - Config provided is invalid.
 * 
 * @memberof StaticEventPublisher
 */
ErrorCode_t static_event_publisher_init(StaticEventPublisher_t * pub, StaticEventPublisherConfig_t const * config);

/**
 * @brief De-initialises the event publisher.
 * 
 * @param[in] pub - Pointer to the publisher
 * 
 * @memberof StaticEventPublisher
 */
void static_event_publisher_deinit(StaticEventPublisher_t * pub);

/**
 * @brief Notifies all subscribed entities of a new event. All notifiers are called in the same context as this call. Be warned.
 * 
 * @param[in] pub - Pointer to the publisher
 * @param[in] event_data - The event data to publish, can be NULL.
 * 
 * @memberof StaticEventPublisher
 */
void static_event_publisher_notify_subscribers(StaticEventPublisher_t * pub, void const * event_data);