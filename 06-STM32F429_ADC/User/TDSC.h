#ifndef _TDSC_H
#define _TDSC_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
	uint16_t prev;
	uint16_t current;
	uint16_t next;
}ADC_values_t;

typedef struct {
	uint8_t shape;
	uint8_t positive;
	uint8_t duration;
	uint8_t lagCode;
}TDSC_crossings_t;

extern ADC_values_t ADC_values;
extern TDSC_crossings_t TDSC_crossings;

void TDSC_adjustValues(uint16_t adcRead);
void TDSC_sampleRoutine(uint16_t read);
void TDSC_init(void);
uint8_t TDSC_positiveMinima(void);
uint8_t TDSC_negativeMinima(void);
void TDSC_setSMatrixValue(uint8_t code);
void TDSC_setAMatrixValue(uint8_t code, uint8_t lag);

#endif /* _TDSC_H */
