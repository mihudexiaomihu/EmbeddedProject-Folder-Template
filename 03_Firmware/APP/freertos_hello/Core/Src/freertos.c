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
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define LED_TIME_TICK 100U
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
/* Definitions for ledTimer */
osTimerId_t ledTimerHandle;
const osTimerAttr_t ledTimer_attributes = {
  .name = "ledTimer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static uint8_t time_led_blink_number = 0;
static uint8_t time_led_run_number = 0;

/* USER CODE END FunctionPrototypes */

void led_default_task(void *argument);
void key_default_task(void *argument);
void ledCallback(void *argument);

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

  /* Create the timer(s) */
  /* creation of ledTimer */
  ledTimerHandle = osTimerNew(ledCallback, osTimerOnce, NULL, &ledTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

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

/* USER CODE BEGIN Header_led_default_task */
/**
  * @brief  Function implementing the led_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_led_default_task */
void led_default_task(void *argument)
{
  /* USER CODE BEGIN led_default_task */
	key_function_t msg;
	
  /* Infinite loop */
	for(;;)
	{
		if(pdTRUE == xQueueReceive(g_key_queueHandle,&msg,0))
		{
		if( KEY_SHORT_PRESSED == msg)
		{
			time_led_blink_number += 1;
			osTimerStart(ledTimerHandle,LED_TIME_TICK);
		}
		if(KEY_LONG_PRESSED == msg)
		{
			time_led_blink_number += 10;
			osTimerStart(ledTimerHandle,LED_TIME_TICK);
		}
		printf("led need blink number is [%d] \r\n",time_led_blink_number);
		}
	  
	osDelay(100);
	}
  /* USER CODE END led_default_task */
}

/* USER CODE BEGIN Header_key_default_task */
/**
* @brief Function implementing the key_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_key_default_task */
void key_default_task(void *argument)
{
  /* USER CODE BEGIN key_default_task */
  /* Infinite loop */
  key_task_func( argument);
  /* USER CODE END key_default_task */
}

/* ledCallback function */
void ledCallback(void *argument)
{
  /* USER CODE BEGIN ledCallback */
	printf("ledCallback\r\n");
	HAL_GPIO_TogglePin(LED_GPIO_GPIO_Port,LED_GPIO_Pin);
	time_led_run_number ++;
	if(time_led_run_number < time_led_blink_number*2)
	{
		osTimerStart(ledTimerHandle,LED_TIME_TICK);
		printf("Toggle number is [%d]\r\n",time_led_run_number);
	}
  /* USER CODE END ledCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

