/**
 * @file
 * @brief A generator based binary search.
 * 
 * This yields back control to the caller at each interation of the search. It also delegates the
 * responsibility of accessing the value at the underlying index.
 * 
 * Typical use for searching through a list of integers.
 * 
 * int int_array[100] = {....};
 * 
 * bin_search_iterator_t iter;
 * 
 * bin_search_interator_init(&iter, 0, 100);
 * 
 * 
 * 
 * while (bin_search_status(&iter)==BIN_SEARCH_CONTINUE))
 * {
 *     int32_t index_to_search = 0;
 *     bsearch_get_index(&iter, &index_to_search)
 *     .. perform compare
 *      int value = int_array[index_to_search];
 * 
 *     bin_search_notify(&iter, BIN_SEARCH_NOTIFY_LEFT);
 * }
 * 
 * You can of course do something else or iterate the search without the use of a loop.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <cemb/error_codes.h>

/**
 * @brief The search status.
 */
typedef enum BsearchIteratorStatus
{
    BSEARCH_CONTINUE, /**< The search has no completed. */
    BSEARCH_DONE_NOT_FOUND, /**< The search has completed, the item has not been found. */
    BSEARCH_DONE_FOUND, /**< The search has completed, the item has been found */
} BsearchIteratorStatus_t;

/**
 * @brief Notifications the user can provide the system in order for it to progress the search.
 */
typedef enum BsearchNotification
{
    BSEARCH_NOTIFY_LEFT, /**< Notifies the iterator that the element is towards the left of this element (left being towards index 0) */
    BSEARCH_NOTIFY_RIGHT, /**< Notifies the iterator that the element is towards the right of this element (right being towards index infinity)  */
    BSEARCH_NOTIFY_EQ, /**< Notifies the iterator that the value here is equal */
    BSEARCH_NOTIFY_ERR_SKIP, /**< Notifies the iterator that the value cannot be fetched for this index, and must be skipped. */
    BSEARCH_NOTIFY_ERR_ABORT, /**< Notifies the interator that the value cannot be fetched, and no other value can be fetched. */
} BsearchNotification_t;

/**
 * @brief The structure for iterative searching.
 */
typedef struct BsearchIterator
{
    BsearchIteratorStatus_t status;

    int32_t left_idx;
    int32_t right_idx;
    int32_t pivot_idx;

    int32_t pivot_offset;

} BsearchIterator_t;

/**
 * @brief  Inits the search iterator for use
 * 
 * @param[in] iter - pointer to the iterator instance
 * @param[in] startIdx - the config to set the heap
 * @param[in] startIdx - the config to set the heap
 *
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG
 * 
 * @memberof BsearchIterator
 */
ErrorCode_t bsearch_iter_init(BsearchIterator_t *iter, int32_t startIdx, int32_t stopIdx);

/**
 * @brief  Destructor for the iterator
 * 
 * @param[in] iter - pointer to the iterator instance
 * 
 * @memberof BsearchIterator
 */
void bsearch_iter_deinit(BsearchIterator_t *iter);

/**
 * @brief  Gets the next index to check for.
 * 
 * @param[in] iter - pointer to the iterator instance
 * 
 * @returns The index of the next element to check (0 indexed)
 * 
 * @memberof BsearchIterator
 */
int32_t bsearch_iter_get_check_index(BsearchIterator_t *iter);

/**
 * @brief  Gets the index where the matching element was found.
 * 
 * @note This is only valid if the array has not changed since the search was conducted.
 * 
 * @param[in] iter - pointer to the iterator instance
 * 
 * @retval -1 - Not found yet
 * @returns The index of the element that matches
 * 
 * @memberof BsearchIterator
 */
int32_t bsearch_iter_get_found_index(BsearchIterator_t *iter);

/**
 * @brief  Gets the current search status.
 * 
 * @param[in] iter - pointer to the iterator instance
 * 
 * @returns The status of the iterator
 * 
 * @memberof BsearchIterator
 */
BsearchIteratorStatus_t bsearch_iter_status(BsearchIterator_t *iter);

/**
 * @brief  Updates the search iterator.
 * 
 * Pass in an appropriate result based on the comparison outcome when inspecting the element at the
 * search iterator's requested index.
 * 
 * @param[in] iter - pointer to the iterator instance
 * @param[in] notification - The comparison outcome as one of the possible notifications.
 * 
 * @memberof BsearchIterator
 */
void bsearch_iter_notify(BsearchIterator_t *iter, BsearchNotification_t notification);
