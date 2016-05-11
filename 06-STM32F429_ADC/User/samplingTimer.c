/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the sampling timer initialisations and interrupt handlers for the ADC
 *
 *  @file		samplingTimer.c
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include <math.h>
#include "samplingTimer.h"
#include "config.h"
#include "stm32f4xx_tim.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_usart.h"
#include "stm32f4xx.h"

#define defaultSamplingRate 48000

/* Variable structure definitions */
TIM_TimeBaseInitTypeDef TIM_timeBaseStructure;
NVIC_InitTypeDef nvicStructure;

/* Extern Variables from main */
extern RCC_ClocksTypeDef clocks; 
extern uint32_t ADC_read;

/**
  * Initialise the sampling timer
  * Name: samplingTimer_init
  *
  * Description: 	Initialises the timer
  *              	This sets the prescaler to divide the APB1 Clock source (84MHz) to give a frequency of 48kHz (or the sampling rate specified in the cofiguration file) 
  *
  * Arguments:      void
  *
  * Returns:        void  
  */   
void samplingTimer_init() {
  /* Set default sampling rate if nothing has been set by the SD Card */
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

/**
  * Initialise the Interupt for the sampling timer
  * Name: samplingTimer_NVICInit
  *
  * Description: 	Initialises the timer interrupt
  *
  * Arguments: 		void
  *
  * Returns:        void  
  */  
void samplingTimer_NVICInit(){
  // Generate Update Event
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
  /* Initialise the timer interrupt */
  nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
  nvicStructure.NVIC_IRQChannelSubPriority = 0;
  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructure);
  // Set the priority of the interrupt to lower than the LPF timer as it takes longer to execute
  NVIC_SetPriority(TIM2_IRQn, 10);
}

/**
  * Interrupt handler for timer 2
  * Name: TIM2_IRQHandler
  * 
  * Description: Interrupt handler for Timer 2: Defined in stm32f4xx.s
  *
  * Arguments:   void
  *
  * Returns:     void 
  */ 
void TIM2_IRQHandler(void) {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    // Clear Flag
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    // Read values from the ADC
    ADC_read = TM_ADC_Read(ADC1, ADC_Channel_0);
  }
}


