/**
  *****************************************************************************
  * @file   bsp_key.h
  * @brief  Type definitions and public interfaces for the board-level KEY driver.
  * 
  * This file abstracts the GPIO operations, OS delay, and millisecond time base
  * required by the KEY. Upper-layer modules depend only on these common interfaces
  * and are therefore independent of a specific MCU or RTOS implementation.
  * It provides:
  * - KEY driver status codes and on/off duty-ratio definitions;
  * - Callback interfaces for KEY control, OS delay, and the time base;
  * - The bsp_key_driver_t driver object;
  * - The key_driver_inst() constructor for a KEY driver object.
  * 
  * @author     wangzihang  
  * @date       2024-10-18
  * @version         1.0.0
  * 
  * @note        Before calling key_driver_inst(), the caller must prepare all
  *              low-level callback objects and keep them valid for the lifetime
  *              of the KEY driver.
  * @warning     This driver accesses hardware through function pointers. A NULL
  *              or invalid callback may cause a runtime fault.
  ****************************************************************************/
  
#include "bsp_key.h"



//key_status_t key_scan(uint8_t *key_value)
//{
//    uint8_t key_pressed_value = KEY_UNPRESSED;
//    uint32_t key_timeout_count   = 0;

//    // 检测按键是否按下
//    while (key_timeout_count <1000) // 假设超时计数为1000
//    {
//        // 检查按键状态
//        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin( KEY_GPIO_GPIO_Port, 
//                                                KEY_GPIO_Pin)) // 假设按键按下时为低电平
//        {
//            key_pressed_value = KEY_PRESSED; // 按键按下
//            *key_value = key_pressed_value; // 将按键状态传递给调用者
//            return KEY_OK; // 返回成功状态
//        }
//        else
//        {
//            key_timeout_count ++; // 按键未按下
//        }
//    }

//    return KEY_ERROR_TIMEOUT; // 返回超时状态

//}

key_status_t key_scan_short_long_press( key_function_t *key_value, 
                                        uint32_t short_press_time, 
                                        uint32_t long_press_time)

{
    uint32_t key_timeout_count   = 0;
    key_function_t key_pressed_value = KEY_NOT_PRESSED;
    key_press_state_t key_state = KEY_STATE_IDLE;
    // 检测按键是否按下
    while (key_timeout_count < long_press_time) // 假设超时计数为long_press_time
    {
        switch(key_state)
        {
            case KEY_STATE_IDLE:
                // 假设按键按下时为低电平
                if (GPIO_PIN_RESET == HAL_GPIO_ReadPin( KEY_GPIO_GPIO_Port, 
                                            KEY_GPIO_Pin)) 
                {
                    // 进入按下消抖状态
                    key_state = KEY_STATE_DEBOUNCE; 
                }
                break;

            case KEY_STATE_DEBOUNCE:
                // 假设按键按下时为低电平
                if (GPIO_PIN_RESET == HAL_GPIO_ReadPin( KEY_GPIO_GPIO_Port, 
                                        KEY_GPIO_Pin)) 
                {
                    // 按键按下，计数增加
                    key_timeout_count ++; 
                    // 达到短按时间
                    if (key_timeout_count >= short_press_time) 
                    {
                        // 进入已确认按下状态
                        key_state = KEY_STATE_PRESSED; 
                    }
                }
                else
                {
                    // 按键释放，进入释放消抖状态
                    key_state = KEY_STATE_RELEASE_DEBOUNCE; 

                }
                break;

            case KEY_STATE_PRESSED:
                // 假设按键按下时为低电平
                if (GPIO_PIN_RESET == HAL_GPIO_ReadPin( KEY_GPIO_GPIO_Port, 
                                        KEY_GPIO_Pin)) 
                {
                    // 按键按下，计数增加 
                    key_timeout_count ++; 
                    // 达到长按时间，进入长按状态
                    if (key_timeout_count >= long_press_time)
                    {
                        key_state = KEY_STATE_LONG_PRESS; 
                    }
                }
                else
                {
                    // 按键释放，返回短按状态
                    key_pressed_value = KEY_SHORT_PRESSED; 
                    // 按键释放，进入释放消抖状态
                    key_state = KEY_STATE_RELEASE_DEBOUNCE; 
                }
                break;

            case KEY_STATE_LONG_PRESS:
                // 假设按键释放时为高电平
                if (GPIO_PIN_SET == HAL_GPIO_ReadPin( KEY_GPIO_GPIO_Port, 
                                            KEY_GPIO_Pin)) 
                {
                    // 按键释放，返回长按状态
                    key_pressed_value = KEY_LONG_PRESSED; 
                    // 按键释放，进入释放消抖状态
                    key_state = KEY_STATE_RELEASE_DEBOUNCE; 
                }

                break;

            case KEY_STATE_RELEASE_DEBOUNCE:
                // 假设按键释放时为高电平
                if (GPIO_PIN_SET == HAL_GPIO_ReadPin( KEY_GPIO_GPIO_Port, 
                                            KEY_GPIO_Pin)) 
                {
                    // 按键释放，计数增加
                    key_timeout_count ++; 
                    // 假设释放消抖时间
                    if (key_timeout_count >= 50) 
                    {
                        // 将按键状态传递给调用者
                        *key_value = key_pressed_value; 
                        // 进入空闲状态
                        key_state = KEY_STATE_IDLE; 
                        // 返回成功状态
                        return KEY_OK; 
                    }
                }
                break;  
            }
    }
    return KEY_ERROR_TIMEOUT; // 返回超时状态

}

