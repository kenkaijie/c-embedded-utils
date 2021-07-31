/**
 * @file
 * @brief Implementation of a bounded (bounded in size) heap.
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "error_codes.h"

typedef struct BoundedHeapConfig BoundedHeapConfig_t;
typedef struct BoundedHeap BoundedHeap_t;
/**
 * @brief  Compare function for the bounded heap.
 *
 * Notifies the heap if a swap is required. Depending on if this swap is needed for min or max, we can get a min or max
 * heap.
 * 
 * @param[in] parent - The first element to compare
 * @param[in] child - The second element to compare
 * 
 * @retval True - if a swap is needed based on comparing parent with child (in the heap).
 * @retval False
 */
typedef bool(*BoundedHeapCompareFunc_t)(void const * const parent, void const * const child);

/**
 * @brief Configuration values for a #BoundedHeap
 */
struct BoundedHeapConfig {
    void ** heap_storage; /**< Underlying buffer to store the values */
    size_t element_count; /**< The number of elements to store */
    BoundedHeapCompareFunc_t compare; /* Heap's compare function, used to determine ordering. */
};

/**
 * @brief A bounded heap. It is bounded in the number of objects it contains (does not grow).
 * 
 * @note Note this heap does NOT store any data, it simply stores pointers to data that the user provides. The ownership
 * of the items in the heap is the sole responsibility of the owner of the heap.
 */
struct BoundedHeap {
    BoundedHeapConfig_t config; /**<*/
    size_t items_in_heap;
    size_t heap_max_size; /**< for correct deinit, we use this to determine the actual max item size of the heap. */
};

/**
 * @brief  Inits the heap for use
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[in] config - the config to set the heap
 *
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG
 * 
 * @memberof BoundedHeap
 */
ErrorCode_t bounded_heap_init(BoundedHeap_t * heap, BoundedHeapConfig_t const * config);

/**
 * @brief  Deinitialises the heap.
 *
 * Ensures that all heap functions will return sensible values when called past init.
 * 
 * @param[in] heap - pointer to the heap instance
 *
 * @memberof BoundedHeap
 */
void bounded_heap_deinit(BoundedHeap_t * heap);

/**
 * @brief  Peeks at the root item of the heap, may be either max or min, depending on the comparator provided.
 *
 * Does not remove it from the heap.
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[inout] heap_item - The variable to store the root of the heap at.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_EMPTY - The heap is empty
 *
 * @memberof BoundedHeap
 */
ErrorCode_t bounded_heap_peek(BoundedHeap_t * heap, void ** heap_item);

/**
 * @brief Similar to peek, but will remove the item from the heap. Once the item is removed,
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[inout] heap_item - The variable to store the root of the heap at.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_EMPTY - The heap is empty
 *
 * @memberof BoundedHeap
 */
ErrorCode_t bounded_heap_pop(BoundedHeap_t * heap, void ** heap_item);

/**
 * @brief Pushes a value into the heap.
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[in] heap_item - The item to push to the heap, can be a NULL
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NO_MEM - The heap is full
 *
 * @memberof BoundedHeap
 */
ErrorCode_t bounded_heap_push(BoundedHeap_t * heap, void * heap_item);

/**
 * @brief Gets the number of items that can be placed in the heap.
 * 
 * @returns Number of items remaining in the heap (i.e the number of free spaces left)
 *
 * @memberof BoundedHeap
 */
size_t bounded_heap_get_remaining(BoundedHeap_t const * heap);

/**
 * @brief  Gets the number of items in the heap.
 * 
 * @returns    The number of items in the heap, aka the count.
 *
 * @memberof BoundedHeap
 */
size_t bounded_heap_get_size(BoundedHeap_t const * heap);