/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the function prototypes for the LPF clock
 *
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#ifndef _LPFCLOCK_H
#define _LPFCLOCK_H

#include <stdint.h>
#include <stdio.h>

/* Function Prototypes */
void LPFClock_init(void);
void LPFClock_PWMInit(uint32_t period);
void LPFClock_GPIOInit(void);
	
#endif /* _LPFCLOCK_H */
