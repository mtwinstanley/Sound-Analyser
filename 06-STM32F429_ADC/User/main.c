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
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_adc.h"
#include "samplingTimer.h"
#include "LPFClock.h"
#include <stdio.h>

RCC_ClocksTypeDef clocks;
int main(void) {
	char str[15];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize Delay library */
	TM_DELAY_Init();
	
	/* Get the Clock Frequencies for use in the timer initialisations */
	RCC_GetClocksFreq(&clocks);
	
	/* Initialize USART1, 115200baud, TX: PB6 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize ADC1 on channel 0, this is pin PA0 */
	TM_ADC_Init(ADC1, ADC_Channel_0);
	
	/* Initialize ADC1 on channel 3, this is pin PA3 */
	//TM_ADC_Init(ADC1, ADC_Channel_3);

	/* Option to set the LPF cut off frequency and the ADC_sampling Rate. This should be changed to read from the SD Card */
	//LPF_cutOffFrequency = 10000;
	//ADC_samplingRate = 48000;
	
	TDSC_init();
	samplingTimer_init();
	LPFClock_init();
	sprintf(str, "START @%d\n\r", SystemCoreClock);
			/* Put to USART */
			TM_USART_Puts(USART1, str);
			
			//low = TM_ADC_Read(ADC1, ADC_Channel_0);
	while (1) {
		/* 							Read ADC1 Channel0					Read ADC1 Channel3 */
		//sprintf(str, "%4d\n\r", TM_ADC_Read(ADC1, ADC_Channel_0));//, TM_ADC_Read(ADC1, ADC_Channel_3));
		//TM_USART_Puts(USART1, str);
		if (TDSC_crossings.length > 200){
			int i;
			for (i = 0; i < TDSC_crossings.length; i++){
				sprintf(str, "%4d", TDSC_crossings.collection[i]);
				TM_USART_Puts(USART1, str);
			}
				sprintf(str, "\n\r");
				TM_USART_Puts(USART1, str);
		}
		
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
		Delayms(100);
	}
}
