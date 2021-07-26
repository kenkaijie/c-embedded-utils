#include "copy_queue.h"
#include <string.h>

static error_t copy_queue_validate_config(copy_queue_config_t const * config)
{
    if (config->queue_buffer == NULL) return ERR_NULL_POINTER;
    if (config->element_size == 0) return ERR_INVALID_ARG;
    if (config->queue_size == 0) return ERR_INVALID_ARG;
    if (config->element_count == 0) return ERR_INVALID_ARG;
    if (config->queue_size < (config->element_count * config->element_size)) return ERR_INVALID_ARG;
    return ERR_NONE;
}

error_t copy_queue_init(copy_queue_t * queue, copy_queue_config_t const * config)
{
    if (config == NULL) return ERR_NULL_POINTER;
    error_t ret = copy_queue_validate_config(config);
    if (ret != ERR_NONE) return ret;

    queue->m_buffer = config->queue_buffer;
    queue->m_element_size = config->element_size;
    queue->m_max_size = config->element_count;
    queue->m_count = 0;
    queue->m_tail_idx = 0;
    queue->m_head_idx = 0;

    return ret;
}

void copy_queue_deinit(copy_queue_t * queue)
{
    queue->m_buffer = NULL; 
    queue->m_element_size = 0;
    queue->m_max_size = 0;
    queue->m_count = 0;
    queue->m_tail_idx = 0;
    queue->m_head_idx = 0;
}

error_t copy_queue_enqueue(copy_queue_t * queue, void const * element)
{
    if (element == NULL) return ERR_NULL_POINTER;
    if (copy_queue_get_remaining(queue) == 0) return ERR_NO_MEM;
    memcpy((void *)&queue->m_buffer[queue->m_tail_idx], element, queue->m_element_size);
    queue->m_tail_idx += queue->m_element_size;
    queue->m_count++;
    return ERR_NONE;
}

error_t copy_queue_dequeue(copy_queue_t * queue, void * element)
{
    if (element == NULL) return ERR_NULL_POINTER;
    if (copy_queue_get_size(queue) == 0) return ERR_EMPTY;
    memcpy(element, (void *)&queue->m_buffer[queue->m_head_idx], queue->m_element_size);
    queue->m_head_idx += queue->m_element_size;
    queue->m_count--;
    return ERR_NONE;
}

error_t copy_queue_peek(copy_queue_t * queue, void * element)
{
    if (element == NULL) return ERR_NULL_POINTER;
    if (copy_queue_get_size(queue) == 0) return ERR_EMPTY;
    memcpy(element, (void *)&queue->m_buffer[queue->m_head_idx], queue->m_element_size);
    return ERR_NONE;
}

size_t copy_queue_get_remaining(copy_queue_t const * queue)
{
    return queue->m_max_size - queue->m_count;
}

size_t copy_queue_get_size(copy_queue_t const * queue)
{
    return queue->m_count;
}