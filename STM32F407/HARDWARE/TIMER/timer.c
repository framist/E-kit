/**
 * @file timer.c
 * @author framist (framist@163.com)
 * @brief ͨ�ö�ʱ��TIM3 -> OS_TimeMS ��
 *        ͨ�ö�ʱ��TIM4 -> ����������
 *        ͨ�ö�ʱ��TIM5 -> �ж�ȡ����
 * @version 0.1
 * @date 2021-08-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "timer.h"
#include "led.h"
#include "GUI.h"
extern __IO int32_t OS_TimeMS; //GUI ��
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//��ֲ������STemWin - framist
////////////////////////////////////////////////////////////////////////////////// 	


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//999,83
//Ft = 84Mhz
//1KHZ ��ʱ��3����Ϊ1ms
TIM_HandleTypeDef 	TIM3_Handler;      	//��ʱ����� 
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
    //HAL_TIM_Base_Stop_IT(&TIM3_Handler);
}

TIM_HandleTypeDef 	TIM4_Handler;      	//��ʱ��4��� 
// 
void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��4
    TIM4_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����=0
    HAL_TIM_Base_Init(&TIM4_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE
}

TIM_HandleTypeDef 	TIM5_Handler;      	//��ʱ��5��� 
// 
void TIM5_Init(u16 arr,u16 psc)
{  
    TIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��4
    TIM5_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM5_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����=0
    HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); //ʹ��
    HAL_TIM_Base_Stop_IT(&TIM5_Handler); //�ر�
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
// StemWin ��Ҫ
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM4ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn,2,3);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����TIM4�ж�   
	}
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}
    if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
		HAL_NVIC_SetPriority(TIM5_IRQn,1,2);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�2
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�   
	}
    if(htim->Instance==TIM6) {
        /* TIM6 clock enable */
        __HAL_RCC_TIM6_CLK_ENABLE();
    }
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM5_Handler);
}

//�ص���������ʱ���жϷ���������
extern int iNumMeasurePoints;
extern int NumMeasurePoints;
extern uint16_t OrginalV[];
extern ADC_HandleTypeDef ADC1_Handler;	


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    if(htim==(&TIM4_Handler)){
        GUI_TOUCH_Exec();    //��ʱ���������¼�
    }
    if(htim==(&TIM3_Handler)){
        OS_TimeMS++;         //ϵͳ����
    }
    if(htim==(&TIM5_Handler)){
        //��ʱ����ȡ��
        if(iNumMeasurePoints >= NumMeasurePoints) return;

        //�������һ��ADC1�������ת�����; 4095-ֵ ����Ϊ���õ�·��Ե��
        OrginalV[iNumMeasurePoints] = 4095 - (u16)HAL_ADC_GetValue(&ADC1_Handler);     
        iNumMeasurePoints++;        
    }

}

