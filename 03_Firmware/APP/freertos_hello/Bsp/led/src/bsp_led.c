/**
 ******************************************************************************
 * Copyright (C) 2026 EternalChip, Inc. or its affiliates.
 * All Rights Reserved.
 *
 * @file bsp_led.c
 * @brief Implement board-level LED control and non-blocking blinking.
 *
 * @par Dependencies
 * - bsp_led.h
 * - STM32 HAL GPIO
 *
 * @par Processing flow
 * Call led_toggle() directly or call led_toggleblink() periodically.
 *
 * @author Wzh | R&D Dept. | EternalChip
 * @version V1.0 2026-08-15
 *
 * @note One tab equals four spaces.
 ******************************************************************************
 */

#include "bsp_led.h"

/**
 * @brief Toggle the current LED output level.
 *
 * @retval LED_OK The LED output was toggled.
 */
led_status_t led_toggle(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_GPIO_Port, LED_GPIO_Pin);
    return LED_OK;
}

/**
 * @brief Blink the LED without blocking the caller.
 *
 * Call this function periodically until it returns LED_OK.
 *
 * @param[in] blink_count Number of complete on/off cycles.
 * @param[in] blink_delay Calls required between output toggles.
 *
 * @retval LED_ERROR_BUSY Blinking is still in progress.
 * @retval LED_OK The requested blink sequence is complete.
 */
led_status_t led_toggleblink(
    uint8_t blink_count,
    uint32_t blink_delay)
{
    static uint8_t count = 0;
    static uint32_t time = 0;

    /* Count periodic calls ------------------------------------------------ */
    time++;

    /* Toggle the LED after one delay interval ----------------------------  */
    if (time >= blink_delay)
    {
        time = 0;

        if (count < (blink_count * 2U))
        {
            count++;
            HAL_GPIO_TogglePin(LED_GPIO_GPIO_Port, LED_GPIO_Pin);
            return LED_ERROR_BUSY;
        }
        else
        {
            /* Reset the state for the next blink request -----------------  */
            count = 0;
            HAL_GPIO_WritePin(	LED_GPIO_GPIO_Port	,
								LED_GPIO_Pin		,
								GPIO_PIN_SET		);
            return LED_OK;
        }
    }

    return LED_ERROR_BUSY;
}
