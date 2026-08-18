/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_key.h"
#include "bsp_led.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for led_task */
osThreadId_t led_taskHandle;
const osThreadAttr_t led_task_attributes = {
  .name = "led_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for key_task */
osThreadId_t key_taskHandle;
const osThreadAttr_t key_task_attributes = {
  .name = "key_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for key_queue */
//osMessageQueueId_t key_queueHandle;
//const osMessageQueueAttr_t key_queue_attributes = {
//  .name = "key_queue"
//};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void led_default_task(void *argument);
void key_default_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of key_queue */
//  key_queueHandle = osMessageQueueNew (16, sizeof(uint16_t), &key_queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of led_task */
  led_taskHandle = osThreadNew(led_default_task, NULL, &led_task_attributes);

  /* creation of key_task */
  key_taskHandle = osThreadNew(key_default_task, NULL, &key_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
   g_key_queueHandle = xQueueCreate(10U,sizeof(key_function_t));
  /* USER CODE END RTOS_EVENTS */

}

/**
 * @brief Default LED task that handles key events and controls LED behaviour.
 * @param argument Task entry parameter (not used in this task; pass NULL).
 * @return This task never returns (infinite loop).
 *
 * @details
 * This function runs as an independent FreeRTOS task. Its main operations:
 * 1. Delays 500 ms at start to allow other system modules to initialise.
 * 2. Repeatedly receives key messages from the queue `g_key_queueHandle`.
 *    - On `KEY_SHORT_PRESSED`: toggles the LED state (calls `led_toggle()`)
 *      only if blink mode is not active.
 *    - On `KEY_LONG_PRESSED`: enables blink mode (`blink_enable = 1`).
 * 3. While blink mode is active, it calls `led_toggleblink(3, 40)` to flash
 *    the LED 3 times with 40 ms intervals. When the blink completes
 *    (`LED_OK`), the blink mode is disabled (`blink_enable = 0`).
 * 4. At the end of each loop, delays 10 ms to reduce CPU usage.
 *
 * @note
 * The function contains several test blocks disabled by `#if 0`. They simulate
 * key interrupt events (rising/falling edges) and send test data to
 * `s_key_irq_queue`. These are kept for debugging reference and do not affect
 * normal operation.
 *
 * @warning
 * The global queue `g_key_queueHandle` must be created before this task runs.
 * Otherwise, the task will check every 10 ms until the queue becomes valid.
 */

 void led_default_task(void *argument)
{

	osDelay(500);
#if 0	//test 01
	if(NULL == s_key_irq_queue)
	{
		printf("s_key_irq_queue create is fail\r\n");
	}
	key_press_irq_status_t  key_irq;
	key_irq.trigger_type = RISING_EDGE;
	key_irq.tick = HAL_GetTick();
	printf("RISING_EDGE time is [%d]",key_irq.tick);
	xQueueSend(s_key_irq_queue,&key_irq,0);
#endif
	
#if 0	//test 02	
	if(NULL == s_key_irq_queue)
	{
		printf("s_key_irq_queue create is fail\r\n");
	}
	key_press_irq_status_t  key_irq;
	key_irq.trigger_type = FALLING_EDGE;
	key_irq.tick = HAL_GetTick();
	printf("FALLING_EDGE time is [%d]",key_irq.tick);
	xQueueSend(s_key_irq_queue,&key_irq,0);
	
	osDelay(15);
	
	key_irq.trigger_type = RISING_EDGE;
	key_irq.tick = HAL_GetTick();
	printf("RISING_EDGE time is [%d]",key_irq.tick);
	xQueueSend(s_key_irq_queue,&key_irq,0);
	
#endif	

#if 0	//test 03	
	if(NULL == s_key_irq_queue)
	{
		printf("s_key_irq_queue create is fail\r\n");
	}
	key_press_irq_status_t  key_irq;
	key_irq.trigger_type = FALLING_EDGE;
	key_irq.tick = HAL_GetTick();
	printf("FALLING_EDGE time is [%d]",key_irq.tick);
	xQueueSend(s_key_irq_queue,&key_irq,0);
	
	osDelay(550);
	
	key_irq.trigger_type = RISING_EDGE;
	key_irq.tick = HAL_GetTick();
	printf("RISING_EDGE time is [%d]",key_irq.tick);
	xQueueSend(s_key_irq_queue,&key_irq,0);
	
#endif

    key_function_t key_value;
    led_status_t ret;
    uint8_t blink_enable = 0U;

    (void)argument;

    for (;;)
    {
        if (g_key_queueHandle == NULL)
        {
            osDelay(10U);
            continue;
        }

        if (xQueueReceive(
                g_key_queueHandle,
                &key_value,
                0U) == pdTRUE)
        {
            if (key_value == KEY_SHORT_PRESSED)
            {
                printf("data is key short\r\n");

                if (blink_enable == 0U)
                {
                    led_toggle();
                }
            }
            else if (key_value == KEY_LONG_PRESSED)
            {
                printf("data is key long\r\n");
                blink_enable = 1U;
            }
        }

        if (blink_enable == 1U)
        {
            ret = led_toggleblink(3U, 40U);

            if (ret == LED_OK)
            {
                blink_enable = 0U;
            }
        }

        osDelay(10U);
    }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

