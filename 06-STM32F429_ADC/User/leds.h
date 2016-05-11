/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the function prototypes for the LPF clock
 *
 *  @file		leds.h
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#ifndef _LEDS_H
#define _LEDS_H

/* Global definition of the LED pins */
#define LED_GREEN_1 GPIO_Pin_0
#define LED_GREEN_2 GPIO_Pin_1
#define LED_GREEN_3 GPIO_Pin_2
#define LED_GREEN_4 GPIO_Pin_3
#define LED_GREEN_5 GPIO_Pin_4
#define LED_GREEN_6 GPIO_Pin_5
#define LED_GREEN_7 GPIO_Pin_6
#define LED_GREEN_8 GPIO_Pin_7
#define LED_RED GPIO_Pin_8

#include <stdint.h>

/* Function Prototypes */
void LED_GPIOInit(void);
void LED_setLED(uint16_t led);
void LED_error(void);
void LED_clearError(void);
void LED_toggleLED(uint16_t led);
void LED_resetLED(uint16_t led);
void LED_resetALL(void );
	
#endif /* _LEDS_H */
