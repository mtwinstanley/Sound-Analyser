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

/* Custom pinout callback for USART */
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

void TM_USART1_ReceiveHandler(uint8_t c){
	char str[256];
	TM_USART_Putc(USART1, c);
	buffer[i] = c;
	i++;
	if (c == 0x7F){
		i--;
		buffer[i] = 0;
		i--;
		buffer[i] = 0;
	}
	if (c == 13){
		if (buffer[0] == 'R' && buffer[1] == 'T' && buffer[2] == 'C'){
			if (isdigit(buffer[4]) && isdigit(buffer[5]) && !isdigit(buffer[6]) && isdigit(buffer[22])){
				strncpy(RTC_time, (char *) buffer + 4, sizeof(buffer) - 4);
				TM_RTC_SetDateTimeString(RTC_time);
			}
		}
		//sprintf(str, "\n\rReceieved %s\n\r", buffer);
		/* Put to USART */
		//TM_USART_Puts(USART1, str);
		sprintf(str, "\n\rRTC %s\n\r", RTC_time);
		/* Put to USART */
		TM_USART_Puts(USART1, str);
		memset(buffer, 0, sizeof(buffer));
		i = 0;
	}
	
}
