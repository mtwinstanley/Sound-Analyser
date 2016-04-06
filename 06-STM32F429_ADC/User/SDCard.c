#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "SDCard.h"
#include "codebook.h"
#include "config.h"
#include "leds.h"
#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_rtc.h"
#include <string.h>

/* FATFS related */
FATFS fatFs;
FIL file, fil;
FRESULT fres;
uint8_t SD_Buffer[512];
uint8_t SDCardInserted;//, SDRead;

/* RTC */
TM_RTC_Time_t datatime;

void SDCard_configureCD(){
	EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Enable clock for SYSCFG */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Tell system that you will use PC7 for EXTI_Line0 */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);

	/* PC7 is connected to EXTI_Line7 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line7;
	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PC7 is connected to EXTI_Line7, which has EXTI9_5_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

}

/* Handle PC7 interrupt */
void EXTI9_5_IRQHandler(void) {
		char str[20];
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
        /* Do your stuff when PB12 is changed */
				if ((((FATFS_USE_DETECT_PIN_PORT)->IDR & (FATFS_USE_DETECT_PIN_PIN)) == 0 ? 0 : 1) != 0 && SDCardInserted == 1) {
					sprintf(str, "NO SD CARD INSERTED\n\r");
					TM_USART_Puts(USART1, str);
					SDCardInserted = 0;
					LED_error();
				}
				if ((((FATFS_USE_DETECT_PIN_PORT)->IDR & (FATFS_USE_DETECT_PIN_PIN)) == 0 ? 0 : 1) == 0 && SDCardInserted == 0) {
					//sprintf(str, "SD CARD INSERTED\n\r");
					//TM_USART_Puts(USART1, str);
//					SDCard_readConfig();
//					SDCard_extractConfig();
					//SDRead = 1;
					SDCardInserted = 1;
					sprintf(str, "SD CARD INSERTED\n\r");
					TM_USART_Puts(USART1, str);
					LED_clearError();
				}
        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
}


void SDCard_readConfig() {
	char str[512];
	uint32_t count;

	fres = f_mount(&fatFs, "SD:", 0);
	
	if (fres != FR_OK) {
		sprintf(str, "Error Message @%d\n\r", fres);
		/* Put to USART */
		TM_USART_Puts(USART1, str);
	}
	if ((fres = f_open(&file, "SD:config.txt", FA_READ | FA_WRITE)) != FR_OK){
		if (fres == FR_NO_FILE) {
			sprintf(str, "NO CONFIG FILE AVAILABLE\n\r");
			TM_USART_Puts(USART1, str);
		}
		else {
			sprintf(str, "Failed to open file. Error: %d\n\r", fres);
			TM_USART_Puts(USART1, str);
		}
	}
	else {
		f_read(&file, SD_Buffer, sizeof(SD_Buffer), &count);
		sprintf(str, "Reading is done. Read %d bytes\n\r", count);
		TM_USART_Puts(USART1, str);
		f_close(&file);
		f_mount(NULL, "SD:", 1);
		sprintf(str, "File Contents:\n\r%s", SD_Buffer);
		/* Put to USART */
		TM_USART_Puts(USART1, str);
	}
}

void SDCard_extractConfig(){
	char str[512];
	char *str1, *str2, *param, *value;
	char *saveptr1, *saveptr2;
	int j;
	
	for (j=1, str1 = (char *) SD_Buffer; ; j++, str1 = NULL) {
		param = strtok_r(str1, ";", &saveptr1);
		if (param == NULL){
			break;
		}
		for (str2 = param; ; str2 = NULL) {
			value = strtok_r(str2, " = ", &saveptr2);
			if (value == NULL) {
				break;
			}
			if (strstr(value, "Cutoff") != NULL){
				str2 = NULL;
				value = strtok_r(str2, " = ", &saveptr2);
				config.LPF_cutOffFrequency = atoi(value);
			}
			else if (strstr(value, "Adc_sampling") != NULL){
				str2 = NULL;
				value = strtok_r(str2, " = ", &saveptr2);
				config.ADC_samplingRate = atoi(value);
			}
			else if (strstr(value, "Time") != NULL){
				str2 = NULL;
				value = strtok_r(str2, " = ", &saveptr2);
				config.classificationTime = atoi(value) * config.ADC_samplingRate;
			}
			sprintf(str, "--> %s\n\r", value);
			/* Put to USART */
			TM_USART_Puts(USART1, str);
		}
	}
	
			sprintf(str, "cut = %u, adc = %u, time = %u\n\r", config.LPF_cutOffFrequency, config.ADC_samplingRate, config.classificationTime);
			/* Put to USART */
			TM_USART_Puts(USART1, str);
}

void SDCard_writeData(data_type type, uint32_t data [], const char *classificationValue) {
	char str[codebookSize * codebookSize * 3] = {0}, fileName[14], time[25];
	char contents[codebookSize * codebookSize * 3] = {0};
	uint32_t count;
	uint32_t cnt;
	//static uint32_t SMatrixCount; //, AMatrixCount;
	int i;
	if (SDCardInserted){
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
		sprintf(time, "%02d.%02d.%04d %02d:%02d:%02d    ",
							datatime.date,
							datatime.month,
							datatime.year + 2000,
							datatime.hours,
							datatime.minutes,
							datatime.seconds
		);
		if (type == SMatrix_type){
			sprintf(fileName, "SD:SMatrix.txt");
		}
		if ((fres = f_mount(&fatFs, "SD:", 0)) == FR_OK){
			if ((fres = f_open(&fil, fileName, FA_READ | FA_WRITE)) == FR_NO_FILE){
				fres = f_open(&fil, fileName, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
			}
			else {
				f_read(&fil, str, sizeof(str), &cnt);
				f_lseek(&fil, cnt);
			}
			if (fres != FR_OK){
				sprintf(str, "Error Message @%d\n\r", fres);
				TM_USART_Puts(USART1, str);
				
			}
			else {
				if (type == SMatrix_type){
					for (i = 0; i < codebookSize; i++){
						if (i == 0) {
							sprintf(contents, "%s%s%u, ", time, classificationValue, ((uint32_t *)data)[i]);
						}
						else if (i == 27) {
							sprintf(contents, "%s%u\n\r", contents, ((uint32_t *)data)[i]);
						}
						else {
							sprintf(contents, "%s%u, ", contents, ((uint32_t *)data)[i]);
						}
					}
					//fres = f_write(&fil, contents, strlen(contents), count);
					count = f_puts(contents, &fil);
				}
	//			else if (type == AMatrix_type){
	//				for (i = 0; i < codebookSize * codebookSize; i++){
	//						sprintf(contents, "%s%u", contents, (data)[i]);
	//				}
	//				fres = f_write(&fil, contents, strlen(contents), count);
	//			}
				//sprintf(str, "File Contents:\n\r%s", contents);
				/* Put to USART */
				TM_USART_Puts(USART1, contents);
				sprintf(str, "Writing is done. Written %d bytes. Error = %d\n\r", count, fres);
				TM_USART_Puts(USART1, str);
				f_close(&fil);
				f_mount(NULL, "SD:", 1);
				
			}
		}
		else {
			sprintf(str, "Error Message @%d\n\r", fres);
			/* Put to USART */
			TM_USART_Puts(USART1, str);
		}
	}
}
