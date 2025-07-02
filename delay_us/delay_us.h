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


#define TIMER_INSTANCE 	TIM2	//modify if using a different timer
#define TIMER_MAX  		0xFFFFFFFF	//modify to 0xFFFF if using 16bit timer


void delay_us(uint32_t delay);
void delayus_init(void);




#endif /* DELAY_US_DELAY_US_H_ */
