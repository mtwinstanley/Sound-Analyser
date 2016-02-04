#include "samplingTimer.h"
#include "stm32f4xx_tim.h"
#include "tm_stm32f4_adc.h"
#include "stm32f4xx.h"


TIM_TimeBaseInitTypeDef TIM_timeBaseStructure;
NVIC_InitTypeDef nvicStructure;

/**
  * Name: timer_init
  * Description: Initialises the timer
  *              This sets the prescaler to divide the APB1 Clock source (84MHz) to give a frequency of 48kHz 
  *              The interupt bit is set and the interrupt request (IRQ) is enabled 
  *              This was written using the STM32 Family Reference Manual Rev 11 Pages (183-187), (373-390) and (582-641) 
  * Arguments: void
  * Returns: void  */   
void timer_init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);							// Enable Clock for Timer 2
	/* Timer 2 is connected to APB1, which runs at 42MHz
		 Timer has internal PLL, which doubles this to 84MHz for use in the timer
		 Prescaler is set to give he timer count frequency
		 tick_frequency = default_frequency / (prescaler + 1)
		 In this case: tick_frequency = 84MHz / (0+1) = 84MHz
	*/
	TIM_timeBaseStructure.TIM_Prescaler = 0;		
	// Count Up
	TIM_timeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	/* Set timer period for when to reset
		 This uses a similar equation as the prescaler
		 period = tick_frequency / frequency +1
		 To get a 48kHz sampling rate: period = 84MHz / 48kHz + 1 = 1750 - 1
		 stored as a value - 1 so that it is easily re-configurable for different sample rates
	*/
	TIM_timeBaseStructure.TIM_Period = 1750 - 1; // 48kHz
	TIM_timeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_timeBaseStructure.TIM_RepetitionCounter = 0;
	// Initialise Timer 2
	TIM_TimeBaseInit(TIM2, &TIM_timeBaseStructure);
	// Start count on Timer 2
	TIM_Cmd(TIM2, ENABLE);
	
	// Generate Update Event
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* Initialise the timer interupt
	*/
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

/**
  * Name: TIM2_IRQHandler
  * Description: Interrupt handler for Timer 2: Defined in stm32f4xx.s
  * Arguments: void
  * Returns: void  */ 
void TIM2_IRQHandler(void) {
	uint32_t read;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		// GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		read = TM_ADC_Read(ADC1, ADC_Channel_0);
		if (read > 2000){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
		}
		else{
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		}
	}
}
