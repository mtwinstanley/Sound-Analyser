/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the function prototypes for the LPF clock
 *
 *  @file		codebook.h
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#ifndef _CODEBOOK_H
#define _CODEBOOK_H

#include <stdint.h>

/* Global Definitions */
#define codebookSize 28

/* Function Prototypes */
uint8_t codebook_getCode(uint8_t shape, uint8_t duration);

#endif /* _CODEBOOK_H */
