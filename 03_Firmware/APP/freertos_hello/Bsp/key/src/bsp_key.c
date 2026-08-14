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
 * @brief Poll the active-low key input.
 *
 * The function returns immediately when a pressed state is observed. If the
 * input remains inactive for KEY_SCAN_POLL_LIMIT samples, it reports a
 * timeout and writes KEY_UNPRESSED to the output parameter.
 *
 * @param[out] key_value Destination for the sampled key state.
 *
 * @retval KEY_OK The key was detected as pressed.
 * @retval KEY_ERROR_TIMEOUT The polling limit expired without a press.
 */
key_status_t key_scan(key_press_status_t *key_value)
{
    /* Local variables ----------------------------------------------------- */
    uint32_t timeout_count = 0;
    key_press_status_t key_press_value = KEY_UNPRESSED;

    /* Poll for an active-low key press ----------------------------------- */
    while (timeout_count < KEY_SCAN_POLL_LIMIT)
    {
        if (HAL_GPIO_ReadPin(KEY_GPIO_PIN_GPIO_Port,
                             KEY_GPIO_PIN_Pin) == GPIO_PIN_RESET)
        {
            key_press_value = KEY_PRESSED;
            *key_value = key_press_value;
            return KEY_OK;
        }

        timeout_count++;
    }

    /* Report polling timeout --------------------------------------------- */
    *key_value = key_press_value;
    return KEY_ERROR_TIMEOUT;
}
