#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "ILI93xx.h"
#include "key.h"
#include "sram.h"
#include "malloc.h"
#include "touch.h"
#include "GUI.h"
#include "timer.h"
#include "arm_math.h"  
#include "GUIDemo.h"
/************************************************
F4_HAL_emwin
移植组合：
正点原子式STM32项目结构 + 正点原子库 + HAL库 + STemWin
试验成功支持：STM32F103ZGT6
************************************************/


int main(void)
{
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART
    
    TIM3_Init(999,83); 	//1KHZ 定时器3设置为1ms
    TIM4_Init(999,839);  //触摸屏扫描速度,100HZ.
	
	LED_Init();						//初始化LED	
	KEY_Init();						//初始化KEY
	TFTLCD_Init();           			//初始化LCD FSMC接口
    TP_Init();				//触摸屏初始化
	SRAM_Init();					//初始化外部SRAM  
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	my_mem_init(SRAMEX);			//初始化外部内存池
	my_mem_init(SRAMCCM);			//初始化CCM内存池
    
    __HAL_RCC_CRC_CLK_ENABLE();//使能CRC时钟，否则STemWin不能使用

	WM_SetCreateFlags(WM_CF_MEMDEV);//为重绘操作自动使用存储设备
	GUI_Init();

//	GUI_SetBkColor(GUI_BLUE);
//	GUI_Clear();
//	GUI_DispStringAt("hello, world\n      -- framist",0,0);
//	HAL_Delay(3000);

	GUIDEMO_Main();
    while(1)
	{
		;
	} 
}

