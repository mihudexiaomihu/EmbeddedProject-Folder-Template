 /*************************************************************************************?
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_led_driver.c
 * 
 * @par dependencies 
 * - ec_bsp_aht21_reg.h
 * - stdio.h?
 * - stdint.h?
 * 
 * @author wzh | R&D Dept. | EternalChip
 * 
 * @brief Provide the HAL APIs of key and corresponding opetions.
 * 
 * Processing flow:
 * 
 * call directly.
 *
 * @version V1.0 2026-8-14
 
 * @note 1 tab == 4 spaces!
 * 
 **************************************************************************************/

/***********************************Include********************************************/
#include "bsp_led_driver.h"
/***********************************Include********************************************/

/***********************************Defines********************************************/

/**
 * @brief  Performs internal initialization of an LED driver object.
 * @param[in,out] self LED driver object with its low-level operations already bound.
 * @retval LED_OK Initialization succeeded and the LED is off by default.
 * @retval LED_ERROR_PARAMETER self is NULL.
 * @note This function is used internally by the constructor. Do not call it
 *       before binding the dependency interfaces.
 */
led_status_t led_driver_init(bsp_led_driver_t *self)
{
	led_status_t ret = LED_OK;
	DEBUG_OUT("led inst kick-off\r\n");
	
    if(NULL == self)
    {
#ifdef DEBUG
        DEBUG_OUT("inout prars error\r\n");
        return LED_ERROR_PARAMETER;
#endif 
    }

    self->p_led_opes_inst->pf_led_off();

	return ret;

}




/**
 * @brief  Constructs an LED driver object and binds its low-level dependencies.
 *
 * The constructor validates its inputs, checks the object state, stores the
 * dependency interfaces, restores the default blink parameters, and calls
 * led_driver_init() to switch the LED off. See bsp_led_driver.h for the full
 * parameter and return-value contract.
 */
led_status_t led_driver_inst(
                                      bsp_led_driver_t              *self,
                                      led_operations_t      *led_ops,
#ifdef OS_SUPPPORTING                                
                                      os_delay_t           *os_delay,  
#endif//OS_SUPPPORTING
								      time_base_ms_t      *time_base   )   
{
    led_status_t ret = LED_OK;
	DEBUG_OUT("led inst start\r\n");
/****************************Input validation*****************************/	
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
    
/****************************Object state check**************************/	
	if (INITED == self->is_inited)
    {
#ifdef DEBUG
        DEBUG_OUT("LED_ERROR_RESOURCES\r\n");
        // return LED_ERROR_RESOURCES;
#endif 
    }

#ifdef DEBUG
        DEBUG_OUT("led_inst_start\r\n");
#endif 
/****************************Bind dependencies and reset defaults*******/	
	self->p_led_opes_inst = led_ops;
    self->os_delay_t = os_delay;
    self->p_time_base_ms = time_base;

    self->period_time_ms = 0;
	self->blink_times = 0;
	self->proportion_on_of = PROPORTION_x_x;
	
    ret = led_driver_init(self);

    return ret;
}


/***********************************Defines********************************************/

/***********************************Declaring******************************************/
/***********************************Declaring******************************************/
