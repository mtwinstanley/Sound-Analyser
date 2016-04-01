
#include "SDCard.h"
#include "codebook.h"
#include "config.h"
#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_rtc.h"
#include <string.h>

/* FATFS related */
FATFS fatFs;
FIL file, fil;
FRESULT fres;
uint8_t SD_Buffer[512];

/* RTC */
//TM_RTC_Time_t datatime;

void SDCard_readConfig() {
	char str[512];
	uint32_t count;

	if ((fres = f_mount(&fatFs, "SD:", 0)) == FR_OK){
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
	else {
		sprintf(str, "Error Message @%d\n\r", fres);
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

void SDCard_writeData(data_type type, uint32_t data []) {
	char str[codebookSize * codebookSize * 3] = {0}, fileName[14];
	char contents[codebookSize * codebookSize * 3] = {0};
	uint32_t * count;
	static uint32_t SMatrixCount; //, AMatrixCount;
	int i;
	
	if (type == SMatrix_type){
		sprintf(fileName, "SD:SMatrix.txt");
		count = &SMatrixCount;
	}
	if ((fres = f_mount(&fatFs, "SD:", 0)) == FR_OK){
		if ((fres = f_open(&fil, fileName, FA_READ | FA_WRITE)) == FR_NO_FILE){
			fres = f_open(&fil, fileName, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
		}
		if (fres != FR_OK){
			sprintf(str, "Error Message @%d\n\r", fres);
			TM_USART_Puts(USART1, str);
			
		}
		else {
			if (type == SMatrix_type){
				for (i = 0; i < codebookSize; i++){
					if (i == 0) {
						sprintf(contents, "%u, ",((uint32_t *)data)[i]);
					}
					else if (i == 27) {
						sprintf(contents, "%s%u\n\r", contents, ((uint32_t *)data)[i]);
					}
					else {
						sprintf(contents, "%s%u, ", contents, ((uint32_t *)data)[i]);
					}
				}
				fres = f_write(&fil, contents, strlen(contents), count);
			}
//			else if (type == AMatrix_type){
//				for (i = 0; i < codebookSize * codebookSize; i++){
//						sprintf(contents, "%s%u", contents, (data)[i]);
//				}
//				fres = f_write(&fil, contents, strlen(contents), count);
//			}
			//sprintf(str, "File Contents:\n\r%s", contents);
			/* Put to USART */
			//TM_USART_Puts(USART1, contents);
			sprintf(str, "Writing is done. Written %d bytes. Error = %d\n\r", *count, fres);
			//TM_USART_Puts(USART1, str);
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
