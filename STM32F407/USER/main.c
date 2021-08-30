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
��ֲ��ϣ�
����ԭ��ʽSTM32��Ŀ�ṹ + ����ԭ�ӿ� + HAL�� + STemWin
����ɹ�֧�֣�STM32F103ZGT6
************************************************/


int main(void)
{
    HAL_Init();                   	//��ʼ��HAL��    
    Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);               	//��ʼ����ʱ����
	uart_init(115200);             	//��ʼ��USART
    
    TIM3_Init(999,83); 	//1KHZ ��ʱ��3����Ϊ1ms
    TIM4_Init(999,839);  //������ɨ���ٶ�,100HZ.
	
	LED_Init();						//��ʼ��LED	
	KEY_Init();						//��ʼ��KEY
	TFTLCD_Init();           			//��ʼ��LCD FSMC�ӿ�
    TP_Init();				//��������ʼ��
	SRAM_Init();					//��ʼ���ⲿSRAM  
	
	my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);			//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);			//��ʼ��CCM�ڴ��
    
    __HAL_RCC_CRC_CLK_ENABLE();//ʹ��CRCʱ�ӣ�����STemWin����ʹ��

	WM_SetCreateFlags(WM_CF_MEMDEV);//Ϊ�ػ�����Զ�ʹ�ô洢�豸
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

