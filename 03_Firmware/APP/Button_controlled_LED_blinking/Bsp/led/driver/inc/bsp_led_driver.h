/**
  *************************************************************************************
  * @file        bsp_led_driver.h
  * @brief       Type definitions and public interfaces for the board-level LED driver.
  * 
  * This file abstracts the GPIO operations, OS delay, and millisecond time base
  * required by the LED. Upper-layer modules depend only on these common interfaces
  * and are therefore independent of a specific MCU or RTOS implementation.
  * It provides:
  * - LED driver status codes and on/off duty-ratio definitions;
  * - Callback interfaces for LED control, OS delay, and the time base;
  * - The bsp_led_driver_t driver object;
  * - The led_driver_inst() constructor for an LED driver object.
  * 
  * @author     wangzihang  
  * @date       2024-10-18
  * @version         1.0.0
  * 
  * @note        Before calling led_driver_inst(), the caller must prepare all
  *              low-level callback objects and keep them valid for the lifetime
  *              of the LED driver.
  * @warning     This driver accesses hardware through function pointers. A NULL
  *              or invalid callback may cause a runtime fault.
  *************************************************************************************
  */
#ifndef __BSP_LED_DRIVER_H__
#define __BSP_LED_DRIVER_H__
/***********************************Include********************************************/

#include <stdio.h>
#include <stdint.h>

/***********************************Include********************************************/
/***********************************Defines********************************************/
#define INITED          1               /* LED is inited                              */
#define NOT_INITED      0               /* LED is not inited                      	  */

#define OS_SUPPPORTING                  /* OS_SUPPPORTING                     	      */
#define DEBUG                           /* OS_SUPPPORTING                     	      */
#define DEBUG_OUT(X)     printf(X)      /* OS_SUPPPORTING                     	      */

typedef enum
{	
    LED_OK                  =   0,      /* Indicates that the LED operation succeeded.                      	*/ 	
    LED_ERROR               =   1,      /* Indicates a general error occurred during the LED operation.     	*/ 	 
    LED_ERROR_TIMEOUT       =   2,      /* Indicates that the LED operation timed out.                      	*/ 	 
    LED_ERROR_RESOURCES     =   3,      /* Indicates insufficient resources to complete the LED operation.   	*/	 
    LED_ERROR_PARAMETER     =   4,      /* Indicates that an invalid parameter was passed.                     	*/
    LED_ERROR_NOMEMORY      =   5,      /* Indicates insufficient memory to complete the LED operation.        	*/
    LED_ERROR_ISR           =   6,      /* Indicates an error occurred in the ISR (Interrupt Service Routine). 	*/ 
    LED_ERROR_BUSY          =   7,      /* Indicates the LED is busy and cannot execute the operation.         	*/
    LED_ERROR_RESERVED      =   0xFF,   /* Reserved error code for future expansion.                           	*/

}led_status_t;

typedef enum
{
    PROPORTION_1_3 			= 0,		/* LED on-to-off time ratio is 1:3. */
    PROPORTION_1_2 			= 1,		/* LED on-to-off time ratio is 1:2. */
    PROPORTION_1_1 			= 2,		/* LED on-to-off time ratio is 1:1. */
    PROPORTION_x_x 			= 0xFF,		/* Ratio is unset or invalid. */
}proportion_t;

typedef struct 
{
    led_status_t (*pf_led_on)		(void);		/* Low-level function that turns the LED on. */
    led_status_t (*pf_led_off)		(void);		/* Low-level function that turns the LED off. */
}led_operations_t;

typedef struct 
{
    led_status_t (*pf_get_time_ms)     ( uint32_t * const );	/* Gets the current time in milliseconds. */
}time_base_ms_t;

#ifdef OS_SUPPPORTING     
typedef struct 
{
    led_status_t (*pf_os_delay)        ( const   uint32_t );	/* OS delay function in milliseconds. */
}os_delay_t;
#endif//OS_SUPPPORTING    

typedef led_status_t (*pf_led_control_t)(
                                        uint32_t,
                                        uint32_t,
                                        proportion_t);

typedef struct
{
/****************************Driver object state*****************************/    
    uint8_t                        is_inited;
/*****************************User configuration******************************/
    uint32_t                  period_time_ms;            /*blink period*/
    uint32_t                     blink_times;             /*blink times*/ 
    proportion_t            proportion_on_of;        /*blink proportion*/
/***************************Internal dependency interfaces*******************/
//Core
    led_operations_t        *p_led_opes_inst;            /*gpio contral*/
    time_base_ms_t           *p_time_base_ms;                /*MCU Tick*/

#ifdef OS_SUPPPORTING     
//FreeRTOS
    os_delay_t                   *os_delay_t;               /*CPU delay*/
#endif//OS_SUPPPORTING    
/*****************************Public operation interface*********************/
    pf_led_control_t         pf_led_controll;             /*contrl  led*/

}bsp_led_driver_t;


/***********************************Defines********************************************/

/***********************************Declaring******************************************/

/**
 * @brief  Constructs and initializes an LED driver object.
 *
 * Injects the LED hardware operations, OS delay, and millisecond time base
 * supplied by the caller into @p self. It then clears the blink period and
 * count, resets the on/off ratio, and leaves the LED off by default.
 *
 * This function acts as a constructor in C. It does not allocate memory. The
 * caller must create the bsp_led_driver_t object and all dependency objects
 * before passing their addresses to this function.
 *
 * @param[in,out] self       LED driver object to construct; must not be NULL.
 * @param[in]     led_ops    Low-level LED on/off operations; must not be NULL.
 * @param[in]     os_delay   OS delay interface; must not be NULL. This parameter
 *                           is present only when OS_SUPPPORTING is defined.
 * @param[in]     time_base  MCU millisecond time-base interface; must not be NULL.
 *
 * @retval LED_OK               Construction and initialization succeeded.
 * @retval LED_ERROR_PARAMETER  self or one of its dependencies is NULL.
 * @retval other                Error status returned by internal initialization.
 *
 * @note All dependency objects and callbacks must remain valid while the driver is used.
 */
led_status_t led_driver_inst(
                                      bsp_led_driver_t              *self,
                                      led_operations_t   const   *led_ops,
#ifdef OS_SUPPPORTING                                
                                      os_delay_t         const  *os_delay,  
#endif//OS_SUPPPORTING
								      time_base_ms_t     const *time_base   ) ;

/***********************************Declaring******************************************/
#endif
