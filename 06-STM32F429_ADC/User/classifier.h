/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the function prototypes for the decision tree classifier
 *
 *  @file		classifier.h
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#ifndef _CLASSIFIER_H
#define _CLASSIFIER_H

#include <stdint.h>

/* Function Prototypes */
const char * classifier_decisionTree(uint32_t SMatrix[]);
	
#endif /* _CLASSIFIER_H */
