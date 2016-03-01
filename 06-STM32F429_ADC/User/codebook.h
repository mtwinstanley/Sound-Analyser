#ifndef _CODEBOOK_H
#define _CODEBOOK_H

#include <stdint.h>

#define codebookSize 28

uint8_t codebook_getCode(uint8_t shape, uint8_t duration);

#endif /* _CODEBOOK_H */
