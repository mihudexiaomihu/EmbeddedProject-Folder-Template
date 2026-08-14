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
  
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__
/***********************************Include***********************************/
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "gpio.h"
/***********************************Include***********************************/
/***********************************Defines***********************************/
#define KEY_UNPRESSED     0
#define KEY_PRESSED       1

typedef enum
{	
    KEY_OK                  =   0,      /* Indicates that the KEY operation succeeded.                      	*/ 	
    KEY_ERROR               =   1,      /* Indicates a general error occurred during the KEY operation.     	*/ 	 
    KEY_ERROR_TIMEOUT       =   2,      /* Indicates that the KEY operation timed out.                      	*/ 	 
    KEY_ERROR_RESOURCES     =   3,      /* Indicates insufficient resources to complete the KEY operation.   	*/	 
    KEY_ERROR_PARAMETER     =   4,      /* Indicates that an invalid parameter was passed.                     	*/
    KEY_ERROR_NOMEMORY      =   5,      /* Indicates insufficient memory to complete the KEY operation.        	*/
    KEY_ERROR_ISR           =   6,      /* Indicates an error occurred in the ISR (Interrupt Service Routine). 	*/ 
    KEY_ERROR_BUSY          =   7,      /* Indicates the KEY is busy and cannot execute the operation.         	*/
    KEY_ERROR_RESERVED      =   0xFF,   /* Reserved error code for future expansion.                           	*/

}key_status_t;

typedef enum
{
    KEY_STATE_IDLE                  = 0,            // 空闲/按键释放
    KEY_STATE_DEBOUNCE              = 1,            // 按下消抖
    KEY_STATE_PRESSED               = 2,            // 已确认按下，等待判断长短按
    KEY_STATE_LONG_PRESS            = 3,            // 已达到长按时间
    KEY_STATE_RELEASE_DEBOUNCE      = 4             // 释放消抖
} key_press_state_t;

typedef enum
{
    KEY_NOT_PRESSED                 = 0,            // 空闲/按键释放
    KEY_SHORT_PRESSED               = 1,            // 短按
    KEY_LONG_PRESSED                = 2             // 长按
} key_function_t;

/***********************************Defines***********************************/
/***********************************Declaring*********************************/

//key_status_t key_scan(uint8_t *key_value);

key_status_t key_scan_short_long_press( key_function_t *key_value, 
                                        uint32_t short_press_time, 
                                        uint32_t long_press_time            );
                                        
/***********************************Declaring*********************************/
#endif//__BSP_KEY_H__

