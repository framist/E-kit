#ifndef _WAVE_OUTPUT_H
#define _WAVE_OUTPUT_H
#include "sys.h"
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//	输出波形							  
////////////////////////////////////////////////////////////////////////////////// 	
#define N_WaveData 100
extern uint16_t DAC_DMA_Data[N_WaveData];
void Error_Handler(void);

void Wave_Output_Init(void);
void Wave_Output_Start(void);
void Wave_Output_Stop(void);
void Wave_Output_Config_F(float f);
void Wave_Output_Config_Form(enum Wave_Form Output_Wave_Form, int duty);
void Wave_Output_Config_VppOffset(float Vpp, float offset);
int Wave_Output_Config(enum Wave_Form Output_Wave_Form, float f, float Vpp, float offset,int duty);
#endif
