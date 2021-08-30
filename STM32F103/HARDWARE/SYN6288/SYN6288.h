#ifndef __SYN6288_H__
#define __SYN6288_H__

#include "main.h"
int SYN6288_SendVoiceText(const char * Voice);
void SYN6288_init(UART_HandleTypeDef UARTx_Handler, unsigned int bound);

#endif



