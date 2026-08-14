/**
 * @file bsp_key.c
 * @brief Polling implementation of the board key driver.
 *
 * @author wzh
 * @version 1.0
 * @date 2026-08-13
 */

#include "bsp_key.h"

/** Maximum number of GPIO samples performed by one scan. */
#define KEY_SCAN_POLL_LIMIT (10000UL)

/**
 * @brief Detect one event for each active-low key press.
 *
 * A press is reported only when the key changes from released to pressed.
 * Holding the key does not generate another press event. The key must be
 * fully released before the next press can be reported.
 *
 * @param[out] key_value Destination for the sampled key state.
 *
 * @retval KEY_OK The key was detected as pressed.
 * @retval KEY_ERROR_TIMEOUT No new press event was detected.
 * @retval KEY_ERROR_PARAMETER key_value is NULL.
 */
key_status_t key_scan(key_press_status_t *key_value)
{
    /* Local variables ----------------------------------------------------- */
    static uint8_t key_is_armed = 1U;
    uint32_t timeout_count = 0;
    uint8_t pressed_level_seen = 0U;

    /* Validate the output parameter -------------------------------------- */
    if (NULL == key_value)
    {
        return KEY_ERROR_PARAMETER;
    }

    *key_value = KEY_UNPRESSED;

    /* Poll for a newly pressed active-low key ---------------------------- */
    while (timeout_count < KEY_SCAN_POLL_LIMIT)
    {
        if (HAL_GPIO_ReadPin(KEY_GPIO_PIN_GPIO_Port,
                             KEY_GPIO_PIN_Pin) == GPIO_PIN_RESET)
        {
            pressed_level_seen = 1U;

            if (1U == key_is_armed)
            {
                key_is_armed = 0U;
                *key_value = KEY_PRESSED;
                return KEY_OK;
            }
        }

        timeout_count++;
    }

    /* Rearm only after one complete scan observes the released level ----- */
    if (0U == pressed_level_seen)
    {
        key_is_armed = 1U;
    }

    /* Report that no new press event was detected ------------------------ */
    return KEY_ERROR_TIMEOUT;
}
