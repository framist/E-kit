#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��ʱ����������	   
//�޸�����STemWin								  
////////////////////////////////////////////////////////////////////////////////// 	
//extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
//extern TIM_HandleTypeDef TIM6_Handler;    	//��ʱ��6��� 

void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM6_Init(u16 arr,u16 psc);
extern TIM_HandleTypeDef htim2;

void MX_TIM2_Init(void);

#endif

