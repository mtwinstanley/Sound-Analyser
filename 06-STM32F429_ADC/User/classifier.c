#include "leds.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_usart.h"
#include <stdio.h>

void classifier_decisionTree(uint32_t SMatrix[]){
	char str[15];
	LED_resetALL();
	if (SMatrix[3] > 2 * SMatrix[5] && SMatrix[3] > SMatrix[4] && SMatrix[3] > SMatrix[2] && SMatrix[4] > SMatrix[5] && SMatrix[4] > SMatrix[1] && SMatrix[5] > SMatrix[1]){
		LED_setLED(LED_GREEN_1);
		sprintf(str, "BH\n\r");
	}
	else if (SMatrix[5] > 2* SMatrix[3] && SMatrix[3] > 5 * SMatrix[2] && SMatrix[6] > SMatrix[2]){
		LED_setLED(LED_GREEN_2);
		sprintf(str, "FL\n\r");
	}
	else if (SMatrix[5] >  4 * SMatrix[2] && SMatrix[2] > SMatrix[6] && SMatrix[9] > SMatrix[8] && SMatrix[3] > 4*SMatrix[2]){
		LED_setLED(LED_GREEN_3);
		sprintf(str, "JP\n\r");
	}
	else{
		sprintf(str, "ERR\n\r");
	}
	//TM_USART_Puts(USART1, str);
}
