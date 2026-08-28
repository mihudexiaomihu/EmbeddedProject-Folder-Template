#ifndef __UART_PARSE_TASK_H__
#define __UART_PARSE_TASK_H__

#include <stdint.h>



void Serial_port_transfer_task(void *argument);
void Serial_port_payload_task(void *argument);  

#endif
