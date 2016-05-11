/**
 *	ARM-based Real-time Sound Analyser and Classifier
 *
 *	This file contains the function prototypes for the SD Card
 *
 *  @file		SDCard.h
 *	@author		Matt Winstanley	
 *	@email		mle.winstanley@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
 
#ifndef _SDCard_H
#define _SDCard_H
#include <stdint.h>

/* Global Defines */
#define FATFS_SDIO_4BIT         1

#define FATFS_USE_DETECT_PIN            1
#define FATFS_USE_DETECT_PIN_PORT        	GPIOC
#define FATFS_USE_DETECT_PIN_PIN        	GPIO_PIN_7

/* Type of data for transmission */
typedef enum {
  SMatrix_type
  /* Add additional data types in here */
} data_type;

extern uint8_t SDCardInserted;

/* Function Prototypes */
void SDCard_configureCD(void);
void SDCard_readConfig(void); 
void SDCard_extractConfig(void);
void SDCard_writeData(data_type type, uint32_t data[], const char *classificationValue);

#endif /* _SDCard_H */
