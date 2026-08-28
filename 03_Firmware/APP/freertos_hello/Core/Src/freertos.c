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
#include "bsp_elog.h"
#include "uart_parse_task.h"
#include <stdio.h>	
#include "bsp_uart_driver.h"



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
/* Definitions for thread_a_Task */
osThreadId_t thread_a_TaskHandle;
const osThreadAttr_t thread_a_Task_attributes = {
  .name = "thread_a_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for thread_b_task */
osThreadId_t thread_b_taskHandle;
const osThreadAttr_t thread_b_task_attributes = {
  .name = "thread_b_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* Definitions for thread_b_task */

osThreadId_t bsp_uart_driver_taskHandle;
const osThreadAttr_t task_bsp_uart_driver_attributes = {
  .name = "task_bsp_uart_driver",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* USER CODE END FunctionPrototypes */

void Serial_port_transfer_task(void *argument);
void Serial_port_payload_task(void *argument);
void bsp_uart_driver_func(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
bsp_elog_init();
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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of thread_a_Task */
  thread_a_TaskHandle       =     osThreadNew(Serial_port_transfer_task,
                                    NULL, 
                                    &thread_a_Task_attributes);

  /* creation of thread_b_task */
  thread_b_taskHandle       =     osThreadNew(Serial_port_payload_task, 
                                    NULL, 
                                    &thread_b_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* creation of bsp_uart_driver_taskHandle */
  bsp_uart_driver_taskHandle =    osThreadNew(bsp_uart_driver_func, 
                                           NULL, 
                                           &task_bsp_uart_driver_attributes);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Serial_port_transfer_task */
/**
  * @brief  Function implementing the thread_a_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Serial_port_transfer_task */

// void Serial_port_transfer_task(void *argument)
// {
//   /* USER CODE BEGIN Serial_port_transfer_task */
//   /* Infinite loop */
//   for(;;)
//   {
//     osDelay(1);
//   }
//   /* USER CODE END Serial_port_transfer_task */
// }

/* USER CODE BEGIN Header_Serial_port_payload_task */
/**
* @brief Function implementing the thread_b_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Serial_port_payload_task */

// void Serial_port_payload_task(void *argument)
// {
//   /* USER CODE BEGIN Serial_port_payload_task */
//   /* Infinite loop */
//   for(;;)
//   {
//     osDelay(1);
//   }
//   /* USER CODE END Serial_port_payload_task */
// }

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

