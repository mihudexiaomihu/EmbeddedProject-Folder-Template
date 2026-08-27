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
#include "string.h"
#include "stdlib.h"
#include "elog.h"
#include "queue.h"
#include "semphr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint32_t *buffer1 = NULL;
uint32_t *buffer2 = NULL;

SemaphoreHandle_t g_buffer_mutex = NULL;

QueueHandle_t  queue;
QueueHandle_t  convert_voltage_queue;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ADC_BUFFER_SIZE  	1
#define DMA_ADC_CPLT_INT	0xA5A5A5A5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

osThreadId_t convert_voltage_TaskHandle;
const osThreadAttr_t convert_voltage_Task_attributes = {
  .name = "convert_voltage_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* USER CODE END Variables */
/* Definitions for defaultTask */

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void app_elog_init(void);
void convert_voltage_Task(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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

  g_buffer_mutex = xSemaphoreCreateMutex();

  if(NULL == g_buffer_mutex )
  {
    log_i("Failed to create mutex");
  }
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  queue						= 		xQueueCreate(1,sizeof(uint32_t));
  convert_voltage_queue 	= 		xQueueCreate(1,sizeof(uint32_t));	
	
  if(NULL == queue || NULL == convert_voltage_queue)
  {
  	log_i(" queue create is failed");
  }
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  convert_voltage_TaskHandle = osThreadNew(convert_voltage_Task, NULL, &convert_voltage_Task_attributes);
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  app_elog_init();

  buffer1 = malloc(ADC_BUFFER_SIZE * sizeof(uint32_t));
  buffer2 = malloc(ADC_BUFFER_SIZE * sizeof(uint32_t));

  if(NULL == buffer1 || NULL == buffer2)
  {
    log_e("Failed to allocate buffer1 or buffer2");
	  return ;
  }
  
  memset((void*)buffer1,(int)0xFF,(unsigned int)ADC_BUFFER_SIZE*sizeof(uint32_t));
  memset((void*)buffer2,(int)0xFF,(unsigned int)ADC_BUFFER_SIZE*sizeof(uint32_t));
  
  // Create a queue to hold data
  HAL_StatusTypeDef ret;
  ret = HAL_ADC_Start_DMA(&hadc1,buffer1,ADC_BUFFER_SIZE);
	if(HAL_OK != ret)
	{
		log_i(" ADC failed");
	}
	
	BaseType_t result;
	uint32_t queue_data;
	uint8_t buffer_select_flag = 0;

  /* Infinite loop */
  for(;;)
  {
    result = xQueueReceive(queue, &queue_data, portMAX_DELAY);
    if (result == pdPASS)
    {
      log_i("Data received from the queue");
    } 
    
    if(queue_data == 0xA5A5A5A5)
    {
      if( 0 == buffer_select_flag)
      {
        if(pdPASS == xSemaphoreTake(g_buffer_mutex, portMAX_DELAY))//获取互斥锁
        {
              buffer_select_flag = 1;
              ret = HAL_ADC_Start_DMA(&hadc1,buffer2,ADC_BUFFER_SIZE);
              if (result == pdPASS)
              {
                result = xQueueSend(convert_voltage_queue,buffer1,0);
                if( pdPASS == result)
                {
                  log_i("The data has been sent to Task Two.");
                }
              }
          xSemaphoreGive(g_buffer_mutex); //执行完成后释放互斥锁
        }
      }
      else
      {
        if(pdPASS == xSemaphoreTake(g_buffer_mutex, portMAX_DELAY))//获取互斥锁
        {
            buffer_select_flag = 0;
            ret = HAL_ADC_Start_DMA(&hadc1,buffer1,ADC_BUFFER_SIZE);
            if (result == pdPASS)
            {
                  result = xQueueSend(convert_voltage_queue,buffer1,0);
                  if( pdPASS == result)
                  {
                    log_i("The data has been sent to Task Two.");
                  }
            }
        xSemaphoreGive(g_buffer_mutex); //执行完成后释放互斥锁
        }
	    } 
    }
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void app_elog_init(void)
{
	elog_init();
//	elog_set_text_color_enable(true);
	
	elog_set_fmt(ELOG_LVL_ASSERT	,	ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR		,	ELOG_FMT_LVL|ELOG_FMT_TAG);
	elog_set_fmt(ELOG_LVL_WARN		,	ELOG_FMT_LVL|ELOG_FMT_TAG);
	elog_set_fmt(ELOG_LVL_INFO		,	ELOG_FMT_LVL|ELOG_FMT_TAG);
	elog_set_fmt(ELOG_LVL_DEBUG		,	ELOG_FMT_ALL & ~(	ELOG_FMT_TIME	|
															ELOG_FMT_P_INFO	|
															ELOG_FMT_T_INFO	));
	elog_set_fmt(ELOG_LVL_VERBOSE	,	ELOG_FMT_ALL);
	
	elog_start();
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
   */
	uint32_t dma_pattern_cplt = DMA_ADC_CPLT_INT;
	
	BaseType_t ret_1;
	BaseType_t xHigherPriorityTaskWoken;
	
	xHigherPriorityTaskWoken 			= 		pdFALSE;
	
	ret_1 = xQueueSendFromISR( queue, &dma_pattern_cplt, &xHigherPriorityTaskWoken );
	if (ret_1 == pdPASS) 
	{
		log_i("Data sent to the queue successfully");
	} 
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ErrorCallback could be implemented in the user file
   */
	log_i("ADC Error");	
}

void convert_voltage_Task(void *argument)
{
	BaseType_t result;
	uint32_t receive_data = 0;
	float voltage = 0;

	for(;;)
	{
      result = xQueueReceive(convert_voltage_queue,&receive_data,portMAX_DELAY);
      
      if (pdPASS == result) 
      {
        log_i("task 2 receive data is [%d]",receive_data);
        if(pdPASS == xSemaphoreTake(g_buffer_mutex, portMAX_DELAY))
          {
            log_i("Task Two has acquired the mutex.");
            
            voltage = (float)receive_data / 4095.0f * 3.3f;;
            log_i("voltage is [%f]",voltage);
            
            xSemaphoreGive(g_buffer_mutex); //执行完成后释放互斥锁
          }
      }
    osDelay(100);
  }
}

/* USER CODE END Application */

