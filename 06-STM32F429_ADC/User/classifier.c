#include "leds.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_usart.h"
#include "classifier.h"
#include <stdio.h>

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
