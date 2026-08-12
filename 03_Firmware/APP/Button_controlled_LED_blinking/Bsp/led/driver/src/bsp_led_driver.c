/**
  *************************************************************************************
  * @file bsp_led_driver.h 
  * @brief 
  * 
  * @author     wangzihang  
  * @date       2024-10-18
  * @version         1.0.0
  * 
  * @note 
  * @warning 
  *************************************************************************************
  */

/***********************************Include********************************************/
#include "bsp_led_driver.h"
/***********************************Include********************************************/

/***********************************Defines********************************************/
led_status_t led_driver_init(
                                      bsp_led_driver_t   const      *self,
                                const led_operations_t   const   *led_ops,
#ifdef OS_SUPPPORTING                                
                                const os_delay_t         const  *os_delay,  
#endif//OS_SUPPPORTING
								const time_base_ms_t     const *time_base   )   
{
    if (NULL == self        ||
        NULL == led_ops     ||
        NULL == os_delay    ||
        NULL == time_base   
    )
    {
#ifdef DEBUG
        DEBUG_OUT("inout prars error\r\n");
        return LED_ERROR_PARAMETER;
#endif 
    }
    
    led_status_t ret = LED_OK;

#ifdef DEBUG
        DEBUG_OUT("led inst start\r\n");
#endif     

    if (NOT_INITED == self->is_inited)
    {
#ifdef DEBUG
        DEBUG_OUT("LED_ERROR_RESOURCES\r\n");
        return LED_ERROR_RESOURCES;
#endif 
    }
    

#ifdef DEBUG
        DEBUG_OUT("led_inst_start\r\n");
#endif 

    return ret;
}


/***********************************Defines********************************************/

/***********************************Declaring******************************************/
/***********************************Declaring******************************************/