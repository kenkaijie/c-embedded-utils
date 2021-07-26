#include "bounded_heap.h"

static error_t bounded_heap_validate_config(bounded_heap_config_t const * config)
{
    if (config->heap_storage == NULL) return ERR_NULL_POINTER;
    if (config->compare == NULL) return ERR_NULL_POINTER;
    if (config->element_count == 0) return ERR_INVALID_ARG;
    return ERR_NONE;
}

/**
 *  @brief  A heap's left most empty position is determined purely by the number of items in the heap.
 */
static size_t bounded_heap_get_next_empty_index(bounded_heap_t * heap)
{
    return heap->m_items_in_heap;
}

static size_t bounded_heap_get_index_of_parent(bounded_heap_t * heap, size_t index)
{
    return (index-1)/2;
}

static size_t bounded_heap_get_index_of_left_child(bounded_heap_t * heap, size_t index)
{
    return (2*index)+1;
}

static size_t bounded_heap_get_index_of_right_child(bounded_heap_t * heap, size_t index)
{
    return (2*index)+2;
}

static void bounded_heap_swap_values_at_index(bounded_heap_t * heap, size_t index_a, size_t index_b)
{
    void * tmp = heap->m_config.heap_storage[index_a];
    heap->m_config.heap_storage[index_a] = heap->m_config.heap_storage[index_b];
    heap->m_config.heap_storage[index_b] = tmp;
}

static void * bounded_heap_get_value_at(bounded_heap_t * heap, size_t index)
{
    return heap->m_config.heap_storage[index];
}

error_t bounded_heap_init(bounded_heap_t * heap, bounded_heap_config_t const * config)
{
    if (config == NULL) return ERR_NULL_POINTER;
    error_t ret = bounded_heap_validate_config(config);
    if (ret != ERR_NONE) return ret;

    heap->m_config = *config;
    heap->m_heap_max_size = config->element_count;
    heap->m_items_in_heap = 0;
    return ret;
}

void bounded_heap_deinit(bounded_heap_t * heap)
{
    heap->m_items_in_heap = 0;
    heap->m_heap_max_size = 0;
}

error_t bounded_heap_peek(bounded_heap_t * heap, void ** heap_item)
{
    if (heap_item == NULL) return ERR_NULL_POINTER;
    if (bounded_heap_get_size(heap) == 0) return ERR_EMPTY;
    
    *heap_item = heap->m_config.heap_storage[0];
    return ERR_NONE;
}

error_t bounded_heap_pop(bounded_heap_t * heap, void ** heap_item)
{
    if (heap_item == NULL) return ERR_NULL_POINTER;
    if (bounded_heap_get_size(heap) == 0) return ERR_EMPTY;

    // give user the top of heap
    *heap_item = heap->m_config.heap_storage[0];
    heap->m_items_in_heap--;

    // swap the first value and the end of the heap
    bounded_heap_swap_values_at_index(heap, 0, bounded_heap_get_next_empty_index(heap));

    // now we heapify
    size_t parent_idx = 0;

    bool done = false;
    while (!done)
    {
        size_t left_child_idx = bounded_heap_get_index_of_left_child(heap, parent_idx);
        size_t right_child_idx = bounded_heap_get_index_of_right_child(heap, parent_idx);

        if ((left_child_idx >= bounded_heap_get_size(heap)) || (right_child_idx >= bounded_heap_get_size(heap))) break;

        bool left_child_swap = heap->m_config.compare(bounded_heap_get_value_at(heap, parent_idx),
                                                      bounded_heap_get_value_at(heap, left_child_idx));
        bool right_child_swap = heap->m_config.compare(bounded_heap_get_value_at(heap, parent_idx),
                                                       bounded_heap_get_value_at(heap, right_child_idx));

        if (left_child_swap && right_child_swap)
        {
            /** 
             * Swap Arbitration
             * If both need to be swapped we just check what happens if we swap with the left. If we swap with the left
             * (left = parent) and we check with the right child idx and a swap still needs to be done, we can just swap
             * the original parent with the right to save a swap
             */
            
            bool swap_with_right = heap->m_config.compare(bounded_heap_get_value_at(heap, left_child_idx),
                                                          bounded_heap_get_value_at(heap, right_child_idx));
            left_child_swap = !swap_with_right;
            right_child_swap = swap_with_right;
        }
        
        if (left_child_swap)
        {
            bounded_heap_swap_values_at_index(heap, parent_idx, left_child_idx);
            parent_idx = left_child_idx;
        }
        else if (right_child_swap)
        {
            bounded_heap_swap_values_at_index(heap, parent_idx, right_child_idx);
            parent_idx = right_child_idx;
        }
        else
        {
            // do swaps needed, we are done
            done = true;
        }
    }

    return ERR_NONE;
}

error_t bounded_heap_push(bounded_heap_t * heap, void * heap_item)
{
    if (bounded_heap_get_remaining(heap) == 0) return ERR_NO_MEM;
    
    size_t child_idx = bounded_heap_get_next_empty_index(heap);

    heap->m_config.heap_storage[child_idx] = heap_item;
    heap->m_items_in_heap++;

    // now we bubble up, we dont have to do anything if we only have 1 item;

    bool done = (bounded_heap_get_size(heap) == 1);

    while (!done)
    {
        if (child_idx == 0) break;
        
        size_t parent_idx = bounded_heap_get_index_of_parent(heap, child_idx);

        if (heap->m_config.compare(bounded_heap_get_value_at(heap, parent_idx),
                                   bounded_heap_get_value_at(heap, child_idx)))
        {
            bounded_heap_swap_values_at_index(heap, parent_idx, child_idx);
            child_idx = parent_idx;
        }
        else
        {
            done = true;
        }
    }
    
    return ERR_NONE;
}

size_t bounded_heap_get_remaining(bounded_heap_t const * heap)
{
    return heap->m_heap_max_size - heap->m_items_in_heap;
}

size_t bounded_heap_get_size(bounded_heap_t const * heap)
{
    return heap->m_items_in_heap;
}