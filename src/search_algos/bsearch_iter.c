#include <cemb/bsearch_iter.h>

#include <assert.h>

static int32_t bsearch_get_next_pivot_offset(int32_t current_pivot_offset)
{
    if (current_pivot_offset == 0)
    {
        return 1;
    }
    else if (current_pivot_offset > 0)
    {
        return current_pivot_offset * -1;
    }
    else
    {
        return (current_pivot_offset * -1) + 1;
    }
}

static int32_t bsearch_get_midpoint_safe(int32_t left_idx, int32_t right_idx)
{
    return left_idx + ((right_idx - left_idx)/2);
}

int bsearch_iter_init(BsearchIterator_t *iter, int32_t start_idx, int32_t stop_idx)
{
    assert(iter);

    if (start_idx >= stop_idx) return 1;

    iter->status = BSEARCH_CONTINUE;

    iter->left_idx = start_idx;
    iter->right_idx = stop_idx;
    iter->pivot_idx = bsearch_get_midpoint_safe(iter->left_idx, iter->right_idx);

    iter->pivot_offset=0;

    return 0;
}

void bsearch_iter_deinit(BsearchIterator_t *iter)
{
    assert(iter);

    if (iter->status == BSEARCH_CONTINUE)
    {
        iter->status = BSEARCH_DONE_NOT_FOUND;
    }
}

int32_t bsearch_iter_get_check_index(BsearchIterator_t *iter)
{
    assert(iter);
    
    return iter->pivot_idx + iter->pivot_offset;
}

int32_t bsearch_iter_get_found_index(BsearchIterator_t *iter)
{
    assert(iter);

    if (iter->status == BSEARCH_DONE_FOUND)
    {
        return iter->pivot_idx + iter->pivot_offset;
    }
    else
    {
        return -1;
    }
}

BsearchIteratorStatus_t bsearch_iter_status(BsearchIterator_t *iter)
{
    assert(iter);

    return iter->status;
}

void bsearch_iter_notify(BsearchIterator_t *iter, BsearchNotification_t notification)
{
    assert(iter);

    // do nothing
    if (iter->status != BSEARCH_CONTINUE)
    {
        return;
    }

    switch (notification)
    {
        case BSEARCH_NOTIFY_EQ:
            iter->status = BSEARCH_DONE_FOUND;
            break;

        case BSEARCH_NOTIFY_LEFT:
            {
                int32_t next_right_idx;
                if (iter->pivot_offset > 0)
                {
                    int32_t next_pivot_offset = bsearch_get_next_pivot_offset(iter->pivot_offset);
                    next_right_idx = iter->pivot_idx + next_pivot_offset;
                }
                else
                {
                    next_right_idx = iter->pivot_idx + iter->pivot_offset - 1;
                }

                if ((next_right_idx >= iter->right_idx) || (next_right_idx < iter->left_idx))
                {
                    iter->status = BSEARCH_DONE_NOT_FOUND;
                }
                else
                {
                    iter->right_idx = next_right_idx;
                    iter->pivot_idx = bsearch_get_midpoint_safe(iter->left_idx, iter->right_idx);
                    iter->pivot_offset = 0;
                }
            }
            break;

        case BSEARCH_NOTIFY_RIGHT:
            {
                int32_t next_left_idx;
                if (iter->pivot_offset < 0)
                {
                    int32_t next_pivot_offset = bsearch_get_next_pivot_offset(iter->pivot_offset);
                    next_left_idx = iter->pivot_idx + next_pivot_offset;
                }
                else
                {
                    next_left_idx = iter->pivot_idx + iter->pivot_offset + 1;
                }

                if ((next_left_idx <= iter->left_idx) || (next_left_idx > iter->right_idx))
                {
                    iter->status = BSEARCH_DONE_NOT_FOUND;
                }
                else
                {
                    iter->left_idx = next_left_idx;
                    iter->pivot_idx = bsearch_get_midpoint_safe(iter->left_idx, iter->right_idx);
                    iter->pivot_offset = 0;
                }
            }
            break;

        case BSEARCH_NOTIFY_ERR_SKIP:
            {
                iter->pivot_offset = bsearch_get_next_pivot_offset(iter->pivot_offset);
                int32_t next = iter->pivot_idx + iter->pivot_offset;
                if (next > iter->right_idx || next < iter->left_idx)
                {
                    iter->status = BSEARCH_DONE_NOT_FOUND;
                }
            }
            break;

        case BSEARCH_NOTIFY_ERR_ABORT: /* Intentional Fallthrough */
        default:
            iter->status = BSEARCH_DONE_NOT_FOUND;
            break;
    }
}