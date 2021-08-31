#include "adc.h"
#include "delay.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////	 
//  my adc								  
////////////////////////////////////////////////////////////////////////////////// 	

ADC_HandleTypeDef ADC1_Handler;//ADC句柄

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=ENABLE;                //关闭连续转换 -> 开启,但精确度降低
    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //返回最近一次ADC1规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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


//按时间采样一轮，记录数据到全局数组
float wave_frequency_calculate(void);
//临时
float wave_frequency_calculate(void){
    return 10000;
}
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
    extern TIM_HandleTypeDef 	TIM5_Handler;      	//定时器4句柄 
    int i;
    float max_mV = -5000;
    float min_mV = 5000;
    float temp;
    float sum = 0;
    float offSet;
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    ADC1_ChanConf.Channel=ADC_CHANNEL_5;                        //通道 5 (注意)
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_3CYCLES;         //采样时间    改为最快           
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
    
//    for(i=0; i<NumMeasurePoints; i++){
//        OrginalV[i] = (u16)HAL_ADC_GetValue(&ADC1_Handler);     //返回最近一次ADC1规则组的转换结果
//    }
    
    //定时器溢出频率计算: F = Ft/((arr+1)*(psc+1)) us.
    // arr  ; psc = 0 ; 1ms
    //Ft=定时器工作频率,F1:72Mhz; F4:168Mhz
    // arr = Ft/F - 1   ; Fmax~200,000;一次取样5us
    //F 自动调节；
    if (sampleF == 200000 && F_measured<2000) {
        sampleF = 20000;
        us_div = 1000; //强制改变 us_div
    }else if (sampleF == 20000 && F_measured>1500) {
        sampleF = 200000;
        us_div = 100; //强制改变 us_div
    }
    TIM5->ARR = (uint16_t)(84000000/(sampleF) - 1); 
    iNumMeasurePoints = 0;
    HAL_TIM_Base_Start_IT(&TIM5_Handler);     //使能
    while(iNumMeasurePoints < NumMeasurePoints);
    
    //HAL_Delay(20);
    HAL_TIM_Base_Stop_IT(&TIM5_Handler);      //关闭
    
    for(i=0; i<NumMeasurePoints; i++){
        temp = (float)OrginalV[i]*(3.3f/4096.0f);		    //得到ADC电压值(现在是真实值，最后需换为转换真实值)
        //temp = (temp - 1.44f)/2.32f*5.0f * 1000.0f;          //跟外置电路有关
        temp = temp * 1000.0f;
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
    
    //取周期
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

    //占空比 (如果强制MODE == 3，则可测)
    if(MODE == 3) {
        DR_measured = (offSet-min_mV)/Vpp_measured;
    }
    
    //波形判断
    //首先排除方波:有偏置电压
    if ( Vpp_measured - max_d < Vpp_measured/2.0f){ 
        MODE = 3;
    }else {
        //积分判别波形
        sum = 0;
        for(i=0; i<nTPoints && i<NumMeasurePoints ; i++){
            sum += fabs( True_mV[i] - offSet );       
        }
        temp = (Vpp_measured*1.15f)*(float)nTPoints/4.0f;
        if( sum <= temp ) {
            //三角
            MODE = 2;
        }else {
            //正弦
            MODE = 1;
        }
    }
}

