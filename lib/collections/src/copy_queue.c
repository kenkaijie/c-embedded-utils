#include "copy_queue.h"

#include <assert.h>
#include <string.h>

static ErrorCode_t copy_queue_validate_config(CopyQueueConfig_t const * config)
{
    assert(config->queue_buffer);

    if (config->element_size == 0) return ERR_INVALID_ARG;
    if (config->queue_size == 0) return ERR_INVALID_ARG;
    if (config->element_count == 0) return ERR_INVALID_ARG;
    if (config->queue_size < (config->element_count * config->element_size)) return ERR_INVALID_ARG;
    return ERR_NONE;
}

ErrorCode_t copy_queue_init(CopyQueue_t * queue, CopyQueueConfig_t const * config)
{
    assert(queue);
    assert(config);

    ErrorCode_t ret = copy_queue_validate_config(config);
    if (ret != ERR_NONE) return ret;

    queue->buffer = config->queue_buffer;
    queue->element_size = config->element_size;
    queue->max_size = config->element_count;
    queue->count = 0;
    queue->tail_idx = 0;
    queue->head_idx = 0;

    return ret;
}

void copy_queue_deinit(CopyQueue_t * queue)
{
    assert(queue);

    queue->buffer = NULL; 
    queue->element_size = 0;
    queue->max_size = 0;
    queue->count = 0;
    queue->tail_idx = 0;
    queue->head_idx = 0;
}

ErrorCode_t copy_queue_enqueue(CopyQueue_t * queue, void const * element)
{
    assert(queue);
    assert(element);
    
    if (copy_queue_get_remaining(queue) == 0) return ERR_NO_MEM;
    memcpy((void *)&queue->buffer[queue->tail_idx], element, queue->element_size);
    queue->tail_idx += queue->element_size;
    queue->count++;
    return ERR_NONE;
}

ErrorCode_t copy_queue_dequeue(CopyQueue_t * queue, void * element)
{
    assert(queue);
    assert(element);

    if (copy_queue_get_size(queue) == 0) return ERR_EMPTY;
    memcpy(element, (void *)&queue->buffer[queue->head_idx], queue->element_size);
    queue->head_idx += queue->element_size;
    queue->count--;
    return ERR_NONE;
}

ErrorCode_t copy_queue_peek(CopyQueue_t * queue, void * element)
{
    assert(queue);
    assert(element);

    if (copy_queue_get_size(queue) == 0) return ERR_EMPTY;
    memcpy(element, (void *)&queue->buffer[queue->head_idx], queue->element_size);
    return ERR_NONE;
}

size_t copy_queue_get_remaining(CopyQueue_t const * queue)
{
    assert(queue);

    return queue->max_size - queue->count;
}

size_t copy_queue_get_size(CopyQueue_t const * queue)
{
    assert(queue);
    
    return queue->count;
}