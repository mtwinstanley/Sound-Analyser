/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the main processing loop
 *
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "config.h"
#include "SDCard.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_rtc.h"
#include "leds.h"
#include "LPFClock.h"
#include "TDSC.h"
#include "samplingTimer.h"
#include "usart.h"
#include <stdio.h>

/* Clock Frequencies type*/
RCC_ClocksTypeDef clocks;

/* ADC values */
uint16_t ADC_read, ADC_previousRead;

/* RTC */
TM_RTC_Time_t datatime1;

/** 
	*	Main function 
	*	Name: main
	*	
	* Description: 	Initialises all peripherals and reads configuration data
	* 							In the main loop, the ADC values are processed
	*
	*	Arguments: 		void
	*
	*	Returns: 			int - will only return a value if the main processing loop exits
	*/
int main(void) {
	char str[20];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize Delay library */
	TM_DELAY_Init();
	
	/* Get the Clock Frequencies for use in the timer initialisations */
	RCC_GetClocksFreq(&clocks);
	
	/* Initialize USART1, 115200baud, TX: PB6 */
	TM_USART_Init(USART1, TM_USART_PinsPack_Custom, 115200);
	
	/* Initialize ADC1 on channel 0, this is pin PA0 */
	TM_ADC_Init(ADC1, ADC_Channel_0);
	
	/* Initialise SD Card Detect Pin */
	TM_GPIO_Init(FATFS_USE_DETECT_PIN_PORT, FATFS_USE_DETECT_PIN_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
	SDCard_configureCD();
	
	/* Initialise RTC if it is not already initialised */
	if (!TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
        // If RTC has not been initialised, these functions are called. 
				// Gives the option to set the time and date to a default value.			
    }

	/* Detect if an SD card is inserted and read configuration data from the SD card */
	if ((((FATFS_USE_DETECT_PIN_PORT)->IDR & (FATFS_USE_DETECT_PIN_PIN)) == 0 ? 0 : 1) == 0) {
		SDCard_readConfig();
		SDCard_extractConfig();
		SDCardInserted = 1;
	}
	/* If no SD card is inserted, alert the user through USART and by lighting an LED */
	else {
		sprintf(str, "NO SD CARD INSERTED\n\r");
		TM_USART_Puts(USART1, str);
		LED_error();
	}
	
	/* Initialisation Functions */ 
	LED_GPIOInit();
	TDSC_init();
	LPFClock_init();
	
	/* Print start message */
	sprintf(str, "START @%d\n\r", SystemCoreClock);
	TM_USART_Puts(USART1, str);
	
	while (1) {
		
		/* Detect changes in the ADC value and process data */
		if (ADC_read != ADC_previousRead){
			ADC_previousRead = ADC_read;
			TDSC_sampleRoutine(ADC_read);
		}
	}
}
