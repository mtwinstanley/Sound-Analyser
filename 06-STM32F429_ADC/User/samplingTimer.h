#ifndef _SAMPLINGTIMER_H
#define _SAMPLINGTIMER_H

#include <stdint.h>
#include <stdio.h>
extern uint32_t ADC_samplingRate;

/* Function Prototypes */
void samplingTimer_init(void);
void samplingTimer_NVICInit(void);

#endif /* _SAMPLINGTIMER_H */
