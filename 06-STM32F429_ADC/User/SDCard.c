
#include "SDCard.h"
#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_usart.h"

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

void SDCard_writeData(data_type type, uint32_t data[]) {
	char str[512], fileName[14];
	uint32_t * count;
	static uint32_t SMatrixCount, AMatrixCount;
	int i;
	uint32_t SMatrixTest[28] = {0,0,0,6,0,0,0,8,0,0,0,0,0,14,0,0,0,0,0,20,0,0,0,0,0,0,0,28};
	
	if (type == SMatrix_type){
		sprintf(fileName, "SD:SMatrix.txt");
		count = &SMatrixCount;
	}
	else if (type == AMatrix_type){
		sprintf(fileName, "SD:AMatrix.txt");
		count = &AMatrixCount;
	}
	if ((fres = f_mount(&fatFs, "SD:", 0)) == FR_OK){
		if ((fres = f_open(&file, fileName, FA_READ | FA_WRITE)) == FR_NO_FILE){
			fres = f_open(&file, fileName, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
		}
		if (fres != FR_OK){
			sprintf(str, "Error Message @%d\n\r", fres);
			TM_USART_Puts(USART1, str);
			
		}
		else {
			if (type == SMatrix_type){
				for (i = 0; i < (sizeof(SMatrixTest) / sizeof(SMatrixTest[0])); i++){
					f_write(&file, &SMatrixTest[i], sizeof(SMatrixTest[i]), count);
					//sprintf(str, "%u, ", SMatrixTest[i]);
				/* Put to USART */
				//TM_USART_Puts(USART1, str);
				}
				//f_write(&file, "\n\r", 2, count);
				
				sprintf(str, "File Contents:\n\r%s", (char* ) SMatrixTest);
				/* Put to USART */
				TM_USART_Puts(USART1, str);
			}
			else if (type == AMatrix_type){
//				for (int i = 0; i < (sizeof(data) / sizeof(data[0])); i++){
//					for (int j = 0; j < (sizeof(data) / sizeof(data[0])); j++){
//						f_write(&file, &data[i][j], sizeof(data[i][j]), count);
//					}
//				}
			}
			
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
