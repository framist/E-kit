#define  NPT  256         /* 采样点数 */
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"

static uint32_t lBufOutArray[NPT/2]; /* FFT 运算的输出数组 */
static uint32_t lBufMagArray[NPT/2]; /* 各次谐波的幅值 */




void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
		//除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
			
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}
int adc_sample(void)
{
    uint32_t lBufInArray[NPT];    /* FFT 运算的输入数组 */
    static int i;
    extern uint16_t OrginalV[];
    extern float F_measured;
    int k;
    if(F_measured<10000){
        k = 4; //?????????
    }else {
        k = 1;
    }
	for (i = 0; i < NPT; i++)
	{
		lBufInArray[i] = (uint32_t)OrginalV[i*k];
	}
	cr4_fft_256_stm32(lBufOutArray,lBufInArray, NPT);
	GetPowerMag();
    return k;
}



float wave_frequency_calculate(void)
{
	int i;
    int k;
    int sample_psc;
    int sample_arr;
    float wave_frequency;
    float sample_frequency;
    int frequency_max_position;
    sample_psc = 0;
    sample_arr = TIM4->ARR;
    k = adc_sample();
	uint32_t Max_Val=0;
			for(i=2;i<NPT/2;i++)
		{
			if(lBufMagArray[i]>Max_Val)
			{
				Max_Val=lBufMagArray[i];
				frequency_max_position=i;
			}
		}
	sample_frequency = 72000000.0 / (float)((sample_arr + 1) * (sample_psc + 1)) / (float)k;
    wave_frequency = (float)frequency_max_position * sample_frequency / NPT;
    return wave_frequency;
}
