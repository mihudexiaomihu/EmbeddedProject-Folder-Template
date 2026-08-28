#include "uart_parse_task.h"



// extern UART_HandleTypeDef huart1;

uint8_t buffer1[1] = {0};
uint8_t buffer2[2] = {0};

QueueHandle_t queue_irq_rec_A = NULL;

static mid_circular_buffer_t *g_circular_buffer_from_driver = NULL;

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


    g_circular_buffer_from_driver = get_circular_buffer();
  for(;;)
  {
		if( pdPASS == xQueueReceive(queue_irq_rec_A, &received_data, portMAX_DELAY))
		{
			    log_i("Received data: %x", (unsigned long)received_data);

          if (NULL == g_circular_buffer_from_driver)
          {
            return ;
          }
          while (   MID_CIRCULAR_BUFFER_ERROR_EMPTY   
                    != 
                    buffer_is_empty(g_circular_buffer_from_driver))
          {
                uint8_t temp_data = 0;

                if  (   MID_CIRCULAR_BUFFER_OK 
                        == 
                        get_data(g_circular_buffer_from_driver,&temp_data)
                    )
                {
                    log_i("buffer get success");
                }
                log_i("buffer_read_out from APP = [%d]",temp_data);

                static status = FRAME_NOT_DETEXTED;
                switch (status)
                {
                case  FRAME_NOT_DETEXTED:
                    if ( FRAME_HEAD_FLAG == temp_data)
                    {
                        status = FRAME_HEAD;
                    }
                    

                  break;
                case  FRAME_HEAD:
                    if (FRAME_END_FLAG == temp_data)
                    {
                        status = FRAME_END;
                    }
                    
                  break;
                case  FRAME_END:

                    status = FRAME_NOT_DETEXTED;

                  break;

                default:
                  break;
                }



          }
          



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







