/**
 * @file my_fft.c
 * @author framist (framist@163.com)
 * @brief fft计算
 *          此工程中sample指"采样"
 * @version 0.1
 * @date 2021-09-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "main.h"
#include "math.h"
#include "arm_math.h"  

#define FFT_LENGTH		256 		//采样点数，FFT长度，默认是1024点FFT
// TODO 以下两个做局部变量时会 进入 HardFault_Handler 错误，暂不知道原因
float fft_inputbuf[FFT_LENGTH*2];	//FFT输入数组 0.实部 1.虚部 ...
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组

/**
 * @brief 波形频率计算函数
 * 用到全局变量:
 * pas, arr, OrginalV, 读取，采样相关；
 * F_measured: 修改
 * @return float 波形频率
 */
float wave_frequency_calculate(void)
{
	int i;
    int k;
    int sample_psc;
    int sample_arr;
    float wave_frequency;
    float sample_frequency;
    int frequency_max_position;

    extern uint16_t OrginalV[];
    extern float F_measured;

    if (F_measured < 10000) {
        k = 4; //?????????
    } else {
        k = 1;
    }
    for (i = 0; i < FFT_LENGTH; i++) {
        fft_inputbuf[2 * i] = (float)OrginalV[i * k]; //实部，不考虑绝对数值，不做数值转换
        fft_inputbuf[2 * i + 1] = 0;                  //虚部全部为0
    }

    //初始化scfft结构体，设定FFT相关参数
    arm_cfft_radix4_instance_f32 scfft;
    arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1); // TODO 不知道加上按位取反的原理
    //FFT计算（基4）
    arm_cfft_radix4_f32(&scfft,fft_inputbuf);	
    //把运算结果复数求模得幅值 
    arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	

    uint32_t Max_Val = 0;
    for (i = 2; i < FFT_LENGTH / 2; i++){
        if (fft_outputbuf[i] > Max_Val){
            Max_Val = fft_outputbuf[i];
            frequency_max_position = i;
        }
    }

    sample_psc = 0;
    sample_arr = TIM5->ARR;
    sample_frequency = 84000000.0f / (float)((sample_arr + 1) * (sample_psc + 1)) / (float)k;
    wave_frequency = (float)frequency_max_position * sample_frequency / FFT_LENGTH;
    return wave_frequency;
}
