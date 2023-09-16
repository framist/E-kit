<div align="center">
  <img width="100" heigth="100" src="./README/image-20210906003413804.png">
</div>


# E-kit

E-kit һ�廯���ӹ����䣬STM32 ʵ�֣�ʾ���� + ���������� + ��Ƶ��������...

![image-20220517134135846](README/image-20220517134135846.png)

![main](README/main.jpg)

![singal](README/singal.jpg)

![��Ƶ����](README/��Ƶ����.jpg)

*Ŀǰ����Ŀ�ѹ鵵*

*ע�⣺���̲��� GB2312 ����*

## �������� 

- STM32F103 (ͣ���ڽϾɵİ汾��ֹͣ���£���֧�����¹���)
  - [x] ����ʾ����ʵ�֣����������жϣ�Ƶ�ʲ��������ֵ������ռ�ձȲ�����
  - [x] ����Ƶ�����Ҳ����
  - [x] �ര��֧��
  - [x] ����ʽ Log
- STM32F407
  - [x] ����ʾ����ʵ�֣����������жϣ�Ƶ�ʲ��������ֵ������ռ�ձȲ�����
  - [x] ����Ƶ�����Ҳ����
  - [x] �ര��֧��
  - [x] ����ʽ Log
  - [x] ��Ƶ��������ֲ
  - [ ] ��λ��ͨѶ�ع�
  - [x] �����źŷ�����
  - [ ] ʾ��������ģʽ
  - [ ] ��ع���


# ����ĵ�

## һ������ԭ��

### 1. �������

���� HAL ��֧�ֵ� C ���Գ���ʵ�֣�ͨ���޲���ϵͳ����ʽֱ�������� ARM �ܹ� STM32F407 ��Ƭ���ϡ�ͨ�� ARM Cortex M4 ����֧�ֵ�Ӳ�� FPU��DSP ָ�������� FSMC�����������ߵ�����Ч�ʡ����ݴ���Ч���Լ����ߵ���ʾ�ٶȡ�

��������ͨ����ʼ�� + ��ѭ���Ľ����GUI �����ص���Ӧ��ģʽ���ȶ��Ժá���ʾ�������� STemWin ͼ�ο�ʵ�֣���֧�ֵ������������Լ���ֲ��ͼ�ο���� HAL ��Ŀ�ܹ��̿�Դ��[framist/STemWinForHAL: ��ֲ emWin �� HAL ���ϡ�������ԭ����Ŀ�ṹ�� (github.com)](https://github.com/framist/STemWinForHAL)��

��Ŀ�����ѿ�Դ�� [framist/E-kit: E-kit һ�廯���ӹ����� (github.com)](https://github.com/framist/E-kit)

����ʹ�õ��İ漶�ӿڣ�

```
TFTLCD <--����--> TFTLCD(>=3.5')

VREF  <-����ñ-- 3.3V

PA5  <-------- adc ����

PA4  --------> dac ���
```

#### 1.1 ʾ��������


ͨ��ͨ�ö�ʱ�� TIM5 ��ʱ�жϣ����� ADC ����׼��ʱ����ȡ����

���� DSP ���е� FFT �㷨�ó�Ƶ�ʣ���Ϊ���������ǰ��ο���΢�ַ������б��Ƿ�Ϊ���������ַ��жϲ��κͲ�ռ�ձȣ�ͬʱ����ʵʱ�����źŵķ��ֵ��ƫ�õ�ѹ�ȵȡ� 

ʾ�����������Զ����̡�ͨ����ǰ����Ĳ���Ƶ���Զ����� ADC �Ĳ������ڣ�����Ӧ�����Ƶ�ʷ�Χ

 

#### 1.2 �źŷ���������

ʹ�� DMA �ķ������ڴ��еĲ����������δ��ݵ� DAC ��������������ʱ�� TIM6 ���� DMA ���ٶ�������������ε�Ƶ�ʡ�

ͨ��������Ʋ��Ρ����ֵ��ƫ�õ�ѹ��ռ�ձȡ�

DAC ��� 0~3.3V �ĵ�ƽ�ź��پ�����Χ��·��ת���õ� -5V �� +5V ��Χ���źŲ����������



#### 1.3 ��Ƶ�����ǲ���

�Ƚ���������źŵķ��ֵ���Լ���ó��˲��������棬

ͨ��ɨƵ�ķ�ʽ�����������Ի��Ƴ�δ֪�˲����ķ�Ƶ�������ߡ�

ͨ����Ƶ�������߿����б���˲��������ͣ�Ŀǰ���Զ��б����¼������͵��˲�����

0����ͨ HPF 1����ͨ LPF 2����ͨ BPF 3������ BRF 



#### 1.4 IOT ��λ������

��������͸��ģ��ʵ�֡���ת��ʵ������λ���������źŴ��䡣

ͨ�������������ݷ�����������ǿ���ͬ������λ���۲첨�β��������ݷ���

![image-20210914214415461](README/image-20210914214415461.png)


### 2. Ӳ������


#### 2.1 ��Դ����

2.1.USB   �ӿ�ģ�鲿�� 

2.1.2   ������ģ�鲿�� 

2.1.3   ��ѹ���ɲ��� 

 

#### 2.2 ������·����

2.2.2   �ź����벿�� 

2.2.2   �ź�������� 

 

##  ���������������� 

###  1.   �������

####  1.1   ���������� 

��������Ҫʵ�� init �� main loop��

####  1.2   ��Ļ���� 

��Ļ���� 3.5' TFTLCD ����ʾ��������Ϊ���������봥����֧����д�ʡ�

1.2.1 ��ʼ��

 ![image-20210914224600397](README/image-20210914224600397.png)

������һ�������ڿ�� mainFramewin�����ڴ˴��ڿ����ͨ�� Multipage ����С���������ҳѡ���ÿҳ�ֱ����һ�����ڡ�

```c
hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
MULTIPAGE_AddEmptyPage(hItem, 0, " oscilloscope ");
MULTIPAGE_AddEmptyPage(hItem, 0, " signal generator ");
MULTIPAGE_AddEmptyPage(hItem, 0, " A-F characteristics ");
MULTIPAGE_AddEmptyPage(hItem, 0, " Logs ");

WM_HWIN CreateoscilloscopeFramewin(void);
hWin_oscilloscopeFramewin = CreateoscilloscopeFramewin(); 
MULTIPAGE_AttachWindow(hItem,0,hWin_oscilloscopeFramewin);

WM_HWIN CreateSingalWindow(void);
hWin_SingalWindow = CreateSingalWindow();
MULTIPAGE_AttachWindow(hItem,1,hWin_SingalWindow);

WM_HWIN CreateAFCFramewin(void);
hWin_AFCFramewin = CreateAFCFramewin();
MULTIPAGE_AttachWindow(hItem,2,hWin_AFCFramewin);

WM_HWIN CreateLogWindow(void);
hWin_LogFramewin = CreateLogWindow();
MULTIPAGE_AttachWindow(hItem,3,hWin_LogFramewin);

MULTIPAGE_SelectPage(hItem,0);
```


1.2.2 ͼ�ν���

�����ֱܷ���� 4 ���Ӵ��ڣ����ֱܷ���ʾ���������η���������Ƶ���Բ����ǡ�ϵͳ��־�Ĵ��ڣ�

![ʾ���� UI](README/image-20210730202408105.png)

![���η�����](README/image-20210907223205490.png)


![��Ƶ���� UI](README/image-20210730202505206.png)


��־����ֻ��һ�������־���ı���


#### 1.3 �źŻ�ȡ�봦����

 

1.3.1 ADC ��ȡ����


1.3.2 FFT ��������

> STM32F4 ���� Cortex-M4 �ںˣ���� Cortex-M3 ϵ�г�������Ӳ�� FPU ��Ԫ���������źŴ����滹������ DSP ָ���֧�����絥���ڳ˼�ָ�MAC�����Ż��ĵ�ָ�������ָ�SIMD�������������ȶ��������źŴ���ָ������ Cortex-M3��Cortex-M4 �������źŴ� ����������õ��˴���������Cortex-M4 ִ�����е� DSP ָ��������ڵ���������ɣ��� Cortex-M3 ��Ҫ���ָ��Ͷ�����ڲ������ͬ���Ĺ��ܡ�

��Ҫ����

```c
void arm_cfft_radix4_f32(
    const arm_cfft_radix4_instance_f32 * S,
    float32_t * pSrc);
void arm_cmplx_mag_f32(
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t numSamples);
```



��� FFT ���㡣������Ҫ����������Ŀǰ�����Ƽ�ʹ�ã�

��Ҫ����ṹ���£�

```c
float wave_frequency_calculate(void)
{
    //��ʼ�� scfft �ṹ�壬�趨 FFT ��ز���
    arm_cfft_radix4_instance_f32 scfft;
    arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1); // TODO ��֪�����ϰ�λȡ����ԭ��
    //FFT ���㣨�� 4��
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

```



1.3.3 �ź���ʾ

1.3.3.1 ʱ���ε���ʾ

һ�β�����������ˢ����Ļ��ʾ����

1.3.3.2 ��Ƶ���Ե���ʾ

�ɲ�ȡ������ʾ�Ͷ�����ʾ



#### 1.4 �źŵĲ������������

1.4.1 DAC �����ź�

DAC+TIM6+DMA �������� cube-MX �������ɡ�

1.4.1.1 DAC �ĳ�ʼ��

```c
hdma_dac1.Instance = DMA1_Stream5;
hdma_dac1.Init.Channel = DMA_CHANNEL_7;
hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
hdma_dac1.Init.Mode = DMA_CIRCULAR;
hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH;
hdma_dac1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
```

1.4.1.2 DMA ��������

 ```c
 /* DMA controller clock enable */
 __HAL_RCC_DMA1_CLK_ENABLE();
 
 /* DMA interrupt init */
 /* DMA1_Stream5_IRQn interrupt configuration */
 HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
 HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
 ```

1.4.1.3 TIM6 �ṩ�ж�Դ

```c
htim6.Instance = TIM6;
htim6.Init.Prescaler = 0;
htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
htim6.Init.Period = 9999;
```

1.4.1.4 ��ͬ���εĲ���

���ƴ����ö�������Լ������������Է�����������

```c
enum Wave_Form
{
  Wave_Form_NA,     
  Wave_Form_SIN,      //���Ҳ�  ~~~~
  Wave_Form_TRI,      //���ǲ�  VVVV
  Wave_Form_SQU,      //����    _||_
  Wave_Form_SAW_1,    //��ݲ� 1 /|/|  
  Wave_Form_SAW_2,    //��ݲ� 2 |\|\|  
  Wave_Form_RAD,      //�벨    ????
  Wave_Form_DC        //ֱ��    ----
};
```

```c
/**
 * @brief Wave_Output_Config
 * 
 * @param Output_Wave_Form ���ͣ�enum Wave_Form 
 * @param f ��λ Hz
 * @param Vpp  ��λ V
 * @param offset  ��λ V
 * @param duty ��λ %
 * @return int 
 */
int Wave_Output_Config(enum Wave_Form Output_Wave_Form, float f, float Vpp, float offset, int duty)
```


1.4.2 ��λ����������

1.4.2.1 ���ߴ���ģ��

������������͸����

1.4.2.2 ��λ������

���������豸������ͨ�������Ĵ����źŽ��գ������ֻ����Եȡ�

 

### 2. Ӳ������

�ο���Χ��·���

from [yuexiavqiufeng](https://github.com/yuexiavqiufeng)

![image-20210915161056175](README/image-20210915161056175.png)

## ����ʵ��������Ӧ��

### 1. ����˵��

��ת�õ�ǰ������ʾ��Ļ������ֱ������Ļ��ֱ�Ӵ������Ը��ֲ����������ã��Ա߷ֱ����źŵ����������˿ڡ���װ�õĺ���� USB ��Դ�ӿں�ָʾ�ơ�

 

### 2. ʾ��������

![main](README/main.jpg)


���źž�������˿ڽ��������Ժ󼴿�����Ļ�ϵõ��䲨�Σ������䲨�ν���ʶ�������жϡ�ͬʱ��ʾ������ֵ��Ƶ�ʣ�������Զ��ж�Ϊ�������Զ�����ռ�ձȣ������ֶ����`DR Measure`��ťҲ�����Զ�����ռ�ձȡ�

���ڲ�����ʾ�����Զ���ˮƽ�����Ⱥʹ�ֱ�����Ƚ��е��ڣ�Ҳ���Ե��`Stop/Run`���в��ζ��ᡣ

 

### 3. �źŷ���������



![singal](README/singal.jpg)

**�������ѡ��**

����ѡ�������

* OFF,      ������ DAC �������
* SIN,      ���Ҳ�  `~~~~`
* TRI,      ���ǲ�  `VVVV`
* SQU,      ����    `_||_`
* SAW_1,    ��ݲ� 1 `/|/|`  
* SAW_2,    ��ݲ� 2 `|\|\|`  
* RAD,      �벨    `????`
* DC,       ֱ��    `----`


**�ݾ�����ѡ��**

������ʾ��������Ľݾ����ƿ���Ըı�Ĳ�����



**������β������ã�**

���ҿ��Է���ʵʱ�ظı� ռ�ձȡ����ֵ��ƫ�õ�ѹ��ռ�ձȣ��������Ƿ�������



**�����������ã�**

����΢����ť�Ĳ�����С



**�����ť��**

���ٿ����Ƿ������Ҳ��ͨ�������ť����ɫ�жϵ�ǰ�Ƿ��������



### 4. ��Ƶ�����ǹ���

![��Ƶ����](README/��Ƶ����.jpg)



ѡ�� `single measurement` �ɲ����˲����ڵ���Ƶ���µ����档

ѡ�� `auto measurement` ��ɨƵ���Զ����в�����������Ƴ��˲����ķ�Ƶ��������

��ѡ `Log coordinate` ��ѡ������Զ�����ʽ���Ʒ�Ƶ��������



### 5. ��λ�����书��

��ʾ��������ѡ�� IOT ��ѡ�򣬿ɿ�����λ�����书�ܣ��ϴ���ǰ��Ļ��ʾ�Ĳ��Σ�������Ļˢ��ͬ����

 

## �ġ���������ָ�����

### 2.ʾ����

2.1 Ƶ�ʷ�Χ��������

Ƶ�ʷ�Χ��50Hz����100kHZ

Ƶ�ʾ��ȣ�1Hz

Ƶ����
 

2.2 ��ֵ��Χ��������

��ֵ��Χ��-5V����+5V

��ֵ���ȣ�10mV

��ֵ��


2.3 ˮƽ�����ȡ���ֱ������

ˮƽ�����ȷ�Χ��1us/div~1000us/div �ɵ�

��ֱ�����ȷ�Χ��50mV/div~500mV/div �ɵ�

��ֱ���ȣ�2.4mV

ˮƽ���ȣ�5us
 

### 3.�źŷ���������

3.1 Ƶ�ʷ�Χ��������

Ƶ�ʷ�Χ��20Hz����50kHZ

Ƶ�ʾ��ȣ�1Hz

Ƶ����

 

3.2 ��ֵ��Χ��������

 

��ֵ��Χ��-5V����+5V

��ֵ���ȣ�1mV

��ֵ��

 

3.3 �ɲ����Ĳ�������

 

���������ǲ������Ҳ�����ݲ���ֱ��������

 

### 4.��Ƶ�����ǹ���

4.1 Ƶ�ʷ�Χ��������

��ת��

4.2 ��ֵ��Χ��������

��ת��

### 5.��λ�����书��

�����ʣ�9600

---

![image-20210906003413804](README/image-20210906003413804.png)

[��ܿƹ�](https://craft.framist.top/) | ����Ϊ���޵���������һ�㻨��?
