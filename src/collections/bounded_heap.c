#include <cemb/bounded_heap.h>

#include <assert.h>

static ErrorCode_t bounded_heap_validate_config(BoundedHeapConfig_t const * config)
{
    assert(config->heap_storage);
    assert(config->compare);
    if (config->element_count == 0) return ERR_INVALID_ARG;
    return ERR_NONE;
}

static size_t bounded_heap_get_next_empty_index(BoundedHeap_t * heap)
{
    /* A heap's left most empty position is determined purely by the number of items in the heap. */
    return heap->items_in_heap;
}

static size_t bounded_heap_get_index_of_parent(size_t index)
{
    return (index-1)/2;
}

static size_t bounded_heap_get_index_of_left_child(size_t index)
{
    return (2*index)+1;
}

static size_t bounded_heap_get_index_of_right_child(size_t index)
{
    return (2*index)+2;
}

static void bounded_heap_swap_values_at_index(BoundedHeap_t * heap, size_t index_a, size_t index_b)
{
    void * tmp = heap->config.heap_storage[index_a];
    heap->config.heap_storage[index_a] = heap->config.heap_storage[index_b];
    heap->config.heap_storage[index_b] = tmp;
}

static void * bounded_heap_get_value_at(BoundedHeap_t * heap, size_t index)
{
    return heap->config.heap_storage[index];
}

ErrorCode_t bounded_heap_init(BoundedHeap_t * heap, BoundedHeapConfig_t const * config)
{
    assert(heap);
    assert(config);

    ErrorCode_t ret = bounded_heap_validate_config(config);
    if (ret != ERR_NONE) return ret;

    heap->config = *config;
    heap->heap_max_size = config->element_count;
    heap->items_in_heap = 0;
    return ret;
}

void bounded_heap_deinit(BoundedHeap_t * heap)
{
    assert(heap);

    heap->items_in_heap = 0;
    heap->heap_max_size = 0;
}

ErrorCode_t bounded_heap_peek(BoundedHeap_t * heap, void ** heap_item)
{
    assert(heap);

    if (bounded_heap_get_size(heap) == 0) return ERR_EMPTY;
    
    *heap_item = heap->config.heap_storage[0];
    return ERR_NONE;
}

ErrorCode_t bounded_heap_pop(BoundedHeap_t * heap, void ** heap_item)
{
    assert(heap);
    assert(heap_item);

    if (bounded_heap_get_size(heap) == 0) return ERR_EMPTY;

    // give user the top of heap
    *heap_item = heap->config.heap_storage[0];
    heap->items_in_heap--;

    // swap the first value and the end of the heap
    bounded_heap_swap_values_at_index(heap, 0, bounded_heap_get_next_empty_index(heap));

    // now we heapify
    size_t parent_idx = 0;

    bool done = false;
    while (!done)
    {
        size_t left_child_idx = bounded_heap_get_index_of_left_child(parent_idx);
        size_t right_child_idx = bounded_heap_get_index_of_right_child(parent_idx);

        if ((left_child_idx >= bounded_heap_get_size(heap)) || (right_child_idx >= bounded_heap_get_size(heap))) break;

        bool left_child_swap = heap->config.compare(bounded_heap_get_value_at(heap, parent_idx),
                                                      bounded_heap_get_value_at(heap, left_child_idx));
        bool right_child_swap = heap->config.compare(bounded_heap_get_value_at(heap, parent_idx),
                                                       bounded_heap_get_value_at(heap, right_child_idx));

        if (left_child_swap && right_child_swap)
        {
            /** 
             * Swap Arbitration
             * If both need to be swapped we just check what happens if we swap with the left. If we swap with the left
             * (left = parent) and we check with the right child idx and a swap still needs to be done, we can just swap
             * the original parent with the right to save a swap
             */
            
            bool swap_with_right = heap->config.compare(bounded_heap_get_value_at(heap, left_child_idx),
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

ErrorCode_t bounded_heap_push(BoundedHeap_t * heap, void * heap_item)
{
    assert(heap);

    if (bounded_heap_get_remaining(heap) == 0) return ERR_NO_MEM;
    
    size_t child_idx = bounded_heap_get_next_empty_index(heap);

    heap->config.heap_storage[child_idx] = heap_item;
    heap->items_in_heap++;

    // now we bubble up, we dont have to do anything if we only have 1 item;

    bool done = (bounded_heap_get_size(heap) == 1);

    while (!done)
    {
        if (child_idx == 0) break;
        
        size_t parent_idx = bounded_heap_get_index_of_parent(child_idx);

        if (heap->config.compare(bounded_heap_get_value_at(heap, parent_idx),
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

size_t bounded_heap_get_remaining(BoundedHeap_t const * heap)
{
    assert(heap);

    return heap->heap_max_size - heap->items_in_heap;
}

size_t bounded_heap_get_size(BoundedHeap_t const * heap)
{
    assert(heap);

    return heap->items_in_heap;
}