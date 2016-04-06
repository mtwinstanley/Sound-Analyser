#ifndef _SDCard_H
#define _SDCard_H
#include <stdint.h>

#define FATFS_SDIO_4BIT         1

#define FATFS_USE_DETECT_PIN            1
#define FATFS_USE_DETECT_PIN_PORT        	GPIOC
#define FATFS_USE_DETECT_PIN_PIN        	GPIO_PIN_7

typedef enum {
	SMatrix_type,
	//AMatrix_type
} data_type;

extern uint8_t SDCardInserted;

void SDCard_readConfig(void); 
void SDCard_extractConfig(void);
void SDCard_writeData(data_type type, uint32_t data[], const char *classificationValue);

#endif /* _SDCard_H */
