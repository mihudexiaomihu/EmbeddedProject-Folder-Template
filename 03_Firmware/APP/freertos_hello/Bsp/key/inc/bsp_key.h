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
 * ​
 * @author Jack | R&D Dept. | EternalChip 立芯嵌入式​
 * ​
 * @brief Provide the HAL APIs of ley and corresponding opetions.​
 * ​
* Processing flow:​
 * ​
 * call directly.​
 * ​
 * @version V1.0 2026-08-13
 *​
 * @note 1 tab == 4 spaces!​
* ​
 *****************************************************************************/


#ifndef __KEY_H__
#define __KEY_H__

/***********************************Include********************************************/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include "gpio.h"
/***********************************Include********************************************/

/***********************************Defines********************************************/
typedef enum
{	
	 KEY_OK                  =	0,      /* Indicates that the LED operation succeeded.                      	*/ 	
	 KEY_ERROR               =	1,      /* Indicates a general error occurred during the LED operation.     	*/ 	 
	 KEY_ERROR_TIMEOUT       =	2,      /* Indicates that the LED operation timed out.                      	*/ 	 
	 KEY_ERROR_RESOURCES     =	3,      /* Indicates insufficient resources to complete the LED operation.   	*/	 
	 KEY_ERROR_PARAMETER     =	4,      /* Indicates that an invalid parameter was passed.                     	*/
	 KEY_ERROR_NOMEMORY      =	5,      /* Indicates insufficient memory to complete the LED operation.        	*/
	 KEY_ERROR_ISR           =	6,      /* Indicates an error occurred in the ISR (Interrupt Service Routine). 	*/ 
	 KEY_ERROR_BUSY          =	7,      /* Indicates the LED is busy and cannot execute the operation.         	*/
	 KEY_ERROR_RESERVED      =	0xFF,   /* Reserved error code for future expansion.                           	*/
}key_status_t;


typedef enum
{
	KEY_PRESSED 			 =	0,		/* Reserved error code for future expansion.                           	*/
	KEY_UNPRESSED 			 =	1		/* Reserved error code for future expansion.                           	*/
}key_press_status_t;
/***********************************Defines********************************************/

/***********************************Declaring******************************************/
key_status_t key_scan(key_press_status_t *key_value);


/***********************************Declaring******************************************/


#endif // __KEY_H__

