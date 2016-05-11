/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the decision tree classification algorithm
 *
 *  @file		classifier.c
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include "leds.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_usart.h"
#include "classifier.h"
#include <stdio.h>

/**
  * Classify incoming data
  * Name: classifier_decisionTree
  *
  * Description: Decision tree classification algorithm
  *              The input data is given a series of statements to answer either yes or no 
  *              If the algorithm detects a species, set the corresponding LED and return the data as a string
  *
  * Arguments: uint32_t SMatrix - array from the TDSC feature extraction
  *
  * Returns: const char * A pointer to a character array  
  */ 
const char * classifier_decisionTree(uint32_t SMatrix[]){
  char str[15];
  const char * c;
  LED_resetALL();
  if (SMatrix[3] > SMatrix[5] && SMatrix[3] > SMatrix[4] && SMatrix[3] > SMatrix[2] && SMatrix[4] > SMatrix[5] && SMatrix[4] > SMatrix[1] && SMatrix[5] > SMatrix[1]){
    LED_setLED(LED_GREEN_1);
    sprintf(str, "BH\n\r");
    c = "BH    ";
  }
  else if (SMatrix[5] > SMatrix[3] && SMatrix[3] > 4 * SMatrix[2] && SMatrix[6] > SMatrix[2]){
    LED_setLED(LED_GREEN_2);
    sprintf(str, "FL\n\r");
    c = "FL    ";
  }
  else if (SMatrix[5] >  3 * SMatrix[2] && SMatrix[2] > SMatrix[6] && SMatrix[3] > 2*SMatrix[2]){
    LED_setLED(LED_GREEN_3);
    sprintf(str, "JP\n\r");
    c = "JP    ";
  }
  else{
    sprintf(str, "ERR\n\r");
    c = "ER    ";
  }
  TM_USART_Puts(USART1, str);
  return c;
}
