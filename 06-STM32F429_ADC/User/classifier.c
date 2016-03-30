#include "leds.h"
#include "stm32f4xx_gpio.h"

void classifier_decisionTree(uint32_t SMatrix[]){
	if (SMatrix[4] > SMatrix[5] && SMatrix[4] > SMatrix[1]){
		if (SMatrix[5] > SMatrix[1]){
			LED_setLED(LED_GREEN_1);
		}
	}
	else if (SMatrix[2] > SMatrix[1] && SMatrix[3] > SMatrix[1] && SMatrix[16] > SMatrix[15]){
		LED_setLED(LED_GREEN_2);
	}
	else{
		LED_resetALL();
	}
		
}