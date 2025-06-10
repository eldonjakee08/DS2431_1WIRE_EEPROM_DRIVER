/*
 * delay_us.c
 *
 *  Created on: Jun 8, 2025
 *      Author: eldon
 */

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "delay_us.h"


void delay_us(uint32_t delay)
{
	uint32_t timeStart = TIMER_INSTANCE->CNT;
	uint32_t wait = delay;

	while( (TIMER_INSTANCE->CNT - timeStart) < wait);
}
