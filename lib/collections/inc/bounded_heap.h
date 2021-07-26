/**
 * @file
 * @brief Implementation of a bounded (bounded in size) heap.
 *
 * This has a maximum number of items that can be within the heap. Note this heap does NOT store any data, it simply
 * stores pointers to data that the user provides. The ownership of the items in the heap is the sole responsibility of
 * the owner of the heap.
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "error_codes.h"

typedef struct bounded_heap_config bounded_heap_config_t;
typedef struct bounded_heap bounded_heap_t;
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
typedef bool(*bounded_heap_compare_func_t)(void const * const parent, void const * const child);

struct bounded_heap_config {
    void ** heap_storage;
    size_t element_count;
    bounded_heap_compare_func_t compare;
};

struct bounded_heap {
    bounded_heap_config_t m_config;
    size_t m_items_in_heap;
    size_t m_heap_max_size; /**< for correct deinit, we use this to determine the actual max item size of the heap. */
};

/**
 * @brief  Inits the heap for use
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[in] config - the config to set the heap
 *
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_INVALID_ARG
 */
error_t bounded_heap_init(bounded_heap_t * heap, bounded_heap_config_t const * config);

/**
 * @brief  Deinitialises the heap.
 *
 * Ensures that all heap functions will return sensible values when called past init.
 * 
 * @param[in] heap - pointer to the heap instance
 */
void bounded_heap_deinit(bounded_heap_t * heap);

/**
 * @brief  Peeks at the root item of the heap, may be either max or min, depending on the comparator provided.
 *
 * Does not remove it from the heap.
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[inout] heap_item - The variable to store the root of the heap at.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_EMPTY - The heap is empty
 */
error_t bounded_heap_peek(bounded_heap_t * heap, void ** heap_item);

/**
 * @brief  Similar to peek, but will remove the item from the heap. Once the item is removed,
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[inout] heap_item - The variable to store the root of the heap at.
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_EMPTY - The heap is empty
 */
error_t bounded_heap_pop(bounded_heap_t * heap, void ** heap_item);

/**
 * @brief  Pushes a value into the heap.
 * 
 * @param[in] heap - pointer to the heap instance
 * @param[in] heap_item - The item to push to the heap
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_NO_MEM - The heap is full
 */
error_t bounded_heap_push(bounded_heap_t * heap, void * heap_item);

/**
 * @brief  Gets the number of items that can be placed in the heap.
 * 
 * @returns    Number of items remaining in the heap (i.e the number of free spaces left)
 */
size_t bounded_heap_get_remaining(bounded_heap_t const * heap);

/**
 * @brief  Gets the number of items in the heap.
 * 
 * @returns    The number of items in the heap, aka the count.
 */
size_t bounded_heap_get_size(bounded_heap_t const * heap);