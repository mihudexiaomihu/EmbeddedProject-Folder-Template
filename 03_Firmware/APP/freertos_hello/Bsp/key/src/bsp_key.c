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
 * @version V1.0 2026-08-13​
 *​
 * @note 1 tab == 4 spaces!​
* ​
 **************************************************************************************/

#include "bsp_key.h"

key_status_t key_scan(key_press_status_t *key_value)
{
	uint32_t timeout_count = 0;
	key_press_status_t key_press_value = KEY_UNPRESSED;
	
	while(timeout_count < 10000)
	{
		if( HAL_GPIO_ReadPin(KEY_GPIO_PIN_GPIO_Port,KEY_GPIO_PIN_Pin) == GPIO_PIN_RESET)
		{
			key_press_value = KEY_PRESSED;
			*key_value = key_press_value;
			return KEY_OK;
		}
		timeout_count ++;
	}
	*key_value = key_press_value;
	return KEY_ERROR_TIMEOUT;
}
