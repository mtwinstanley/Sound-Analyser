/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the receive handler for the connected USART
 *
 *  @file		usart.c
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "usart.h"
#include "misc.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_rtc.h"

uint8_t buffer[512], RTC_update;
uint32_t i;

char RTC_time[19];

/**
  * Custom pinout callback for USART
  * Name: TM_USART_InitCustomPinsCallback
  *
  * Description: Sets the GPIO pins for the USART communication
  *              Extends from the library used to control USART functions 
  *              Adapted to use the correct pins
  *
  * Arguments: USART_TypeDef* USARTx - USART1
  *            uint16_t AlternateFunction
  *
  * Returns: void  
  */  
void TM_USART_InitCustomPinsCallback(USART_TypeDef* USARTx, uint16_t AlternateFunction) {
  /* Check for proper USART */
  if (USARTx == USART1) {
    /* Initialize custom pins */
    /* TX pin = PB6 */
    TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low, AlternateFunction);
    /* RX pin = PA10 */
    TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low, AlternateFunction);
  }
}

/**
  * Receive Handler for USART1
  * Name: TM_USART1_ReceiveHandler
  *
  * Description: Gives functionality when a character is received over the USART serial communication channel
  *              Stores data received into a buffer for use in other functions 
  *              Control available for use of the backspace key, functions called when enter key pressed (carriage return)
  *              Used to set the RTC time
  *
  * Arguments: uint8_t c - character received
  *
  * Returns: void  
  */  
void TM_USART1_ReceiveHandler(uint8_t c){
  char str[256];
  TM_USART_Putc(USART1, c);
  buffer[i] = c;
  i++;
  /* Backspace */
  if (c == 0x7F){
    i--;
    buffer[i] = 0;
    i--;
    buffer[i] = 0;
  }
  /* Carriage Return */
  if (c == 13){
    if (buffer[0] == 'R' && buffer[1] == 'T' && buffer[2] == 'C'){
    if (isdigit(buffer[4]) && isdigit(buffer[5]) && !isdigit(buffer[6]) && isdigit(buffer[22])){
      strncpy(RTC_time, (char *) buffer + 4, sizeof(buffer) - 4);
	  /* Set RTC time */
      TM_RTC_SetDateTimeString(RTC_time);
    }
  }
  sprintf(str, "\n\rRTC %s\n\r", RTC_time);
  /* Put to USART */
  TM_USART_Puts(USART1, str);
  memset(buffer, 0, sizeof(buffer));
  i = 0;
  }
}
