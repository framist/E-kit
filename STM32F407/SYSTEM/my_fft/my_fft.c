/**
 * @file my_fft.c
 * @author framist (framist@163.com)
 * @brief fft����
 *          �˹�����sampleָ"����"
 * @version 0.1
 * @date 2021-09-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "main.h"
#include "math.h"
#include "arm_math.h"  

#define FFT_LENGTH		256 		//����������FFT���ȣ�Ĭ����1024��FFT
// TODO �����������ֲ�����ʱ�� ���� HardFault_Handler �����ݲ�֪��ԭ��
float fft_inputbuf[FFT_LENGTH*2];	//FFT�������� 0.ʵ�� 1.�鲿 ...
float fft_outputbuf[FFT_LENGTH];	//FFT�������

/**
 * @brief ����Ƶ�ʼ��㺯��
 * �õ�ȫ�ֱ���:
 * pas, arr, OrginalV, ��ȡ��������أ�
 * F_measured: �޸�
 * @return float ����Ƶ��
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
        fft_inputbuf[2 * i] = (float)OrginalV[i * k]; //ʵ���������Ǿ�����ֵ��������ֵת��
        fft_inputbuf[2 * i + 1] = 0;                  //�鲿ȫ��Ϊ0
    }

    //��ʼ��scfft�ṹ�壬�趨FFT��ز���
    arm_cfft_radix4_instance_f32 scfft;
    arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1); // TODO ��֪�����ϰ�λȡ����ԭ��
    //FFT���㣨��4��
    arm_cfft_radix4_f32(&scfft,fft_inputbuf);	
    //��������������ģ�÷�ֵ 
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
