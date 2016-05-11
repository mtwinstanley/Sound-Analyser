/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the Time Domain Signal Coding (TDSC) Algorithm
 *
 *  @file		TDSC.c
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
#include <stdio.h>
#include <string.h>
#include "TDSC.h"
#include "samplingTimer.h"
#include "codebook.h"
#include "config.h"
#include "tm_stm32f4_usart.h"
#include "leds.h"
#include "SDCard.h"
#include "classifier.h"

/* Variable Structures */
ADC_values_t ADC_values;
TDSC_crossings_t TDSC_crossings;

/* Time since last classification */
uint32_t time = 0;

/* TDSC Matrices */
uint32_t SMatrix[codebookSize] = {0};
uint32_t AMatrix[codebookSize * codebookSize] = {0};

/**
  * Initialise variables and call sampling timer initialisation functions
  * Name: TDSC_init
  *
  * Description: Initialises ADC and TDSC values to zero
  *              Calls the functions to initialise the sampling timer 
  *
  * Arguments: void 
  * 
  * Returns:   void  
  */ 
void TDSC_init(){
  ADC_values.prev = ADC_values.current = ADC_values.next = 0;
  TDSC_crossings.shape = TDSC_crossings.positive = TDSC_crossings.lagCode = 0;
  samplingTimer_init();
  samplingTimer_NVICInit();
}


/**
  * Main processing for TDSC algorithm
  * Name: TDSC_sampleRoutine
  *
  * Description: Increments duration and detects for minima or zero crossings each sample time
  *              Populates the S and A Matrices with using values extracted from the codebook to create histograms
  *              Calls classification function and SD card write function
  *
  * Arguments: uint16_t read - ADC value 
  * 
  * Returns:   void  
  */ 
void TDSC_sampleRoutine(uint16_t read){
  uint8_t code;
  const char * classificationValue;
	
  TDSC_adjustValues(read);
  /* Increment time variables */
  TDSC_crossings.duration++;
  time ++;
  /* Positive zero crossing occurred */
  if (read > 2300 && TDSC_crossings.positive == 0){
    TDSC_crossings.positive = 1;
	TDSC_processSDpair();
  }
  /* Negative zero crossing occurred */
  else if (read < 2200 && TDSC_crossings.positive == 1){
    TDSC_crossings.positive = 0;
    TDSC_processSDpair();
  }
  /* Check for positive minima */
  else if (TDSC_crossings.positive && TDSC_positiveMinima()){
    TDSC_crossings.shape++;
  }
  /* Check for negative minima */
  else if (!TDSC_crossings.positive && TDSC_negativeMinima()){
    TDSC_crossings.shape++;
  }
  /* Run classification algorithm */
  if (time == config.classificationTime){
    time = 0;
	/* Pass S Matrix to classifier */
    classificationValue = classifier_decisionTree(SMatrix);
	/* Write data to SD card */
    SDCard_writeData(SMatrix_type, SMatrix, classificationValue);
	/* Reset S Matrix */
    memset(SMatrix, 0, codebookSize * sizeof(SMatrix[0]));	
  }
}

/**
  * Shifts values within ADC_values structure
  * Name: TDSC_adjustValues
  *
  * Description: Moves sample data to next value
  *              Used to detect minima i.e. the shape
  *
  * Arguments: uint16_t adcRead - ADC value 
  * 
  * Returns:   void  
  */ 
void TDSC_adjustValues(uint16_t adcRead){
	ADC_values.prev = ADC_values.current;
	ADC_values.current = ADC_values.next;
	ADC_values.next = adcRead;
}

/**
  * Processes the duration and shape values 
  * Name: TDSC_processSDpair
  *
  * Description: Extracts the code value from the codebook
  *              S and A matrices adjusted
  *
  * Arguments: void 
  * 
  * Returns:   void  
  */ 
void TDSC_processSDpair(){
  /* Extract code */
  code = codebook_getCode(TDSC_crossings.shape, TDSC_crossings.duration);
  /* S and A Matrices populated*/
  TDSC_setSMatrixValue(code);
  TDSC_setAMatrixValue(code, TDSC_crossings.lagCode);
  TDSC_crossings.lagCode = code;
  /* Shape and Duration values reset */
  TDSC_crossings.shape = 0;
  TDSC_crossings.duration = 0;
}

/**
  * Detect positive Minima 
  * Name: TDSC_positiveMinima
  *
  * Description: Calculates if the previous sample caused a minima in the positive signal
  *
  * Arguments: void 
  * 
  * Returns:   uint8_t - boolean result if minima is detected  
  */ 
uint8_t TDSC_positiveMinima(){
	if (ADC_values.current < ADC_values.next && ADC_values.current < ADC_values.prev){
		return 1;
	}
	else{
		return 0;
	}
}

/**
  * Detect negative Minima 
  * Name: TDSC_negativeMinima
  *
  * Description: Calculates if the previous sample caused a minima in the negative signal
  *              Equivalent to a positive maxima
  *
  * Arguments: void 
  * 
  * Returns:   uint8_t - boolean result if minima is detected 
  */
uint8_t TDSC_negativeMinima(){
	if (ADC_values.current > ADC_values.next && ADC_values.current > ADC_values.prev){
		return 1;
	}
	else{
		return 0;
	}
}

/**
  * Increments the S Matrix value by one
  * Name: TDSC_setSMatrixValue
  *
  * Description: Uses the code value to increment the S Matrix, adding one to the element of the histogram
  *
  * Arguments: uint8_t code - code from codebook
  * 
  * Returns:   void 
  */
void TDSC_setSMatrixValue(uint8_t code){
	if (code == 0){
		return;
	}
	SMatrix[code - 1] = SMatrix[code-1] + 1; 
}
/**
  * Returns S Matrix
  * Name: getSMatrix
  *
  * Description: Returns pointer to S Matrix 
  *
  * Arguments: void
  * 
  * Returns:   uint32_t* - pointer to S Matrix  
  */
uint32_t * getSMatrix(){
	return SMatrix;
}

/**
  * Increments the A Matrix value by one
  * Name: TDSC_setSMatrixValue
  *
  * Description: Uses the code value to increment the A Matrix, adding one to the element of the histogram
  *              Uses the lag code as the secondary element
  *
  * Arguments: uint8_t code - code from codebook
  *            uint8_t lag - code from codebook
  * 
  * Returns:   void 
  */
void TDSC_setAMatrixValue(uint8_t code, uint8_t lag){
	if (code == 0 || lag == 0){
		return;
	}
	AMatrix[((code-1) * codebookSize) + (lag-1)] = AMatrix[((code-1) * codebookSize) + (lag-1)] + 1;
}

/**
  * Returns A Matrix
  * Name: getAMatrix
  *
  * Description: Returns pointer to A Matrix 
  *
  * Arguments: void
  * 
  * Returns:   uint32_t* - pointer to A Matrix  
  */
uint32_t * getAMatrix(){
	return AMatrix;
}
