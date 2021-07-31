#include "static_event_publisher.h"

#include <assert.h>

ErrorCode_t static_event_publisher_validate_config(StaticEventPublisherConfig_t const * config)
{
    assert(config->subscribers);
    if (config->subscriber_count == 0) return ERR_INVALID_ARG;

    for (size_t idx = 0; idx < config->subscriber_count; ++idx)
    {
        assert(config->subscribers[idx].callback);
    }
    return ERR_NONE;
}

ErrorCode_t static_event_publisher_init(StaticEventPublisher_t * pub, StaticEventPublisherConfig_t const * config)
{
    assert(pub);
    assert(config);

    ErrorCode_t ret;
    ret = static_event_publisher_validate_config(config);
    if (ret != ERR_NONE) return ret;
    pub->config = *config;
    return ERR_NONE;
}

void static_event_publisher_deinit(StaticEventPublisher_t * pub)
{
    assert(pub);

    pub->config.subscriber_count = 0;
}

void static_event_publisher_notify_subscribers(StaticEventPublisher_t * pub, void const * event_data)
{
    assert(pub);

    for (size_t idx = 0; idx < pub->config.subscriber_count; ++idx)
    {
        pub->config.subscribers[idx].callback(pub->config.subscribers[idx].context, event_data);
    }
}