#include "adc.h"
#include "delay.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////	 
//  my adc								  
////////////////////////////////////////////////////////////////////////////////// 	

ADC_HandleTypeDef ADC1_Handler;		//ADC���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
	
	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC����ʱ��
	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//��Ƶ����6ʱ��Ϊ72M/6=12MHz
	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//����ADCʱ��
	
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.ContinuousConvMode=ENABLE;                //�ر�����ת�� -> ����,����ȷ�Ƚ���
    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
	
	HAL_ADCEx_Calibration_Start(&ADC1_Handler);					 //У׼ADC
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_1;            //PA1
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;      //����ʱ��    ��Ϊ���           
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    //HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        	//�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 


//��ʱ�����һ�֣���¼���ݵ�ȫ������
float wave_frequency_calculate(void);
volatile int iNumMeasurePoints;
void Measure(void){
    extern const int NumMeasurePoints;
    extern uint16_t OrginalV[];
    extern float True_mV[];
    extern float Vpp_measured;
    extern float F_measured;
    extern float DR_measured;
    extern int MODE; 
    extern int sampleF;
    extern int us_div;
    extern TIM_HandleTypeDef 	TIM4_Handler;      	//��ʱ��4��� 
    int i;
    float max_mV = -5000;
    float min_mV = 5000;
    float temp;
    float sum = 0;
    float offSet;
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    ADC1_ChanConf.Channel=ADC_CHANNEL_1;                        //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;         //����ʱ��    ��Ϊ���           
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
    
//    for(i=0; i<NumMeasurePoints; i++){
//        OrginalV[i] = (u16)HAL_ADC_GetValue(&ADC1_Handler);     //�������һ��ADC1�������ת�����
//    }
    
    //��ʱ�����Ƶ�ʼ���: F = Ft/((arr+1)*(psc+1)) us.
    // arr  ; psc = 0 ; 1ms
    //Ft=��ʱ������Ƶ��,72Mhz
    // arr = Ft/F - 1   ; Fmax~200,000;һ��ȡ��5us
    //F �Զ����ڣ�
    if (sampleF == 200000 && F_measured<2000) {
        sampleF = 20000;
        us_div = 1000; //ǿ�Ƹı� us_div
    }else if (sampleF == 20000 && F_measured>1500) {
        sampleF = 200000;
        us_div = 100; //ǿ�Ƹı� us_div
    }
    TIM4->ARR = (uint16_t)(72000000/(sampleF) - 1); 
    iNumMeasurePoints = 0;
    HAL_TIM_Base_Start_IT(&TIM4_Handler);     //ʹ��
    while(iNumMeasurePoints < NumMeasurePoints);
    
    //HAL_Delay(20);
    HAL_TIM_Base_Stop_IT(&TIM4_Handler);      //�ر�
    
    for(i=0; i<NumMeasurePoints; i++){
        temp = (float)OrginalV[i]*(3.3/4096.0);		    //�õ�ADC��ѹֵ(��������ʵֵ������軻Ϊת����ʵֵ)
        temp = (temp - 1.44)/2.32*5.0 * 1000.0;          //�����õ�·�й�
        True_mV[i] = temp;
        if(max_mV < True_mV[i]){
            max_mV = temp;
        }
        if(min_mV > True_mV[i]){
            min_mV = temp;
        }
        //sum += temp;
    }

    //Vpp
    Vpp_measured = max_mV - min_mV;
    //F, Hz
    F_measured = wave_frequency_calculate();
    
    //ȡ����
    sum = 0.0;
    static int nT = 1;
    int nTPoints;
    float max_d = 0;
    nTPoints = (int)((float)sampleF /F_measured )*nT;
    for(i=0; i< nTPoints && i<NumMeasurePoints ; i++){
        sum += True_mV[i];  
        if(max_d < fabs(True_mV[i]-True_mV[i-1]) && i>0) {
            max_d = fabs(True_mV[i]-True_mV[i-1]);
        }
    }
    //offSet
    offSet = sum / (float)(nTPoints) ; 

    //ռ�ձ� (���ǿ��MODE == 3����ɲ�)
    if(MODE == 3) {
        DR_measured = (offSet-min_mV)/Vpp_measured;
    }
    
    //�����ж�
    //�����ų�����:��ƫ�õ�ѹ
    if ( Vpp_measured - max_d < Vpp_measured/2.0){ 
        MODE = 3;
    }else {
        //�����б���
        sum = 0;
        for(i=0; i<nTPoints && i<NumMeasurePoints ; i++){
            sum += fabs( True_mV[i] - offSet );       
        }
        temp = (Vpp_measured*1.15)*(float)nTPoints/4.0;
        if( sum <= temp ) {
            //����
            MODE = 2;
        }else {
            //����
            MODE = 1;
        }
    
    }
    
    
}

