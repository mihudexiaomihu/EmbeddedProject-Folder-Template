/**
 * @file bsp_key.h
 * @brief Public types and interface for the board key driver.
 *
 * The driver polls the key GPIO and reports whether the key is pressed.
 * The GPIO port, pin, and active level are provided by the CubeMX-generated
 * board configuration.
 *
 * @author wzh
 * @version 1.0
 * @date 2026-08-13
 */

#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__
/***********************************Include**********************************/
#include <stdint.h>
#include "gpio.h"
#include "stm32f4xx_hal.h"
/***********************************Include**********************************/
/***********************************Defines**********************************/
/**
 * @brief Return codes used by the key driver.
 */
typedef enum
{
    KEY_OK              = 0,    /**< Operation completed successfully.      */
    KEY_ERROR           = 1,    /**< An unspecified error occurred.         */
    KEY_ERROR_TIMEOUT   = 2,    /**< No press was found before timeout.     */
    KEY_ERROR_RESOURCES = 3,    /**< Required resources are unavailable.    */
    KEY_ERROR_PARAMETER = 4,    /**< An input parameter is invalid.         */
    KEY_ERROR_NOMEMORY  = 5,    /**< Insufficient memory.                   */
    KEY_ERROR_ISR       = 6,    /**< Operation failed in interrupt context. */
    KEY_ERROR_BUSY      = 7,    /**< The driver is busy.                    */
    KEY_ERROR_RESERVED  = 0xFF  /**< Reserved for future use.               */
} key_status_t;

/**
 * @brief Logical key states.
 *
 * The key input is active low: a low GPIO level maps to KEY_PRESSED.
 */
typedef enum
{
    KEY_PRESSED         = 0,    /**< The key input is active.               */
    KEY_UNPRESSED       = 1     /**< The key input is inactive.             */
} key_press_status_t;

/***********************************Defines**********************************/

/**
 * @brief Detect one event for each active-low key press.
 *
 * @param[out] key_value Destination for the sampled logical key state.
 *
 * @retval KEY_OK A new key press was detected.
 * @retval KEY_ERROR_TIMEOUT No new key press was detected.
 * @retval KEY_ERROR_PARAMETER key_value is NULL.
 *
 * @note Holding the key does not produce repeated press events. The key must
 *       be released before another press can be detected.
 */
key_status_t key_scan(key_press_status_t *key_value);

#endif /* __BSP_KEY_H__ */
