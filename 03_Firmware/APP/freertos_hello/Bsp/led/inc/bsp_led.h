 /****************************************************************************​
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.​
 * ​
 * All Rights Reserved.​
 * ​
 * @file bsp_led.h​
 * ​
 * @par dependencies ​
 * - stdio.h​
 * - stdint.h​
 * - stm32f4xx_hal.h​
 * ​
 * @author Wzh | R&D Dept. | EternalChip 立芯嵌入式​
 * ​
 * @brief Provide the HAL APIs of led and corresponding opetions.​
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


#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/***********************************Include***********************************/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include "gpio.h"
/***********************************Include***********************************/

/***********************************Defines***********************************/

typedef enum
{	
    LED_OK                  = 0,      /* Success 							*/
    LED_ERROR               = 1,      /* General error 						*/
    LED_ERROR_TIMEOUT       = 2,      /* Timeout							*/
    LED_ERROR_RESOURCES     = 3,      /* Resource shortage 					*/
    LED_ERROR_PARAMETER     = 4,      /* Invalid parameter 					*/
    LED_ERROR_NOMEMORY      = 5,      /* Out of memory 						*/
    LED_ERROR_ISR           = 6,      /* ISR error 							*/
    LED_ERROR_BUSY          = 7,      /* Device busy 						*/
    LED_ERROR_RESERVED      = 0xFF,   /* Reserved 							*/
} led_status_t;


/***********************************Defines***********************************/

/***********************************Declaring*********************************/
led_status_t led_toggle(void);

led_status_t led_toggleblink(uint8_t blink_count, uint32_t blink_delay);

/***********************************Declaring*********************************/


#endif // __KEY_H__

