/*
 * delay_us.h
 *
 *  Created on: Jun 8, 2025
 *      Author: eldon
 */

#ifndef DELAY_US_DELAY_US_H_
#define DELAY_US_DELAY_US_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define TIMER_INSTANCE TIM2		//Timer used for timebase generation peripheral base address


void delay_us(uint32_t delay);


#endif /* DELAY_US_DELAY_US_H_ */
