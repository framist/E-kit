#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "24cxx.h"
#include "w25qxx.h"
#include "touch.h"
#include "sram.h"
#include "timer.h"
#include "malloc.h"
#include "adc.h"
#include "24l01.h" // 无线通讯功能
#include "SYN6288.h" //语音功能
#include "AD9850.h" //AD9851
#include "wave_output.h"
#include <stdio.h>

//GUI支持
#include "GUI.h"
#include "FramewinDLG.h"

/************************************************
 ALIENTEK 精英STM32F103开发板
 物理学术竞赛
 一体化万能示波器 E-kit

 耦合一时爽，重构火葬场！
************************************************/

//记录采样数据原始数据的全局变量
const int NumMeasurePoints = 1024; //200,000Hz采样率情况下采集1024可保证识别到90hz
uint16_t OrginalV[NumMeasurePoints];
//记录转换为真实值数据的全局变量(现在是真实值，最后需换为转换真实值)
float True_mV[NumMeasurePoints];
float Vpp_measured;
float F_measured = 1000000; //先用高频确保FFT正确
float DR_measured;
int sampleF = 200000; //取样频率
int MODE; 
int StopRun = 1; //1=Run
int IOT;

int main(void)
{
    HAL_Init();                   	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
    Wave_Output_Init();
    MY_ADC_Init();                      //初始化ADC
	delay_init(72);               		//初始化延时函数
    //HAL_NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级 已有正点原子修改在hal库
    
	uart_init(9600);					//初始化串口
    SYN6288_init(UART1_Handler,19200);  //初始化语音模块
	uart_init(19200);					//初始化串口

	LED_Init();							//初始化LED	

	KEY_Init();							//初始化按键
	LCD_Init();							//初始化LCD
	//tp_dev.init();				   		//触摸屏初始化 


 	TP_Init();				//触摸屏初始化
    //TP_Adjust();
    
	SRAM_Init();		//初始化SRAM
	TIM3_Init(999,71);	//1KHZ 定时器1ms 
    TIM4_Init(999,0);    //测量用定时器中断
	TIM6_Init(999,719);	//10ms中断

	// my_mem_init(SRAMIN); 		//初始化内部内存池
	// my_mem_init(SRAMEX);  		//初始化外部内存池
    AD9850_Init();
    
	
	__HAL_RCC_CRC_CLK_ENABLE();//使能CRC时钟，否则STemWin不能使用 
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
    GUI_CURSOR_Show();
    
    NRF24L01_Init();    		    	//初始化NRF24L01 无线模块
    
// 更换皮肤
// #define USE_SKIN_FLEX
#ifdef USE_SKIN_FLEX
#include "DIALOG.h"
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
    CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    MENU_SetDefaultSkin(MENU_SKIN_FLEX);
    MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
#endif
    

    void True_mV_To_aPoints(void);
    void plot_aPoint(WM_HWIN hWin);
    void LogPrint(char *log, WM_HWIN  hWin);
    void refresh_Measure(WM_HWIN hWin);
    //IOT用
    static u8 tmp_buf[33];	
    char stemp[33] = "";
    int i;
    int j;
        
    

    WM_HWIN CreatemainFramewin(void);
    CreatemainFramewin();

    
    while(NRF24L01_Check()) {
        mainLogPrint("NRF24L01 Error ");
		delay_ms(200);
	}
    SYN6288_SendVoiceText("[o0][v4]初始化完成");
    mainLogPrint("\ninit over!");
	while(1)
	{
		GUI_Delay(100); 
        GUI_Exec();
        if(StopRun==0) continue;
        //测量
        Measure();
        //绘制
        True_mV_To_aPoints();
        plot_aPoint(hWin_oscilloscopeFramewin);
        //显示数据
        refresh_Measure(hWin_oscilloscopeFramewin);
        LogPrint(".", hWin_oscilloscopeFramewin);
        LED0 = !LED0;
        
        //TOT_TX--------------------
        if(IOT==0) continue;

        for(j=0;j<64;j++) { //少传一点
            sprintf(stemp, "%9.4f",True_mV[j*4] );//指定输出的数据占9列，其中包括4位小数：
            
            //传输一个幅度
            NRF24L01_TX_Mode();
            if(NRF24L01_TxPacket(tmp_buf)==TX_OK) {
                for(i=0;i<9;i++) { 
                    tmp_buf[i] = stemp[i];
                }
                for(i=9;i<32;i++) { 
                    tmp_buf[i] = ' ';
                }
                tmp_buf[32]=0;//加入结束符		   
            }else {
                LogPrint("Send Failed!", hWin_oscilloscopeFramewin);
                break;
            };
            HAL_Delay(20);
        }
	}
}

