#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "ILI93xx.h"
#include "key.h"
#include "sram.h"
#include "malloc.h"
#include "touch.h"
#include "timer.h"
#include "arm_math.h"  
#include "wave_output.h"
#include "adc.h"
//GUI支持
#include "GUI.h"
#include "FramewinDLG.h"

/************************************************
F4_HAL_emwin
移植组合：
正点原子式STM32项目结构 + 正点原子库 + HAL库 + STemWin
试验成功支持：STM32F407ZGT6

 ALIENTEK STM32F407ZGT6最小系统版
 物理学术竞赛
 一体化万能示波器 E-kit

版级接口：
TFTLCD <-排线-> TFTLCD(>=3.5')
VREF <-跳线帽-> 3.3V
PA5 <-- adc输入
PA4 --> dac输出

耦合一时爽，重构火葬场！
 移植重构LOG：
 
 
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
    HAL_Init();                   	//初始化HAL库  
    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART
    
    TIM3_Init(999,83); 	//1KHZ 定时器3设置为1ms
    TIM4_Init(999,839);  //触摸屏扫描速度,100HZ.
	TIM5_Init(999,0);        //采样用
    Wave_Output_Init();     //初始化 dac-DMA-tim6 输出 
	LED_Init();						//初始化LED	
	KEY_Init();						//初始化KEY
	TFTLCD_Init();           			//初始化LCD FSMC接口
    TP_Init();				//触摸屏初始化
	//SRAM_Init();					//初始化外部SRAM  
    MY_ADC_Init();
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	//my_mem_init(SRAMEX);			//不使用外部内存池
	my_mem_init(SRAMCCM);			//初始化CCM内存池
    
    __HAL_RCC_CRC_CLK_ENABLE();//使能CRC时钟，否则STemWin不能使用

	WM_SetCreateFlags(WM_CF_MEMDEV);//为重绘操作自动使用存储设备
	GUI_Init();
    GUI_CURSOR_Show();


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

    WM_HWIN CreatemainFramewin(void);
    CreatemainFramewin();
    
    mainLogPrint("\ninit over!");
    
    char stemp[100] = "";
    extern ADC_HandleTypeDef ADC1_Handler;	

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
	} 
}

