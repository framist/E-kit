#ifndef AD9850_H
#define AD9850_H

#include <stdint.h>

void AD9850_Init(void);
void AD9850_Reset(void);
void AD9850_Write(uint8_t byte);
void AD9850_Update_Freq(float freq);

#endif  // AD9850_H
