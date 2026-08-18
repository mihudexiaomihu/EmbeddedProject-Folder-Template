 /****************************************************************************​
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.​
 * ​
 * All Rights Reserved.​
 * ​
 * @file key.h​
 * ​
 * @par dependencies ​
 * - stdio.h​
 * - stdint.h​
 * - stm32f4xx_hal.h​
 * - gpio.h​ 
 * ​
 * @author Wzh | R&D Dept. | EternalChip 立芯嵌入式​
 * ​
 * @brief Provide the HAL APIs of ley and corresponding opetions.​
 * ​
* Processing flow:​
 * ​
 * call directly.​
 * ​
 * @version V1.0 2026-08-15
 *​
 * @note 1 tab == 4 spaces!​
* ​
 *****************************************************************************/


#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/***********************************Include***********************************/
#include "stm32f4xx_hal.h"
#include "freertos.h"
#include <stdio.h>
#include <stdint.h>
#include "gpio.h"
#include "queue.h"
#include "task.h"
/***********************************Include***********************************/

/***********************************Defines***********************************/
typedef enum
{	
	 KEY_OK                  	=	0,    /* Success 						*/ 	
	 KEY_ERROR               	=	1,    /* General error 					*/ 	 
	 KEY_ERROR_TIMEOUT       	=	2,    /* Timeout						*/ 	 
	 KEY_ERROR_RESOURCES     	=	3,    /* Resource shortage 				*/	 
	 KEY_ERROR_PARAMETER     	=	4,    /* Invalid parameter 				*/
	 KEY_ERROR_NOMEMORY      	=	5,    /* Out of memory 					*/
	 KEY_ERROR_ISR           	=	6,    /* ISR error 						*/ 
	 KEY_ERROR_BUSY          	=	7,    /* Device busy 					*/
	 KEY_ERROR_RESERVED      	=	0xFF, /* Reserved 						*/
}key_status_t;


typedef enum
{
    KEY_STATE_IDLE              = 	0,    /* Idle 							*/
    KEY_STATE_DEBOUNCE          = 	1,    /* Debounce 						*/
    KEY_STATE_PRESSED           = 	2,    /* Pressed						*/
    KEY_STATE_LONG_PRESS        = 	3,    /* Long press 					*/
    KEY_STATE_RELEASE_DEBOUNCE  = 	4     /* Release debounce 				*/
} key_press_status_t;	

typedef enum
{
    KEY_NOT_PRESSED     		= 	0,    /* Not pressed 					*/
    KEY_SHORT_PRESSED   		= 	1,    /* Short press 					*/
    KEY_LONG_PRESSED    		= 	2     /* Long press 					*/
} key_function_t;




typedef enum
{
	RISING_EDGE   =    0,  /*上升沿*/
	FALLING_EDGE	
}trigger_type_t;



typedef struct
{
	trigger_type_t trigger_type;
	uint32_t tick;
	
} key_press_irq_status_t;	


/***********************************Defines***********************************/

/***********************************Declaring*********************************/
extern QueueHandle_t g_key_queueHandle;

void key_default_task(void *argument);

/***********************************Declaring*********************************/


#endif // __BSP_KEY_H__

