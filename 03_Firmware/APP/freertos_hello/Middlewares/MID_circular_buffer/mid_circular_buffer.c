#include "mid_circular_buffer.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#include "elog.h"

/**
* @brief  Create a circular buffer
* @param  not
* @retval NULL : Failed to create a circular buffer
*          p_buffer_temp: Pointer to the buffer area
* @note not
*/
mid_circular_buffer_t* create_empty_circular_buffer(void)
{
    mid_circular_buffer_t *p_buffer_temp = NULL;

    /* Allocate a block of memory on the heap for the buffer control block. */
    p_buffer_temp = (mid_circular_buffer_t *)malloc(sizeof(mid_circular_buffer_t));

    if(NULL == p_buffer_temp)
    {
        /* Out of memory: report failure to the caller. */
        return NULL;
    }

    /* Zero-initialize the whole structure so head/tail start at a known state. */
    memset(p_buffer_temp ,0,sizeof(mid_circular_buffer_t));

    return p_buffer_temp;
}

/**
 * @brief  Check whether the circular buffer holds no data.
 * @param  p_buffer  Pointer to the circular buffer instance.
 * @retval MID_CIRCULAR_BUFFER_ERROR_PARAMETER  p_buffer is NULL.
 *         MID_CIRCULAR_BUFFER_ERROR_EMPTY      The buffer is empty.
 *         MID_CIRCULAR_BUFFER_OK               The buffer contains at least one element.
 * @note   An empty buffer is defined by head == tail.
 */
mid_circular_buffer_status_t buffer_is_empty(mid_circular_buffer_t * p_buffer)
{
    if( NULL == p_buffer)
    {
        return MID_CIRCULAR_BUFFER_ERROR_PARAMETER;
    }

    /* When head and tail meet, there is nothing left to read. */
    if(p_buffer->head == p_buffer->tail)
    {
        return MID_CIRCULAR_BUFFER_ERROR_EMPTY;
    }
    else
    {
        return MID_CIRCULAR_BUFFER_OK;
    }

}


/**
 * @brief  Check whether the circular buffer has no free slot left.
 * @param  p_buffer  Pointer to the circular buffer instance.
 * @retval MID_CIRCULAR_BUFFER_ERROR_PARAMETER  p_buffer is NULL.
 *         MID_CIRCULAR_BUFFER_ERROR_FULL       The buffer is full.
 *         MID_CIRCULAR_BUFFER_OK               There is still room to write.
 * @note   One slot is deliberately kept unused, so "full" is detected
 *         as (head + 1) % SIZE == tail rather than head == tail.
 */
mid_circular_buffer_status_t    buffer_is_full  (
                                            mid_circular_buffer_t   *p_buffer)
{
    if( NULL == p_buffer)
    {
        return MID_CIRCULAR_BUFFER_ERROR_PARAMETER;
    }

    /* If the next write position would collide with the tail, the buffer is full. */
    if (
        ((p_buffer->head)%CIRCULAR_BUFFER_SIZE + 1)
                ==
        ((p_buffer->tail)%CIRCULAR_BUFFER_SIZE)
        )
    {
        return MID_CIRCULAR_BUFFER_ERROR_FULL;
    }
    else
    {
        return MID_CIRCULAR_BUFFER_OK;
    }
}


/**
 * @brief  Append one data element to the head of the circular buffer.
 * @param  p_buffer  Pointer to the circular buffer instance.
 * @param  data      Value to write into the buffer.
 * @retval MID_CIRCULAR_BUFFER_ERROR_PARAMETER  p_buffer is NULL.
 *         MID_CIRCULAR_BUFFER_ERROR_WRITE      The buffer is full, nothing was written.
 *         MID_CIRCULAR_BUFFER_OK               The element was written successfully.
 * @note   The head index wraps around via modulo so the buffer reuses its storage.
 */
mid_circular_buffer_status_t    insert_data     (
                                            mid_circular_buffer_t   *p_buffer,\
                                                      data_type_t    data    )
{

    if( NULL == p_buffer)
    {
        return MID_CIRCULAR_BUFFER_ERROR_PARAMETER;
    }

    /* Refuse to write when there is no free slot. */
    if( MID_CIRCULAR_BUFFER_ERROR_FULL == buffer_is_full(p_buffer) )
    {
        return MID_CIRCULAR_BUFFER_ERROR_WRITE;
    }

    /* Store the value at the current head position, then advance head. */
    p_buffer->data[ (p_buffer->head) % CIRCULAR_BUFFER_SIZE ] = data;

    p_buffer->head ++;

    return MID_CIRCULAR_BUFFER_OK;
}


/**
 * @brief  Remove one data element from the tail of the circular buffer.
 * @param  p_buffer  Pointer to the circular buffer instance.
 * @param  data      Output pointer where the read value is stored.
 * @retval MID_CIRCULAR_BUFFER_ERROR_PARAMETER  p_buffer is NULL.
 *         MID_CIRCULAR_BUFFER_ERROR_READ       The buffer is empty, nothing was read.
 *         MID_CIRCULAR_BUFFER_OK               An element was read successfully.
 * @note   The tail index wraps around via modulo so the buffer reuses its storage.
 */
mid_circular_buffer_status_t    get_data        (
                                            mid_circular_buffer_t   *p_buffer,\
                                                      data_type_t   *data    )
{
    if( NULL == p_buffer)
    {
        return MID_CIRCULAR_BUFFER_ERROR_PARAMETER;
    }

    /* Refuse to read when there is no data to consume. */
    if( MID_CIRCULAR_BUFFER_ERROR_EMPTY == buffer_is_empty(p_buffer) )
    {
        return MID_CIRCULAR_BUFFER_ERROR_READ;
    }

    /* Fetch the value at the current tail position, then advance tail. */
    *data = p_buffer->data[(p_buffer->tail) % CIRCULAR_BUFFER_SIZE];
    p_buffer->tail ++;

    return MID_CIRCULAR_BUFFER_OK;
}
