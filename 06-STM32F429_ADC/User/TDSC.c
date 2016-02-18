#include "TDSC.h"
#include "samplingTimer.h"
#include "codebook.h"

ADC_values_t ADC_values;
TDSC_crossings_t TDSC_crossings;

volatile uint32_t SMatrix[28] = {0};
volatile uint32_t AMatrix[28][28] = {0};

void TDSC_init(){
	ADC_values.prev = ADC_values.current = ADC_values.next = 0;
	TDSC_crossings.shape = TDSC_crossings.positive = TDSC_crossings.lagCode = 0;
	samplingTimer_init();
	samplingTimer_NVICInit();
}
void TDSC_sampleRoutine(uint16_t read){
	uint8_t code;

	TDSC_adjustValues(read);
	TDSC_crossings.duration++;
	if (read > 2000 && TDSC_crossings.positive == 0){
		TDSC_crossings.positive = 1;
		code = codebook_getCode(TDSC_crossings.shape, TDSC_crossings.duration);
		TDSC_setSMatrixValue(code);
		TDSC_setAMatrixValue(code, TDSC_crossings.lagCode);
		TDSC_crossings.lagCode = code;
		TDSC_crossings.shape = 0;
		TDSC_crossings.duration = 0;
	}
	else if (read < 2000 && TDSC_crossings.positive == 1){
		TDSC_crossings.positive = 0;
		code = codebook_getCode(TDSC_crossings.shape, TDSC_crossings.duration);
		TDSC_setSMatrixValue(code);
		TDSC_setAMatrixValue(code, TDSC_crossings.lagCode);
		TDSC_crossings.lagCode = code;
		TDSC_crossings.shape = 0;
		TDSC_crossings.duration = 0;
	}
	else if (TDSC_crossings.positive && TDSC_positiveMinima()){
		TDSC_crossings.shape++;
	}
	else if (!TDSC_crossings.positive && TDSC_negativeMinima()){
		TDSC_crossings.shape++;
	}
}

void TDSC_adjustValues(uint16_t adcRead){
	ADC_values.prev = ADC_values.current;
	ADC_values.current = ADC_values.next;
	ADC_values.next = adcRead;
}

uint8_t TDSC_positiveMinima(){
	if (ADC_values.current < ADC_values.next && ADC_values.current < ADC_values.prev){
		return 1;
	}
	else{
		return 0;
	}
}

uint8_t TDSC_negativeMinima(){
	if (ADC_values.current > ADC_values.next && ADC_values.current > ADC_values.prev){
		return 1;
	}
	else{
		return 0;
	}
}

void TDSC_setSMatrixValue(uint8_t code){
	if (code == 0){
		return;
	}
	SMatrix[code - 1] = SMatrix[code-1] + 1; 
}

void TDSC_setAMatrixValue(uint8_t code, uint8_t lag){
	if (code == 0 || lag == 0){
		return;
	}
	AMatrix[code-1][lag-1] = AMatrix[code-1][lag-1] + 1;
}