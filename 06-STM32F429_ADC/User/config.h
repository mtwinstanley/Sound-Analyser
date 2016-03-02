#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

typedef struct {
	uint32_t ADC_samplingRate;
	uint32_t LPF_cutOffFrequency;

	uint32_t classificationTime;
}config_type;

extern config_type config;

#endif /* _CONFIG_H */
