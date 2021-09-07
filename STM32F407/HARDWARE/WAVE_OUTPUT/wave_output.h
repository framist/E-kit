#ifndef _WAVE_OUTPUT_H
#define _WAVE_OUTPUT_H
#include "sys.h"
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//	输出波形							  
////////////////////////////////////////////////////////////////////////////////// 	

void Error_Handler(void);

void Wave_Output_Init(void);
void Wave_Output_Start(void);
void Wave_Output_Stop(void);
void Wave_Output_F(float f);
int Wave_Output_Config(enum Wave_Form Output_Wave_Form, float f, float Vpp, float offset,float duty);
#endif
