#include <math.h>
#include "samplingTimer.h"
#include "config.h"
#include "stm32f4xx_tim.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_usart.h"
#include "stm32f4xx.h"

#define defaultSamplingRate 48000

TIM_TimeBaseInitTypeDef TIM_timeBaseStructure;
NVIC_InitTypeDef nvicStructure;

extern RCC_ClocksTypeDef clocks; 
extern uint32_t ADC_read;

/**
  * Name: timer_init
  * Description: Initialises the timer
  *              This sets the prescaler to divide the APB1 Clock source (84MHz) to give a frequency of 48kHz 
  *              The interupt bit is set and the interrupt request (IRQ) is enabled 
  *              This was written using the STM32 Family Reference Manual Rev 11 Pages (183-187), (373-390) and (582-641) 
  * Arguments: void
  * Returns: void  */   
void samplingTimer_init() {
	
	if (config.ADC_samplingRate == 0){
		config.ADC_samplingRate = defaultSamplingRate;
	}
		
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
		 To get a 48kHz sampling rate: period = 84MHz / 48kHz + 1 = 1751
		 stored as a value - 1 so that it is easily re-configurable for different sample rates
	*/
	TIM_timeBaseStructure.TIM_Period = rint((float) (clocks.PCLK1_Frequency * 2) / (float)(config.ADC_samplingRate)) + 1; //1751 48kHz
	TIM_timeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_timeBaseStructure.TIM_RepetitionCounter = 0;
	// Initialise Timer 2
	TIM_TimeBaseInit(TIM2, &TIM_timeBaseStructure);
	// Start count on Timer 2
	TIM_Cmd(TIM2, ENABLE);
}

void samplingTimer_NVICInit(){
	// Generate Update Event
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* Initialise the timer interupt
	*/
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	// Set the priority of the interrupt to lower than the LPF timer as it takes longer to execute
	NVIC_SetPriority(TIM2_IRQn, 10);
}

/**
  * Name: TIM2_IRQHandler
  * Description: Interrupt handler for Timer 2: Defined in stm32f4xx.s
  * Arguments: void
  * Returns: void  */ 
void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		ADC_read = TM_ADC_Read(ADC1, ADC_Channel_0);
	}
}


