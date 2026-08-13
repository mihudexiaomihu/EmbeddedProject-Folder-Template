 /*************************************************************************************​
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.​
 * ​
 * All Rights Reserved.​
 * ​
 * @file bsp_key.c​
 * ​
 * @par dependencies ​
 * - ec_bsp_aht21_reg.h​
 * - stdio.h​
 * - stdint.h​
 * ​
 * @author Jack | R&D Dept. | EternalChip 立芯嵌入式​
 * ​
 * @brief Provide the HAL APIs of key and corresponding opetions.​
 * ​
* Processing flow:​
 * ​
 * call directly.​
 * ​
 * @version V1.0 2023-12-03​
 *​
 * @note 1 tab == 4 spaces!​
* ​
 **************************************************************************************/
/***********************************Include********************************************/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdint.h>
/***********************************Include********************************************/

/***********************************Defines********************************************/

typedef enum
{	
	 KEY_OK                  =   0,      /* Indicates that the LED operation succeeded.                      	*/ 	
	 KEY_ERROR               =   1,      /* Indicates a general error occurred during the LED operation.     	*/ 	 
	 KEY_ERROR_TIMEOUT       =   2,      /* Indicates that the LED operation timed out.                      	*/ 	 
	 KEY_ERROR_RESOURCES     =   3,      /* Indicates insufficient resources to complete the LED operation.   	*/	 
	 KEY_ERROR_PARAMETER     =   4,      /* Indicates that an invalid parameter was passed.                     	*/
	 KEY_ERROR_NOMEMORY      =   5,      /* Indicates insufficient memory to complete the LED operation.        	*/
	 KEY_ERROR_ISR           =   6,      /* Indicates an error occurred in the ISR (Interrupt Service Routine). 	*/ 
	 KEY_ERROR_BUSY          =   7,      /* Indicates the LED is busy and cannot execute the operation.         	*/
	 KEY_ERROR_RESERVED      =   0xFF,   /* Reserved error code for future expansion.                           	*/
}key_status_t;


/***********************************Defines********************************************/






/***********************************Declaring******************************************/
