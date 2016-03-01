
#include "SDCard.h"
#include "codebook.h"
#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_usart.h"
#include <string.h>

/* FATFS related */
FATFS fatFs;
FIL file;
FRESULT fres;
uint8_t SD_Buffer[512];

void SDCard_readConfig() {
	char str[512];
	uint32_t count;

	if ((fres = f_mount(&fatFs, "SD:", 0)) == FR_OK){
		if ((fres = f_open(&file, "SD:SMatrix.txt", FA_READ | FA_WRITE)) != FR_OK){
			sprintf(str, "Error Message @%d\n\r", fres);
			TM_USART_Puts(USART1, str);
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
	
}

void SDCard_writeData(data_type type, uint32_t data []) {
	char str[codebookSize * codebookSize * 3] = {0}, fileName[14];
	char contents[codebookSize * codebookSize * 3] = {0};
	uint32_t * count;
	static uint32_t SMatrixCount, AMatrixCount;
	uint32_t a[28 * 28];
	int i, j;
	
	if (type == SMatrix_type){
		sprintf(fileName, "SD:SMatrix.txt");
		count = &SMatrixCount;
	}
	else if (type == AMatrix_type){
		sprintf(fileName, "SD:AMatrix.txt");
		count = &AMatrixCount;
	}
	if ((fres = f_mount(&fatFs, "SD:", 0)) == FR_OK){
		if ((fres = f_open(&file, fileName, FA_CREATE_ALWAYS | FA_READ | FA_WRITE)) == FR_NO_FILE){
			fres = f_open(&file, fileName, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
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
				f_write(&file, contents, strlen(contents), count);
			}
			else if (type == AMatrix_type){
				for (i = 0; i < codebookSize * codebookSize; i++){
//					if (i == 0) {
//						sprintf(contents, "%u, ",(data)[i]);
//					}
//					else if (i % codebookSize - 1 == 0) {
//						sprintf(contents, "%s%u\n\r", contents, (data)[i]);
//					}
//					else {
						sprintf(contents, "%s%u", contents, (data)[i]);
//					}
				}
				f_write(&file, contents, strlen(contents), count);
			}
			//sprintf(str, "File Contents:\n\r%s", contents);
			/* Put to USART */
			TM_USART_Puts(USART1, contents);
			sprintf(str, "Writing is done. Written %d bytes\n\r", *count);
			TM_USART_Puts(USART1, str);
			f_close(&file);
			f_mount(NULL, "SD:", 1);
			
		}
	}
	else {
		sprintf(str, "Error Message @%d\n\r", fres);
		/* Put to USART */
		TM_USART_Puts(USART1, str);
	}
}
