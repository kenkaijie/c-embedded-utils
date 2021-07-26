#include "static_event_publisher.h"

error_t static_event_publisher_validate_config(static_event_publisher_config_t const * config)
{
    if (config->subscriber_count == 0) return ERR_INVALID_ARG;
    if (config->subscribers == NULL) return ERR_NULL_POINTER;
    for (size_t idx = 0; idx < config->subscriber_count; ++idx)
    {
        if (config->subscribers[idx].callback == NULL) return ERR_NULL_POINTER;
    }
    return ERR_NONE;
}

error_t static_event_publisher_init(static_event_publisher_t * pub, static_event_publisher_config_t const * config)
{
    error_t ret;
    if (config == NULL) return ERR_NULL_POINTER;
    ret = static_event_publisher_validate_config(config);
    if (ret != ERR_NONE) return ret;
    pub->m_config = *config;
    return ERR_NONE;
}

void static_event_publisher_deinit(static_event_publisher_t * pub)
{
    pub->m_config.subscriber_count = 0;
}

void static_event_publisher_notify_subscribers(static_event_publisher_t * pub, void const * event_data)
{
    for (size_t idx = 0; idx < pub->m_config.subscriber_count; ++idx)
    {
        pub->m_config.subscribers[idx].callback(pub->m_config.subscribers[idx].context, event_data);
    }
}