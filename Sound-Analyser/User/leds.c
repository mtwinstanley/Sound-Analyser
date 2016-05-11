/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the initialisation functions for the LEDs and their operational functions
 *
 *  @file		leds.c
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include "tm_stm32f4_gpio.h"
#include "leds.h"

/**
  * Initialise the LEDs
  * Name: LED_GPIOInit
  *
  * Description: Initialises the GPIO pins connected to the LEDs
  *              This sets the pins to output and initialises the required ports
  *
  * Arguments: void
  *
  * Returns: void  
  */ 
void LED_GPIOInit(){
  GPIO_InitTypeDef GPIO_InitDef;
	
  /* Initialise the GPIO pins PE0, PE1, PE2, PE3, PE4, PE5, PE6, PB7 and PB8 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
  GPIO_InitDef.GPIO_Pin = GPIO_Pin_7 | GPIO_PIN_8;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOB, &GPIO_InitDef);
	
  GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOE, &GPIO_InitDef);
}

/**
  * Set the LEDs
  * Name: LED_setLED
  *
  * Description: Sets the GPIO pins high
  *
  * Arguments: uint16_t - a binary value to set the correct pins high
  *
  * Returns: void  
  */ 
void LED_setLED(uint16_t led){
  GPIO_SetBits(GPIOB, led);
  GPIO_SetBits(GPIOE, led);
}

/**
  * Show the Error LED
  * Name: LED_error
  *
  * Description: Sets the RED LED high
  *
  * Arguments: void
  *
  * Returns: void  
  */ 
void LED_error(){
  GPIO_SetBits(GPIOB, LED_RED);
}

/**
  * Clear the Error LED
  * Name: LED_error
  *
  * Description: Resets the RED LED
  *
  * Arguments: void
  *
  * Returns: void  
  */
void LED_clearError(){
  GPIO_SetBits(GPIOB, LED_RED);
}

/**
  * Toggles the output of the LEDs
  * Name: LED_toggleLED
  *
  * Description: Alternates the polarity of the LED status. 
  *
  * Arguments: uint16_t - a binary value to set the correct pins high
  *
  * Returns: void  
  */ 
void LED_toggleLED(uint16_t led){
  GPIO_ToggleBits(GPIOB, led);
  GPIO_ToggleBits(GPIOE, led);
}

/**
  * Reset the LEDs
  * Name: LED_resetLED
  *
  * Description: Resets the GPIO pins
  *
  * Arguments: uint16_t - a binary value to set the correct pins low
  *
  * Returns: void  
  */  
void LED_resetLED(uint16_t led){
  GPIO_ResetBits(GPIOB, led);
  GPIO_ResetBits(GPIOE, led);
}

/**
  * Clear all LEDs
  * Name: LED_resetALL
  *
  * Description: Resets all the LEDs
  *
  * Arguments: void
  *
  * Returns: void  
  */
void LED_resetALL(){
  GPIO_ResetBits(GPIOB, LED_GREEN_8 | LED_RED);
  GPIO_ResetBits(GPIOE, LED_GREEN_1 | LED_GREEN_2 | LED_GREEN_3 | LED_GREEN_4 | LED_GREEN_5 | LED_GREEN_6 | LED_GREEN_7);
}
