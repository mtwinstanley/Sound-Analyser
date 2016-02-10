#include "samplingTimer.h"
#include "stm32f4xx_tim.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_usart.h"
#include "stm32f4xx.h"

/**
  * Name: LPFClock_init
  * Description: Initialises the timer
  *              This sets the prescaler to divide the APB1 Clock source (84MHz) to give a frequency of 1.46MHz 
  *              The interupt bit is set and the interrupt request (IRQ) is enabled 
  *              This was written using the STM32 Family Reference Manual Rev 11 Pages (183-187), (373-390) and (582-641) 
  * Arguments: void
  * Returns: void  */   
void LPFClock_init() {
	// Variable definititions
	TIM_TimeBaseInitTypeDef TIM3_timeBaseStructure;
	NVIC_InitTypeDef nvic3Structure;
	GPIO_InitTypeDef GPIO_InitDef;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);							// Enable Clock for Timer 3
	/* Timer 3 is connected to APB1, which runs at 42MHz
		 Timer has internal PLL, which doubles this to 84MHz for use in the timer
		 Prescaler is set to give he timer count frequency
		 tick_frequency = default_frequency / (prescaler + 1)
		 In this case: tick_frequency = 84MHz / (0+1) = 84MHz
	*/
	TIM3_timeBaseStructure.TIM_Prescaler = 0;		
	// Count Up
	TIM3_timeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	/* Set timer period for when to reset
		 The cut off frequency of the filter is the incoming clock / 32
		 This clock source is driven by a toggled GPIO pin which changes every period
		 In order to achieve the desired frequency, the timer tick must be set to twice the frequency of the required filter.
		 
		 clock frequency = cut off frequency * 32
		 frequency = clock frequency * 2
		 period = tick_frequency / frequency +1
		 In this case: frequency = 23kHz * 32 * 2 = 1.472MHz
		 period = 84MHz / 1.472MHz + 1 = 57 (57.1) + 1
		 
	*/
	TIM3_timeBaseStructure.TIM_Period = 58;
	TIM3_timeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_timeBaseStructure.TIM_RepetitionCounter = 0;
	// Initialise Timer 3
	TIM_TimeBaseInit(TIM3, &TIM3_timeBaseStructure);
	// Start count on Timer 3
	TIM_Cmd(TIM3, ENABLE);
	
	// Generate Update Event
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* Initialise the timer interupt
	*/
	nvic3Structure.NVIC_IRQChannel = TIM3_IRQn;
	nvic3Structure.NVIC_IRQChannelPreemptionPriority = 0;
	nvic3Structure.NVIC_IRQChannelSubPriority = 1;
	nvic3Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic3Structure);
	
	/* Initialise the GPIO pin PE0
		 This will toggle on each timer tick to generate a clock signal to send to the LPF
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitDef);
}

/**
  * Name: TIM3_IRQHandler
  * Description: Interrupt handler for Timer 3: Defined in stm32f4xx.s
  * Arguments: void
  * Returns: void  */ 
void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
	}
}
