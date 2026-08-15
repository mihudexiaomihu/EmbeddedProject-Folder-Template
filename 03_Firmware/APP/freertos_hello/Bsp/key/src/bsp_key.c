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

/**
 * @brief Scan the key and detect short- or long-press events.
 *
 * Call this function periodically. Time parameters are measured in calls.
 *
 * @param[out] key_value Detected key event.
 * @param[in] short_press_time Calls required to confirm a short press.
 * @param[in] long_press_time Calls required to confirm a long press.
 *
 * @retval KEY_OK A complete key event was detected.
 * @retval KEY_ERROR_TIMEOUT No complete key event is available.
 * @retval KEY_ERROR_PARAMETER key_value is NULL.
 */
key_status_t key_scan_short_long_press(	key_function_t *key_value	,
										uint32_t short_press_time	,
										uint32_t long_press_time	)
{
    static uint32_t key_timeout_count 	= 	0;
	key_function_t key_pressed_value 	= 	KEY_NOT_PRESSED;
    static key_press_status_t key_state = 	KEY_STATE_IDLE;

    if (key_value == NULL)
    {
        return KEY_ERROR_PARAMETER;
    }

    switch (key_state)
    {
        /* Wait for the active-low key input ------------------------------ */
        case KEY_STATE_IDLE:
            if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(	KEY_GPIO_GPIO_Port	,
													KEY_GPIO_Pin		))
            {
                key_timeout_count 	= 	0;
                key_pressed_value 	= 	KEY_NOT_PRESSED;
                key_state 			= 	KEY_STATE_DEBOUNCE;
            }
            break;

        /* Confirm that the key press is stable --------------------------- */
        case KEY_STATE_DEBOUNCE:
            if ( GPIO_PIN_RESET == HAL_GPIO_ReadPin(	KEY_GPIO_GPIO_Port,
														KEY_GPIO_Pin))
            {
                key_timeout_count++;

                if (key_timeout_count >= short_press_time)
                {
                    key_state 	= 	KEY_STATE_PRESSED;
                }
            }
            else
            {
                key_timeout_count 	= 	0;
                key_pressed_value	= 	KEY_NOT_PRESSED;
                key_state 			= 	KEY_STATE_RELEASE_DEBOUNCE;
            }
            break;

        /* Measure how long the key remains pressed ----------------------- */
        case KEY_STATE_PRESSED:
            if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(	KEY_GPIO_GPIO_Port	,
													KEY_GPIO_Pin		))
            {
                key_timeout_count++;

                if (key_timeout_count >= long_press_time)
                {
                    key_timeout_count 	= 	0;
                    key_state 			= 	KEY_STATE_LONG_PRESS;

                }
            }
            else
            {
                key_pressed_value	=	KEY_SHORT_PRESSED;
                key_timeout_count	=	0;
                key_state 			= 	KEY_STATE_RELEASE_DEBOUNCE;
            }
            break;

        /* Generate one long-press event ---------------------------------- */
        case KEY_STATE_LONG_PRESS:

				key_timeout_count 	= 	0;
				key_pressed_value 	= 	KEY_LONG_PRESSED;
				key_state 			= 	KEY_STATE_RELEASE_DEBOUNCE;

            break;

        /* Confirm release before accepting another press ---------------- */
        case KEY_STATE_RELEASE_DEBOUNCE:
            if (GPIO_PIN_SET == HAL_GPIO_ReadPin(	KEY_GPIO_GPIO_Port	,
													KEY_GPIO_Pin		))
            {
                key_timeout_count++;

                if (key_timeout_count >= 5U)
                {
                    key_timeout_count 	= 	0;
                    key_state 			= 	KEY_STATE_IDLE;

                    return KEY_OK;
                }
            }
            else
            {
                key_timeout_count = 0;
            }
            break;

        /* Recover the state machine from an invalid state ---------------- */
        default:
            key_timeout_count 	= 	0;
            key_pressed_value 	= 	KEY_NOT_PRESSED;
            key_state 			= 	KEY_STATE_IDLE;
            break;
    }

	*key_value 			= 	key_pressed_value;
	key_pressed_value 	= 	KEY_NOT_PRESSED;

    return KEY_ERROR_TIMEOUT;
}
