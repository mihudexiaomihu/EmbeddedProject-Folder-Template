#ifndef __UART_PARSE_TASK_H__
#define __UART_PARSE_TASK_H__

#include <stdint.h>

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "elog.h"
#include "mid_circular_buffer.h"

#include "bsp_uart_driver.h"




#define FRAME_NOT_DETEXTED    0X00
#define FRAME_HEAD            0X01
#define FRAME_END             0X02

#define FRAME_HEAD_FLAG       0XFE
#define FRAME_END_FLAG        0XFF

void Serial_port_transfer_task(void *argument);
void Serial_port_payload_task(void *argument);  

#endif
