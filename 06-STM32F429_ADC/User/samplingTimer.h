/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the sampling timer prototype functions
 *
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#ifndef _SAMPLINGTIMER_H
#define _SAMPLINGTIMER_H

#include <stdint.h>
#include <stdio.h>

/* Function Prototypes */
void samplingTimer_init(void);
void samplingTimer_NVICInit(void);

#endif /* _SAMPLINGTIMER_H */
