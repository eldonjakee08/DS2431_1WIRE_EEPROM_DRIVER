




#include "delay_us.h"

//internal functions
static void Error_Handler(void);
static void tim_MSP_init(void);


//TIM2 handle structure
TIM_HandleTypeDef htim2;

//initialize TIMER peripheral and start time base generation
void delayus_init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};


	htim2.Instance = TIMER_INSTANCE;
	htim2.Init.Prescaler = ( HAL_RCC_GetHCLKFreq() / 1e6) - 1; //GET 1Mhz TIM2 cnt_clk for 1us period
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = TIMER_MAX; //max counter value
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	//implement low level MSP inits internal to function not having to code in MSP init
	tim_MSP_init();

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}

	//use internal clock for TIM2
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	//start TIM2 upcount
	HAL_TIM_Base_Start(&htim2);
}


/*
 * Blocking delay in us
 */
void delay_us(uint32_t delay)
{
	volatile uint32_t timeStart = TIMER_INSTANCE->CNT;

	while (1) {
		volatile uint32_t currentTime = TIMER_INSTANCE->CNT;

		// Handle timer overflow
		if (currentTime >= timeStart) {
			if ( (currentTime - timeStart) >= delay)
			{
				break;
			}
		}else{
			if ( ( (TIMER_MAX - timeStart) + currentTime + 1) >= delay)
			{
				break;
			}
		}
	}
}


//low level initialization for timer peripheral
static void tim_MSP_init(void)
{
	//if using other timers, modify the instance to desired timer
	if(htim2.Instance==TIM2)
	{
		//ENABLE TIM2 PERIPHERAL CLOCK
		__HAL_RCC_TIM2_CLK_ENABLE();


	}
}


static void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}
