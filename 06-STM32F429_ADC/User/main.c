/**
 *	Keil project for ADC peripheral
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
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

config_type config;
RCC_ClocksTypeDef clocks;
uint16_t ADC_read, ADC_previousRead;

/* RTC */
TM_RTC_Time_t datatime1;

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
        //RTC was first time initialized
        //Do your stuf here
        //eg. set default time
				
    }

	/* Option to set the LPF cut off frequency and the ADC_sampling Rate. This should be changed to read from the SD Card */
	//LPF_cutOffFrequency = 5000;
	//ADC_samplingRate = 48000;
	//config.classificationTime = 30*48000;
	
	if ((((FATFS_USE_DETECT_PIN_PORT)->IDR & (FATFS_USE_DETECT_PIN_PIN)) == 0 ? 0 : 1) == 0) {
		SDCard_readConfig();
		SDCard_extractConfig();
		SDCardInserted = 1;
	}
	else {
		sprintf(str, "NO SD CARD INSERTED\n\r");
		TM_USART_Puts(USART1, str);
		LED_error();
	}

	config.classificationTime = 480000;
	
	LED_GPIOInit();
	TDSC_init();
	LPFClock_init();
	
	//SDCard_writeData(SMatrix_type, getSMatrix());
	
	sprintf(str, "START @%d\n\r", SystemCoreClock);
	/* Put to USART */
	TM_USART_Puts(USART1, str);
	
	while (1) {
		
		if (ADC_read != ADC_previousRead){
			ADC_previousRead = ADC_read;
			TDSC_sampleRoutine(ADC_read);
		}
//		else if (SDRead){
//			SDCard_readConfig();
//			SDCard_extractConfig();
//			SDRead = 0;
//		}
	}
}
