/**
  *************************************************************************************
  * @file bsp_led_driver.h​ 3  
  * @brief 文件功能简述​ 
  * 
  * 详细描述：
  * - 功能1：具体描述
  * - 功能2：具体描述
  * - ……
  * 
  * @author     wangzihang  
  * @date       2024-10-18
  * @version         1.0.0
  * 
  * @note 备注信息 (可选)
  * @warning 警告信息 (可选)​ 
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
    PROPORTION_1_3 			= 0,		/* Reserved error code for future expansion.                           	*/
    PROPORTION_1_2 			= 1,		/* Reserved error code for future expansion.                           	*/
    PROPORTION_1_1 			= 2,		/* Reserved error code for future expansion.                           	*/
    PROPORTION_x_x 			= 0xFF,		/* Reserved error code for future expansion.                           	*/
}proportion_t;

typedef struct 
{
    led_status_t (*pf_led_on)		(void);		/* Reserved error code for future expansion.                   	*/
    led_status_t (*pf_led_off)		(void);		/* Reserved error code for future expansion.                 	*/
}led_operations_t;

typedef struct 
{
    led_status_t (*pf_get_time_ms)     ( uint32_t * const );	/* Reserved error code for future expansion.  	*/
}time_base_ms_t;

#ifdef OS_SUPPPORTING     
typedef struct 
{
    led_status_t (*pf_os_delay)        ( const   uint32_t );	/* Reserved error code for future expansion. 	*/
}os_delay_t;
#endif//OS_SUPPPORTING    

typedef led_status_t (*pf_led_control_t)(
                                        uint32_t,
                                        uint32_t,
                                        proportion_t);

typedef struct
{
/****************************结构体参数状态*****************************/    
    uint8_t                        is_inited;
/*****************************外部需求接口******************************/
    uint32_t                  period_time_ms;            /*blink period*/
    uint32_t                     blink_times;             /*blink times*/ 
    proportion_t            proportion_on_of;        /*blink proportion*/
/***************************内部实现使用的接口**************************/
//Core
    led_operations_t        *p_led_opes_inst;            /*gpio contral*/
    time_base_ms_t           *p_time_base_ms;                /*MCU Tick*/

#ifdef OS_SUPPPORTING     
//FreeRTOS
    os_delay_t                   *os_delay_t;               /*CPU delay*/
#endif//OS_SUPPPORTING    
/*****************************向外部提供接口****************************/
    pf_led_control_t         pf_led_controll;             /*contrl  led*/

}bsp_led_driver_t;


/***********************************Defines********************************************/

/***********************************Declaring******************************************/

led_status_t led_driver_init(
                                      bsp_led_driver_t   const      *self,
                                const led_operations_t   const   *led_ops,
#ifdef OS_SUPPPORTING                                
                                const os_delay_t         const  *os_delay,  
#endif//OS_SUPPPORTING
								const time_base_ms_t     const *time_base   ) ;

/***********************************Declaring******************************************/
#endif
