/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the initialisation functions for the LPF clock
 *
 *  @file		codebook.c
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include <stdint.h>
#include "codebook.h"

/* Constant and statically declared codebook. Stored as a two dimensional array for easy look-up */
static const uint32_t codebook[6][33] = {	
											{1,2,3,4,5,6,6,7,7,7,9,9,9,11,11,11,11,11,14,14,14,14,14,18,18,18,18,18,18,18,23,23,23},
											{0,0,0,0,0,0,0,8,8,8,10,10,10,12,12,12,12,12,15,15,15,15,15,19,19,19,19,19,19,19,24,24,24},
											{0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,13,13,13,16,16,16,16,16,20,20,20,20,20,20,20,25,25,25},
											{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,17,17,17,17,21,21,21,21,21,21,21,26,26,26},
											{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,22,22,22,22,22,22,27,27,27},
											{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,28,28}
};

/**
  * Extract code from codebook
  * Name: codebook_getCode
  *
  * Description: Extract code from codebook
  *              This uses the S-D pair to look up the code from the standard codebook
  *
  * Arguments:  uint8_t shape - The number of minima extracted
  * 			uint8_t duration - The number of samples between zero crossings
  *
  * Returns:    uint8_t extracted code  
  */  	
uint8_t codebook_getCode(uint8_t shape, uint8_t duration){
  if(shape > 6 || duration > 33){
    return 0;
  }
  return codebook[shape][duration-1];
}
