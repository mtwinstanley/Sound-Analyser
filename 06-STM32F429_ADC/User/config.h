/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the configuration structure data
 *
 *  @file		config.h
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

/* Configuration data structure */

typedef struct {
  uint32_t ADC_samplingRate;
  uint32_t LPF_cutOffFrequency;
  uint32_t classificationTime;
}config_type;

extern config_type config;

#endif /* _CONFIG_H */
