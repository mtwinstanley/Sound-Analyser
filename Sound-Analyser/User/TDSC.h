/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the function prototypes for the TDSC algorithm
 *
 *  @file		TDSC.h
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#ifndef _TDSC_H
#define _TDSC_H

#include <stdint.h>
#include <stdio.h>

/* Structure of ADC values used to detect minima */
typedef struct {
  uint16_t prev;
  uint16_t current;
  uint16_t next;
}ADC_values_t;

/* Structure of TDSC values used to store data extracted */
typedef struct {
  uint8_t shape;
  uint8_t positive;
  uint8_t duration;
  uint8_t lagCode;
}TDSC_crossings_t;

extern ADC_values_t ADC_values;
extern TDSC_crossings_t TDSC_crossings;

/* Function Prototypes */
void TDSC_adjustValues(uint16_t adcRead);
void TDSC_sampleRoutine(uint16_t read);
void TDSC_init(void);
uint8_t TDSC_positiveMinima(void);
uint8_t TDSC_negativeMinima(void);

void TDSC_processSDpair(void);
void TDSC_setSMatrixValue(uint8_t code);
void TDSC_setAMatrixValue(uint8_t code, uint8_t lag);
uint32_t * getSMatrix(void);
uint32_t * getAMatrix(void);

#endif /* _TDSC_H */
