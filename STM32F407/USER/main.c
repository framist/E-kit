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
//GUI֧��
#include "GUI.h"
#include "FramewinDLG.h"

/************************************************
F4_HAL_emwin
��ֲ��ϣ�
����ԭ��ʽSTM32��Ŀ�ṹ + ����ԭ�ӿ� + HAL�� + STemWin
����ɹ�֧�֣�STM32F407ZGT6

 ALIENTEK STM32F407ZGT6��Сϵͳ��
 ����ѧ������
 һ�廯����ʾ���� E-kit

�漶�ӿڣ�
TFTLCD <-����-> TFTLCD(>=3.5')
VREF <-����ñ-> 3.3V
PA5 <-- adc����
PA4 --> dac���

���һʱˬ���ع����᳡��
 ��ֲ�ع�LOG��
 
 
************************************************/

//��¼��������ԭʼ���ݵ�ȫ�ֱ���
const int NumMeasurePoints = 1024; //200,000Hz����������²ɼ�1024�ɱ�֤ʶ��90hz
uint16_t OrginalV[NumMeasurePoints];
//��¼ת��Ϊ��ʵֵ���ݵ�ȫ�ֱ���(��������ʵֵ������軻Ϊת����ʵֵ)
float True_mV[NumMeasurePoints];
float Vpp_measured;
float F_measured = 1000000; //���ø�Ƶȷ��FFT��ȷ
float DR_measured;
int sampleF = 200000; //ȡ��Ƶ��
int MODE; 
int StopRun = 1; //1=Run
int IOT;

int main(void)
{
    HAL_Init();                   	//��ʼ��HAL��  
    
    Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);               	//��ʼ����ʱ����
	uart_init(115200);             	//��ʼ��USART
    
    TIM3_Init(999,83); 	//1KHZ ��ʱ��3����Ϊ1ms
    TIM4_Init(999,839);  //������ɨ���ٶ�,100HZ.
	TIM5_Init(999,0);        //������
    Wave_Output_Init();     //��ʼ�� dac-DMA-tim6 ��� 
	LED_Init();						//��ʼ��LED	
	KEY_Init();						//��ʼ��KEY
	TFTLCD_Init();           			//��ʼ��LCD FSMC�ӿ�
    TP_Init();				//��������ʼ��
	//SRAM_Init();					//��ʼ���ⲿSRAM  
    MY_ADC_Init();
	
	my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
	//my_mem_init(SRAMEX);			//��ʹ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);			//��ʼ��CCM�ڴ��
    
    __HAL_RCC_CRC_CLK_ENABLE();//ʹ��CRCʱ�ӣ�����STemWin����ʹ��

	WM_SetCreateFlags(WM_CF_MEMDEV);//Ϊ�ػ�����Զ�ʹ�ô洢�豸
	GUI_Init();
    GUI_CURSOR_Show();


// ����Ƥ��
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
        //����
        Measure();
        //����
        True_mV_To_aPoints();
        plot_aPoint(hWin_oscilloscopeFramewin);
        //��ʾ����
        refresh_Measure(hWin_oscilloscopeFramewin);
        LogPrint(".", hWin_oscilloscopeFramewin);
        LED0 = !LED0;
	} 
}

