#ifndef _SDCard_H
#define _SDCard_H
#include <stdint.h>

#define FATFS_USE_DETECT_PIN_PORT        	GPIOC
#define FATFS_USE_DETECT_PIN_PIN        	GPIO_PIN_7

typedef enum {
	SMatrix_type,
	//AMatrix_type
} data_type;

void SDCard_readConfig(void); 
void SDCard_extractConfig(void);
void SDCard_writeData(data_type type, uint32_t data[]);

#endif /* _SDCard_H */
