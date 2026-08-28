#ifndef __BSP_UART_DRIVER_H__
#define __BSP_UART_DRIVER_H__

#include "usart.h"

#include "FreeRTOS.h"
// #include "task.h"
#include "queue.h"
#include "elog.h"
#include "mid_circular_buffer.h"


void bsp_uart_driver_func(void *argument);

mid_circular_buffer_t * get_circular_buffer(void);
#endif
