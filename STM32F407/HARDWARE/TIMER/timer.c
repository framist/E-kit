/**
 * @file timer.c
 * @author framist (framist@163.com)
 * @brief 通用定时器TIM3 -> OS_TimeMS 用
 *        通用定时器TIM4 -> 处理触摸采样
 *        通用定时器TIM5 -> 中断取样用
 * @version 0.1
 * @date 2021-08-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "timer.h"
#include "led.h"
#include "GUI.h"
extern __IO int32_t OS_TimeMS; //GUI 用
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//移植以适配STemWin - framist
////////////////////////////////////////////////////////////////////////////////// 	


//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//999,83
//Ft = 84Mhz
//1KHZ 定时器3设置为1ms
TIM_HandleTypeDef 	TIM3_Handler;      	//定时器句柄 
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
    //HAL_TIM_Base_Stop_IT(&TIM3_Handler);
}

TIM_HandleTypeDef 	TIM4_Handler;      	//定时器4句柄 
// 
void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //通用定时器4
    TIM4_Handler.Init.Prescaler=psc;                     //分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Handler.Init.Period=arr;                        //自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子=0
    HAL_TIM_Base_Init(&TIM4_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器3和定时器4更新中断：TIM_IT_UPDATE
}

TIM_HandleTypeDef 	TIM5_Handler;      	//定时器5句柄 
// 
void TIM5_Init(u16 arr,u16 psc)
{  
    TIM5_Handler.Instance=TIM5;                          //通用定时器4
    TIM5_Handler.Init.Prescaler=psc;                     //分频系数
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM5_Handler.Init.Period=arr;                        //自动装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子=0
    HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); //使能
    HAL_TIM_Base_Stop_IT(&TIM5_Handler); //关闭
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
// StemWin 需要
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM4时钟
		HAL_NVIC_SetPriority(TIM4_IRQn,2,3);    //设置中断优先级，抢占优先级2，子优先级3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启TIM4中断   
	}
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
    if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
		HAL_NVIC_SetPriority(TIM5_IRQn,1,2);    //设置中断优先级，抢占优先级1，子优先级2
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断   
	}
    if(htim->Instance==TIM6) {
        /* TIM6 clock enable */
        __HAL_RCC_TIM6_CLK_ENABLE();
    }
}

//定时器3中断服务函数
void TIM3_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//定时器4中断服务函数
void TIM4_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

//定时器5中断服务函数
void TIM5_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM5_Handler);
}

//回调函数，定时器中断服务函数调用
extern int iNumMeasurePoints;
extern int NumMeasurePoints;
extern uint16_t OrginalV[];
extern ADC_HandleTypeDef ADC1_Handler;	


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    if(htim==(&TIM4_Handler)){
        GUI_TOUCH_Exec();    //定时处理触摸屏事件
    }
    if(htim==(&TIM3_Handler)){
        OS_TimeMS++;         //系统心跳
    }
    if(htim==(&TIM5_Handler)){
        //定时单次取样
        if(iNumMeasurePoints >= NumMeasurePoints) return;

        //返回最近一次ADC1规则组的转换结果; 4095-值 是因为外置电路的缘故
        OrginalV[iNumMeasurePoints] = 4095 - (u16)HAL_ADC_GetValue(&ADC1_Handler);     
        iNumMeasurePoints++;        
    }

}

