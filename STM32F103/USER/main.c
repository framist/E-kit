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
#include "24l01.h" // ����ͨѶ����
#include "SYN6288.h" //��������
#include "AD9850.h" //AD9851
#include "wave_output.h"
#include <stdio.h>

//GUI֧��
#include "GUI.h"
#include "FramewinDLG.h"

/************************************************
 ALIENTEK ��ӢSTM32F103������
 ����ѧ������
 һ�廯����ʾ���� E-kit

 ���һʱˬ���ع����᳡��
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
    HAL_Init();                   	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
    Wave_Output_Init();
    MY_ADC_Init();                      //��ʼ��ADC
	delay_init(72);               		//��ʼ����ʱ����
    //HAL_NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� ��������ԭ���޸���hal��
    
	uart_init(9600);					//��ʼ������
    SYN6288_init(UART1_Handler,19200);  //��ʼ������ģ��
	uart_init(19200);					//��ʼ������

	LED_Init();							//��ʼ��LED	

	KEY_Init();							//��ʼ������
	LCD_Init();							//��ʼ��LCD
	//tp_dev.init();				   		//��������ʼ�� 


 	TP_Init();				//��������ʼ��
    //TP_Adjust();
    
	SRAM_Init();		//��ʼ��SRAM
	TIM3_Init(999,71);	//1KHZ ��ʱ��1ms 
    TIM4_Init(999,0);    //�����ö�ʱ���ж�
	TIM6_Init(999,719);	//10ms�ж�

	// my_mem_init(SRAMIN); 		//��ʼ���ڲ��ڴ��
	// my_mem_init(SRAMEX);  		//��ʼ���ⲿ�ڴ��
    AD9850_Init();
    
	
	__HAL_RCC_CRC_CLK_ENABLE();//ʹ��CRCʱ�ӣ�����STemWin����ʹ�� 
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
    GUI_CURSOR_Show();
    
    NRF24L01_Init();    		    	//��ʼ��NRF24L01 ����ģ��
    
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
    //IOT��
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
    SYN6288_SendVoiceText("[o0][v4]��ʼ�����");
    mainLogPrint("\ninit over!");
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
        
        //TOT_TX--------------------
        if(IOT==0) continue;

        for(j=0;j<64;j++) { //�ٴ�һ��
            sprintf(stemp, "%9.4f",True_mV[j*4] );//ָ�����������ռ9�У����а���4λС����
            
            //����һ������
            NRF24L01_TX_Mode();
            if(NRF24L01_TxPacket(tmp_buf)==TX_OK) {
                for(i=0;i<9;i++) { 
                    tmp_buf[i] = stemp[i];
                }
                for(i=9;i<32;i++) { 
                    tmp_buf[i] = ' ';
                }
                tmp_buf[32]=0;//���������		   
            }else {
                LogPrint("Send Failed!", hWin_oscilloscopeFramewin);
                break;
            };
            HAL_Delay(20);
        }
	}
}

