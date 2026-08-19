/**
 ******************************************************************************
 * Copyright (C) 2026 EternalChip, Inc. or its affiliates.
 * All Rights Reserved.
 *
 * @file bsp_led.c
 * @brief Implement board-level LED control and non-blocking blinking.
 *
 * @par Dependencies
 * - bsp_led.h
 * - STM32 HAL GPIO
 *
 * @par Processing flow
 * Call led_toggle() directly or call led_toggleblink() periodically.
 *
 * @author Wzh | R&D Dept. | EternalChip
 * @version V1.0 2026-08-15
 *
 * @note One tab equals four spaces.
 ******************************************************************************
 */

#include "bsp_led.h"

