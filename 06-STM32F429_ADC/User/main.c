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
	uint16_t high = 0, low = 0xFFFF;
	
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
	
	TM_GPIO_Init(FATFS_USE_DETECT_PIN_PORT, FATFS_USE_DETECT_PIN_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
	
	if (!TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
        //RTC was first time initialized
        //Do your stuf here
        //eg. set default time
    }
	
	sprintf(str, "****** START *****\n\r");
	/* Put to USART */
	TM_USART_Puts(USART1, str);
	/* Initialize ADC1 on channel 3, this is pin PA3 */
	//TM_ADC_Init(ADC1, ADC_Channel_3);

	/* Option to set the LPF cut off frequency and the ADC_sampling Rate. This should be changed to read from the SD Card */
	//LPF_cutOffFrequency = 5000;
	//ADC_samplingRate = 48000;
	//config.classificationTime = 30*48000;
	if ((((FATFS_USE_DETECT_PIN_PORT)->IDR & (FATFS_USE_DETECT_PIN_PIN)) == 0 ? 0 : 1) == 0) {
		SDCard_readConfig();
		SDCard_extractConfig();
	}
	else {
		sprintf(str, "NO SD CARD INSERTED\n\r");
		TM_USART_Puts(USART1, str);
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
			if (ADC_read > high){
				high = ADC_read;
				sprintf(str, "high = %4d\n\r", ADC_read);
				//TM_USART_Puts(USART1, str);
			}
			if (ADC_read < low){
				low = ADC_read;
				sprintf(str, "low = %4d\n\r", ADC_read);
				//TM_USART_Puts(USART1, str);
			}
			//LED_toggleLED(LED_GREEN_8);
		if (RTC_update){
			TM_RTC_SetDateTimeString(RTC_time);
			RTC_update = 0;
		}
		}
//		TM_RTC_GetDateTime(&datatime1, TM_RTC_Format_BIN);
//			sprintf(str, "%02d.%02d.%04d %02d:%02d:%02d  Unix: %u\n\r",
//                datatime1.date,
//                datatime1.month,
//                datatime1.year + 2000,
//                datatime1.hours,
//                datatime1.minutes,
//                datatime1.seconds,
//                datatime1.unix
//			);
//			TM_USART_Puts(USART1, str);
		
		/* 							Read ADC1 Channel0					Read ADC1 Channel3 */
		//sprintf(str, "%4d\n\r", TM_ADC_Read(ADC1, ADC_Channel_0));//, TM_ADC_Read(ADC1, ADC_Channel_3));
		//TM_USART_Puts(USART1, str);
//		if (TDSC_crossings.length > 200){
//			int i;
//			for (i = 0; i < TDSC_crossings.length; i++){
//				sprintf(str, "%4d", TDSC_crossings.collection[i]);
//				TM_USART_Puts(USART1, str);
//			}
//				sprintf(str, "\n\r");
//				TM_USART_Puts(USART1, str);
//		}
		
//		read = TM_ADC_Read(ADC1, ADC_Channel_0);
//		if (low > read && read != 0){
//			low = read;
//			sprintf(str, "low = %4d\n\r", low);
//			/* Put to USART */
//			TM_USART_Puts(USART1, str);
//		}
//		if (read > high){
//			high = read;
//			sprintf(str, "high = %4d\n\r", high);
//			/* Put to USART */
//			TM_USART_Puts(USART1, str);
//		}


		
		/* Little delay */
//		Delayms(100);
	}
}
