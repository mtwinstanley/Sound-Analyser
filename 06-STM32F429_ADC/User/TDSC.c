#include "TDSC.h"
#include "samplingTimer.h"

ADC_values_t ADC_values;
TDSC_crossings_t TDSC_crossings;

void TDSC_init(){
	ADC_values.prev = ADC_values.current = ADC_values.next = 0;
	TDSC_crossings.crossings = TDSC_crossings.positive = TDSC_crossings.prev = TDSC_crossings.length = 0;
	samplingTimer_init();
	samplingTimer_NVICInit();
}
void TDSC_sampleRoutine(uint16_t read){
		TDSC_adjustValues(read);
		if (read > 2000 && TDSC_crossings.positive == 0){
			TDSC_crossings.positive = 1;
			TDSC_crossings.collection[TDSC_crossings.length] = TDSC_crossings.crossings;
			TDSC_crossings.crossings = 0;
			TDSC_crossings.length++;
		}
		else if (read < 2000 && TDSC_crossings.positive == 1){
			TDSC_crossings.positive = 0;
			TDSC_crossings.collection[TDSC_crossings.length] = TDSC_crossings.crossings;
			TDSC_crossings.crossings = 0;
			TDSC_crossings.length++;
		}
		else if (TDSC_crossings.positive && TDSC_positiveMinima()){
			TDSC_crossings.crossings++;
		}
		else if (!TDSC_crossings.positive && TDSC_negativeMinima()){
			TDSC_crossings.crossings++;
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
