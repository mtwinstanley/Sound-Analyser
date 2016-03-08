#ifndef _LEDS_H
#define _LEDS_H

#define LED_GREEN_1 GPIO_Pin_0
#define LED_GREEN_2 GPIO_Pin_1
#define LED_GREEN_3 GPIO_Pin_2
#define LED_GREEN_4 GPIO_Pin_3
#define LED_GREEN_5 GPIO_Pin_4
#define LED_GREEN_6 GPIO_Pin_5
#define LED_GREEN_7 GPIO_Pin_6
#define LED_GREEN_8 GPIO_Pin_7
#define LED_RED GPIO_Pin_8

#include <stdint.h>

/* Function Prototypes */
void LED_GPIOInit(void);
void LED_setLED(uint16_t led);
void LED_toggleLED(uint16_t led);
void LED_resetLED(uint16_t led);
void LED_resetALL(void );
	
#endif /* _LEDS_H */
