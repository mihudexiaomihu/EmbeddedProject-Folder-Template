/**
 ******************************************************************************
 * Copyright (C) 2026 EternalChip, Inc. or its affiliates.
 * All Rights Reserved.
 *
 * @file bsp_key.c
 * @brief Implement board-level key scanning and press detection.
 *
 * @par Dependencies
 * - bsp_key.h
 * - STM32 HAL GPIO
 *
 * @par Processing flow
 * Call key_scan_short_long_press() periodically from one task.
 *
 * @author Wzh | R&D Dept. | EternalChip
 * @version V1.0 2026-08-15
 *
 * @note One tab equals four spaces.
 ******************************************************************************
 */

#include "bsp_key.h"

#define JITTER_TIME		5
#define SHORT_TIME		500
#define KEY_CALLBACK	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)

QueueHandle_t g_key_queueHandle;
static QueueHandle_t s_key_irq_queue;
/**
 * @brief Default key processing task that interprets edge events and generates key events.
 * @param argument Task entry parameter (not used in this task; pass NULL).
 * @return This task never returns (infinite loop).
 *
 * @details
 * This FreeRTOS task is responsible for:
 * 1. Creating two queues:
 *    - `s_key_irq_queue`: receives raw edge events (FALLING/RISING) from the ISR callback.
 *    - `g_key_queueHandle`: sends interpreted key events (short/long press) to the LED task.
 * 2. Waiting indefinitely (`portMAX_DELAY`) for a raw edge event from the interrupt queue.
 * 3. Processing a pair of FALLING + RISING edges to measure the press duration:
 *    - On the first FALLING edge, it stores the timestamp and sets a flag.
 *    - On the next RISING edge, it calculates the difference.
 * 4. Classifying the duration:
 *    - If < `JITTER_TIME` → ignored as jitter.
 *    - If between `JITTER_TIME` and `SHORT_TIME` → sends `KEY_SHORT_PRESSED`.
 *    - If >= `SHORT_TIME` → sends `KEY_LONG_PRESSED`.
 * 5. Unexpected edge sequences (e.g., two FALLINGs in a row) are logged as errors.
 *
 * @note
 * - The task uses `portMAX_DELAY` for queue receive, so it blocks until an event arrives.
 * - Both queues are created with a depth of 10 items; if creation fails, the task deletes itself.
 * - The `printf` statements are useful for debugging but may be removed in production.
 * - `JITTER_TIME` and `SHORT_TIME` are assumed to be defined elsewhere (e.g., as macro constants).
 *
 * @warning
 * - The raw interrupt queue (`s_key_irq_queue`) must be accessed by the ISR callback
 *   (e.g., `KEY_CALLBACK`) which sends edge events using `xQueueSendFromISR()`.
 * - The interpreted event queue (`g_key_queueHandle`) is consumed by the LED task
 *   (`led_default_task`).
 * - This task does not handle queue full conditions robustly – it only prints an error
 *   if sending to `g_key_queueHandle` fails.
 */
void key_default_task(void *argument)
{
    key_function_t key_value;
    key_press_irq_status_t key_irq;

    uint8_t first_trigger_mode = 0U;
    uint32_t first_trigger_tick = 0U;
    uint32_t press_time;

    (void)argument;

    s_key_irq_queue = xQueueCreate(10U,sizeof(key_press_irq_status_t));

    g_key_queueHandle = xQueueCreate(10U,sizeof(key_function_t));

    if ((s_key_irq_queue    ==  NULL) ||
        (g_key_queueHandle  ==  NULL))
    {
        printf("Queue create failed\r\n");
        vTaskDelete(NULL);
        return;
    }

    for (;;)
    {

        if (pdTRUE != xQueueReceive(    s_key_irq_queue,
                                        &key_irq,
                                        portMAX_DELAY))
        {
            continue;
        }

        if ((FALLING_EDG    ==      key_irq.trigger_type) &&
            (0              ==      first_trigger_mode))
        {
            first_trigger_tick  =   key_irq.tick;
            first_trigger_mode  =   1U;

            printf( "Falling edge trigger time[%lu]\r\n",
                            (unsigned long)key_irq.tick);
        }
        else if ((key_irq.trigger_type  ==  RISING_EDGE) &&
                 (first_trigger_mode    ==  1U))
        {
            first_trigger_mode = 0U;
            press_time = key_irq.tick - first_trigger_tick;

            printf( "Rising edge trigger time[%lu]\r\n",
                                        (unsigned long)key_irq.tick);

            if (press_time < JITTER_TIME)
            {

                printf("It is key jitter[%lu ms]\r\n",
                                        (unsigned long)press_time);
            }
            else if (press_time < SHORT_TIME)
            {
                key_value = KEY_SHORT_PRESSED;

                printf("It is key short[%lu ms]\r\n",
                                    (unsigned long)press_time
                );

                if ( pdTRUE != xQueueSend(  g_key_queueHandle,
                                            &key_value,
                                            0U))
                {
                    printf("Key event queue is full\r\n");
                }
            }
            else
            {

                key_value = KEY_LONG_PRESSED;

                printf("It is key long[%lu ms]\r\n",
                                    (unsigned long)press_time
                );

                if (pdTRUE != xQueueSend( g_key_queueHandle,
                                            &key_value,
                                            0U))
                {
                    printf("Key event queue is full\r\n");
                }
            }
        }
        else
        {
            printf(
                "Unexpected edge: type=%d, state=%u\r\n",
                key_irq.trigger_type,
                first_trigger_mode
            );
        }
    }
}

/**
 * @brief GPIO interrupt callback for key detection (alternating edge trigger).
 * @param None (this is an ISR-style callback, no parameters).
 * @return None.
 *
 * @details
 * This function is called on every GPIO interrupt (both falling and rising edges)
 * of the key pin. It implements a simple edge-alternating scheme:
 * - On the first interrupt (falling edge), it records the timestamp, sends a
 *   `FALLING_EDGE` event to the queue (`s_key_irq_queue`), then reconfigures
 *   the GPIO interrupt to trigger on the *rising* edge.
 * - On the next interrupt (rising edge), it does the same with `RISING_EDGE`,
 *   then reconfigures back to falling edge.
 *
 * This way, each complete key press (down + up) generates two events in the queue,
 * allowing the task to measure the duration between them (for short/long press).
 *
 * @note
 * - The function is designed to be called from an ISR context, hence it uses
 *   `xQueueSendFromISR()` instead of `xQueueSend()`.
 * - The static variable `trigger_type` retains its state across calls to toggle
 *   the expected edge.
 * - The queue `s_key_irq_queue` must be created before the first interrupt occurs.
 * - GPIO reconfiguration is done using HAL functions; the pin is pulled up to
 *   avoid floating states.
 *
 * @warning
 * - The queue send is non-blocking (`pdFALSE` for the pxHigherPriorityTaskWoken
 *   parameter). If the queue is full, the event is dropped (no check is performed
 *   in this code for full condition, only for NULL queue).
 * - The printf statements inside ISR are generally not recommended for production
 *   due to performance and stack usage; they are kept here for debugging.
 */

KEY_CALLBACK
{
	static trigger_type_t trigger_type = FALLING_EDGE;

	if(FALLING_EDGE == trigger_type)
	{
		trigger_type = RISING_EDGE;

		key_press_irq_status_t  key_irq={
                                        .trigger_type	=	FALLING_EDGE,
                                        .tick			=	HAL_GetTick()	
		};

		if(NULL == s_key_irq_queue)
		{
			printf("Queue is not created\r\n");
		}

		if(pdTRUE == xQueueSendFromISR(	s_key_irq_queue	,
										&key_irq,NULL	))
		{
			printf("Key interrupt send FALLING_EDGE success[%d]\r\n", 
                                                                key_irq.tick);
		}

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = KEY_GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;

		HAL_GPIO_Init(KEY_GPIO_GPIO_Port, &GPIO_InitStruct);
	}
	else if(RISING_EDGE == trigger_type)
	{
		trigger_type = FALLING_EDGE;

		key_press_irq_status_t  key_irq={
                                        .trigger_type	=	RISING_EDGE,
                                        .tick			=	HAL_GetTick()	
		};

		if(NULL == s_key_irq_queue)
		{
			printf("Queue is not created\r\n");
		}

		if(pdTRUE == xQueueSendFromISR(	s_key_irq_queue	,
										&key_irq,NULL	))
		{
			printf("Key interrupt send RISING_EDGE success[%d]\r\n",
                                                                 key_irq.tick);
		}

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = KEY_GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;

		HAL_GPIO_Init(KEY_GPIO_GPIO_Port, &GPIO_InitStruct);
	}
}













