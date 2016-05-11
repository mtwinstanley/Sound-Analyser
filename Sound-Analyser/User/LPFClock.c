/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the initialisation functions for the LPF clock
 *
 *  @file		LPFClock.c
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include <stdio.h>
#include <math.h>
#include "LPFClock.h"
#include "samplingTimer.h"
#include "config.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_usart.h"
#include "stm32f4xx.h"

#define defaultCutOffFrequency (config.ADC_samplingRate/2)

/* Clock Frequencies type declared in main.c */
extern RCC_ClocksTypeDef clocks;

/**
  * Initialise LPF Clock
  * Name: LPFClock_init
  *
  * Description: Initialises the timer
  *              This sets the prescaler to divide the APB1 Clock source (84MHz) to give a frequency related to the LPF cut off frequency 
  *              The timer is initialised to use PWM so that it will produce a clock source without requiring an interrupt or polling source
  *
  * Arguments: void
  *
  * Returns: void  
  */   
void LPFClock_init() {
  // Variable definititions
  TIM_TimeBaseInitTypeDef TIM_timeBase;
	
  // Load defaults if no cut off frequency is set
  if (!config.LPF_cutOffFrequency){
    config.LPF_cutOffFrequency = defaultCutOffFrequency;
    if (!config.LPF_cutOffFrequency){
      config.LPF_cutOffFrequency = 24000;
    }
  }
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);							// Enable Clock for Timer 3
  /* Timer 3 is connected to APB1, which runs at 42MHz
     Timer has internal PLL, which doubles this to 84MHz for use in the timer
     Prescaler is set to give he timer count frequency
     tick_frequency = default_frequency / (prescaler + 1)
     In this case: tick_frequency = 84MHz / (0+1) = 84MHz
  */
  TIM_timeBase.TIM_Prescaler = 0;		
  // Count Up
  TIM_timeBase.TIM_CounterMode = TIM_CounterMode_Up;
	
  /* Set timer period for when to reset
     The cut off frequency of the filter is the incoming clock / 32
     This clock source is driven by a toggled GPIO pin which changes every period
     In order to achieve the desired frequency, the timer tick must be set to twice the frequency of the required filter.
		 
     clock frequency = cut off frequency * 32
     period = tick_frequency / clock frequency +1
     For example: frequency = 23kHz * 32 * 2 = 736kHz
     period = 84MHz / 736kHz + 1 = 114 (114.1) + 1
  */
  TIM_timeBase.TIM_Period = rint((float) (clocks.PCLK1_Frequency * 2) / (float)(config.LPF_cutOffFrequency * 32)) + 1;
  TIM_timeBase.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_timeBase.TIM_RepetitionCounter = 0;
  // Initialise Timer 3
  TIM_TimeBaseInit(TIM3, &TIM_timeBase);
  // Start count on Timer 3
  TIM_Cmd(TIM3, ENABLE);
	
  // Initialise the PWM
  LPFClock_PWMInit(TIM_timeBase.TIM_Period);
  // Initialise the GPIO pins for the clock output
  LPFClock_GPIOInit();
}

/**
  * Initialise the Pulse Width Modulation
  * Name: LPFClock_PWMInit
  *
  * Description: Initialises the PWM for the timer
  *              This sets the pulse length of the PWM pin in order to create the desired clock 
  *              This was written with help from Majerle Tilen with his tutorials at stm32f4-discovery.com
  *
  * Arguments:   uint32_t period 
  * 
  * Returns:     void  
  */ 
void LPFClock_PWMInit(uint32_t period){
  TIM_OCInitTypeDef TIM_OCStruct;
	
  TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	
  /* Duty Cycle can be determined by a simple equation
     pulse_length = ((TIM_Period +1) * DutyCycle) / 100 - 1
  */
	
  TIM_OCStruct.TIM_Pulse = ((period + 1) * 50) / 100 - 1;
  TIM_OC1Init(TIM3, &TIM_OCStruct);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

/**
  * Initialise output pin
  * Name: LPFClock_GPIOInit
  *
  * Description: Initialises the GPIO Pins for the timer outputs
  *              This initialises all pins required to output the clock data
  *              The pins must be initialised with the timers output channels PC6 is Timer 3's Output Channel 1
  *
  * Arguments:   void 
  *
  * Returns:     void  
  */ 
void LPFClock_GPIOInit(){
  GPIO_InitTypeDef GPIO_InitDef;
	
  /* Initialise the GPIO pin PC6
     This will toggle on each timer tick to generate a clock signal to send to the LPF
  */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
  // Set as Alternating Functions
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	
  GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	
  // Initialise Pin
  GPIO_Init(GPIOC, &GPIO_InitDef);
}
