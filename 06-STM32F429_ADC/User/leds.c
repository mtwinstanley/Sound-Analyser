
#include "tm_stm32f4_gpio.h"
#include "leds.h"
/**
  * Name: LPFClock_GPIOInit
  * Description: Initialises the GPIO Pins for the timer outputs
  *              This initialises all pins required to output the clock data
  *              The pins must be initialised with the timers output channels PC6 is Timer 3's Output Channel 1
  * Arguments: void 
  * Returns: void  */ 
void LED_GPIOInit(){
	GPIO_InitTypeDef GPIO_InitDef;
	
	/* Initialise the GPIO pin PE0
		 This will toggle on each timer tick to generate a clock signal to send to the LPF
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_7 | GPIO_PIN_8;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitDef);
	
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitDef);
}

void LED_setLED(uint16_t led){
	GPIO_SetBits(GPIOB, led);
	GPIO_SetBits(GPIOE, led);
}

void LED_error(){
	GPIO_SetBits(GPIOB, LED_RED);
}

void LED_clearError(){
	GPIO_SetBits(GPIOB, LED_RED);
}

void LED_toggleLED(uint16_t led){
	GPIO_ToggleBits(GPIOB, led);
	GPIO_ToggleBits(GPIOE, led);
}

void LED_resetLED(uint16_t led){
	GPIO_ResetBits(GPIOB, led);
	GPIO_ResetBits(GPIOE, led);
}

void LED_resetALL(){
	GPIO_ResetBits(GPIOB, LED_GREEN_8 | LED_RED);
	GPIO_ResetBits(GPIOE, LED_GREEN_1 | LED_GREEN_2 | LED_GREEN_3 | LED_GREEN_4 | LED_GREEN_5 | LED_GREEN_6 | LED_GREEN_7);
}
