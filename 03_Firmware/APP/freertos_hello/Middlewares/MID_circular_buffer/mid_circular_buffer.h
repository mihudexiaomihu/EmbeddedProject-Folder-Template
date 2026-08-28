#ifndef __MID_CIRCULAR_BUFFER_H__
#define __MID_CIRCULAR_BUFFER_H__

#include <stdint.h>

#define CIRCULAR_BUFFER_SIZE        100

typedef uint8_t data_type_t;

typedef struct 
{
    data_type_t data[CIRCULAR_BUFFER_SIZE]  ;
    uint32_t    head;
    uint32_t    tail;

}mid_circular_buffer_t;

/**
 * @brief 环形缓冲区操作状态。
 */
typedef enum
{
    MID_CIRCULAR_BUFFER_OK              = 0,    //  buffer is ok
    MID_CIRCULAR_BUFFER_ERROR_PARAMETER = 1,    //  buffer isnot create success
    MID_CIRCULAR_BUFFER_ERROR_EMPTY     = 2,    //  buffer is empty
    MID_CIRCULAR_BUFFER_ERROR_FULL      = 3,    //  buffer is full
    MID_CIRCULAR_BUFFER_ERROR_WRITE     = 4,    //  write failed
    MID_CIRCULAR_BUFFER_ERROR_READ      = 5     //  read failed
} mid_circular_buffer_status_t;

mid_circular_buffer_t*          create_empty_circular_buffer(void);
mid_circular_buffer_status_t    buffer_is_empty (   
                                            mid_circular_buffer_t   *p_buffer);

mid_circular_buffer_status_t    buffer_is_full  (   
                                            mid_circular_buffer_t   *p_buffer);

mid_circular_buffer_status_t    insert_data     (   
                                            mid_circular_buffer_t   *p_buffer,\
                                                      data_type_t    data    );

mid_circular_buffer_status_t    get_data        (   
                                            mid_circular_buffer_t   *p_buffer,\
                                                      data_type_t   *data    );
#endif
