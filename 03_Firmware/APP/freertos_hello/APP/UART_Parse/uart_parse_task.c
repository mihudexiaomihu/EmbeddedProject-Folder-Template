#include "uart_parse_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "elog.h"
#include "queue.h"

extern UART_HandleTypeDef huart1;

uint8_t buffer1[1] = {0};
uint8_t buffer2[2] = {0};

QueueHandle_t queue_irq_rec_A = NULL;
void Serial_port_transfer_task(void *argument)
{
  /* USER CODE BEGIN Serial_port_transfer_task */
  /* Infinite loop */
    uint32_t received_data = 0;

    log_i("Serial_port_transfer_task is running");
    queue_irq_rec_A = NULL;
    queue_irq_rec_A = xQueueCreate(1, sizeof(uint32_t));


	
    if(NULL == queue_irq_rec_A)
    {
        log_e("Failed to create queue_irq_rec_A");
    }


  for(;;)
  {
		if( pdPASS == xQueueReceive(queue_irq_rec_A, &received_data, portMAX_DELAY))
		{
			    log_i("Received data: %x", (unsigned long)received_data);
		}


    osDelay(10);
  }
  /* USER CODE END Serial_port_transfer_task */
}

void Serial_port_payload_task(void *argument)
{

    /* USER CODE BEGIN Serial_port_payload_task */
	log_i("Serial_port_payload_task is running");
    /* Infinite loop */
    for(;;)
    {

        osDelay(10);
    }
    /* USER CODE END Serial_port_payload_task */
}







