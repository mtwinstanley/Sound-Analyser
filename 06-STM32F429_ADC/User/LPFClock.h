#ifndef _LPFCLOCK_H
#define _LPFCLOCK_H

#include <stdint.h>
#include <stdio.h>

/* Function Prototypes */
void LPFClock_init(void);
void LPFClock_PWMInit(uint32_t period);
void LPFClock_GPIOInit(void);
	
extern uint32_t LPF_cutOffFrequency;
#endif /* _LPFCLOCK_H */
