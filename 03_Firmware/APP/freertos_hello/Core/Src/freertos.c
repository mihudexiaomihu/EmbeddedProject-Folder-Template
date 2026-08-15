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
#include "queue.h"
#include "bsp_key.h"
#include "bsp_led.h"
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
/* Definitions for led_task_func */
osThreadId_t led_task_funcHandle;
const osThreadAttr_t led_task_func_attributes = {
  .name = "led_task_func",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ked_task_func */
osThreadId_t ked_task_funcHandle;
const osThreadAttr_t ked_task_func_attributes = {
  .name = "ked_task_func",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for key_value */
osMessageQueueId_t key_valueHandle;
const osMessageQueueAttr_t key_value_attributes = {
  .name = "key_value"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void led_task(void *argument);
void key_task(void *argument);

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
  /* creation of key_value */
  key_valueHandle = osMessageQueueNew (   1                     , 
                                          sizeof(key_function_t),
                                          &key_value_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of led_task_func */
  led_task_funcHandle = osThreadNew(  led_task                  , 
                                      NULL                      , 
                                      &led_task_func_attributes);

  /* creation of ked_task_func */
  ked_task_funcHandle = osThreadNew(  key_task                  , 
                                      NULL                      , 
                                      &ked_task_func_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_led_task */
/**
 * @brief Process key events and control the LED.
 *
 * A short press toggles the LED. A long press starts three blinks.
 *
 * @param[in] argument Unused task argument.
 * @return This task never returns.
 */
/* USER CODE END Header_led_task */
void led_task(void *argument)
{
  /* USER CODE BEGIN led_task */
    key_function_t key_value = KEY_NOT_PRESSED;
    led_status_t ret = LED_OK;
    static uint8_t blink_enable = 0;

    (void)argument;

  /* Infinite loop */
  for (;;)
  {
    /* Read and process one queued key event ------------------------------ */
    if (pdTRUE == xQueueReceive(key_valueHandle, &key_value, 0))
    {
        if (KEY_SHORT_PRESSED == key_value)
        {
            printf("data is key short \r\n");

            if (0U == blink_enable)
            {
                led_toggle();
            }
        }
        else if (KEY_LONG_PRESSED == key_value)
        {
            blink_enable = 1U;
            printf("data is key long \r\n");
        }
    }

    /* Continue the active non-blocking blink sequence -------------------- */
    if (1U == blink_enable)
    {
        ret = led_toggleblink(3U, 40U);

        if (LED_OK == ret)
        {
            blink_enable = 0U;
        }
    }

    osDelay(10);
  }
  /* USER CODE END led_task */
}

/* USER CODE BEGIN Header_key_task */
/**
* @brief Function implementing the ked_task_func thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_key_task */
void key_task(void *argument)
{
  /* USER CODE BEGIN key_task */
	key_function_t key_value = KEY_NOT_PRESSED;

  /* Infinite loop */
	for(;;)
	{
		key_scan_short_long_press(&key_value,5,200);
		if( KEY_NOT_PRESSED != key_value )
		{
			printf ("have a key value \r\n");
			xQueueSend(key_valueHandle,&key_value,0);
		}
		
		osDelay(10);
	}
  /* USER CODE END key_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

