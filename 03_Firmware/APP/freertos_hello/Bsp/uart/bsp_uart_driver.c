#include "bsp_uart_driver.h"

#include "usart.h"

#include "FreeRTOS.h"
// #include "task.h"
#include "queue.h"
#include "elog.h"

#include "mid_circular_buffer.h"

#define BUFFER_A    0
#define BUFFER_B    1

#define IRQ_SEND_TO_THREAD  0xA1A2A3A4
#define FRONT_SEND_TO_END   0xB1B2B3B4

extern QueueHandle_t queue_irq_rec_A ;


QueueHandle_t queue_uart_irq_threaf = NULL;

#if 0
uint8_t flag_ab = BUFFER_A;
uint8_t g_data_buffer_a[1] = {0x00};
uint8_t g_data_buffer_b[1] = {0x00};
#endif

#if 1

uint8_t g_data_buffer = 0;

#endif

static mid_circular_buffer_t *g_circular_buffer_irq = NULL;

void bsp_uart_driver_func(void *argument)
{
    uint32_t received_data = 0;
    uint32_t front_to_end = FRONT_SEND_TO_END;

    //Create a circular buffer
	mid_circular_buffer_t *p_circular_buffer = create_empty_circular_buffer();
	if( NULL == p_circular_buffer)
	{
		log_i(" buffer create  is failed");
	}
	log_i(" buffer create  is success");

    g_circular_buffer_irq = p_circular_buffer;

//	flag_ab = BUFFER_A;

	//Start the reception process
	HAL_StatusTypeDef ret;
	ret = HAL_UART_Receive_IT(&huart1, &g_data_buffer, 1);
	if(HAL_OK == ret)
	{
		log_i("HAL_UART_Receive_IT(&huart1, g_data_buffer, 1)  == ok");
	}
	
    //Create email queue
	queue_uart_irq_threaf = xQueueCreate(1,4);
	if( NULL == queue_uart_irq_threaf)
	{
		log_i(" queue create is failed ");
	}
	
	
	for(;;)
	{
        //Front-end receives email notifications
        if( pdPASS == xQueueReceive(    queue_uart_irq_threaf   , 
                                        &received_data          ,
                                        portMAX_DELAY           ))
		{
		    log_i("Received data: [%x]", (unsigned long)received_data);
            
            //Front-end sends email notification to the back-end.
            if(pdPASS == xQueueSend(queue_irq_rec_A,front_to_end,0))
            {
                log_i("Front-end sends email notification to the back-end.");

            }
        }

        

		vTaskDelay(100);
	}
	
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

#if 0
  log_d("HAL_UART_RxCpltCallback");

  log_d("g_data_buffer_a = 0x%02X", g_data_buffer_a[0]);
  log_d("g_data_buffer_b = 0x%02X", g_data_buffer_b[0]);

	HAL_StatusTypeDef ret; 
  if(flag_ab == BUFFER_A)
  {
    log_d("flag_ab == BUFFER_A");
    flag_ab = BUFFER_B;
    ret = HAL_UART_Receive_IT(&huart1, g_data_buffer_b, 1);
  }
  else if(flag_ab == BUFFER_B)
  {
    log_d("flag_ab == BUFFER_B");
    flag_ab = BUFFER_A;
    ret = HAL_UART_Receive_IT(&huart1, g_data_buffer_a, 1);
  }

  if(HAL_OK != ret)
  {
    log_d("HAL_UART_RxCpltCallback switch mode failed");
  }
#endif

#if 0

    mid_circular_buffer_status_t ret = MID_CIRCULAR_BUFFER_OK;

    if(NULL == g_circular_buffer_irq)
    {
    return ;
    }
    log_d("temp_data = 0x%02X", g_data_buffer);
    ret = insert_data(g_circular_buffer_irq,g_data_buffer);
    if (  MID_CIRCULAR_BUFFER_OK == ret )
    {
        uint8_t temp_data = 0;
        if(MID_CIRCULAR_BUFFER_OK == get_data(g_circular_buffer_irq,&temp_data))
        {
            log_d("temp_data = 0x%02X", temp_data);
        }
    }

  	HAL_StatusTypeDef ret_1;
	ret_1 = HAL_UART_Receive_IT(&huart1, &g_data_buffer, 1);
	if(HAL_OK == ret_1)
	{
		log_i("HAL_UART_Receive_IT(&huart1, g_data_buffer, 1)  == ok");
	}
  
#endif

#if 1
    mid_circular_buffer_status_t ret = MID_CIRCULAR_BUFFER_OK;

    if(NULL == g_circular_buffer_irq)
    {
        return ;
    }
    ret = insert_data(g_circular_buffer_irq,g_data_buffer);
    if (  MID_CIRCULAR_BUFFER_OK != ret )
    {
        log_d("Data failed to be placed in the buffer zone.");
    }

    //The email has been sent to the front end.
    BaseType_t ret_1 = pdFALSE;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t send_to_thread = IRQ_SEND_TO_THREAD;

    ret_1 = xQueueSendFromISR(queue_uart_irq_threaf, 
                            &send_to_thread, 
                            &xHigherPriorityTaskWoken); 

    if (ret_1 != pdPASS)
    {
		log_d("mail failed to be placed in the buffer zone.");
		return ;
    }
	log_d("The email has been sent to the front end.");
	
    //Start the next reception process
  	HAL_StatusTypeDef ret_2;
	ret_2 = HAL_UART_Receive_IT(&huart1, &g_data_buffer, 1);
	if(HAL_OK != ret_2)
	{
		log_i("HAL_UART_Receive_IT(&huart1, g_data_buffer, 1)  == not");
	}

#endif

}
