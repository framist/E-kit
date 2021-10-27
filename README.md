# E-kit

E-kit 一体化电子工具箱，STM32实现，示波器+函数发生器+幅频特性仪器...

![main](README/main.jpg)

![singal](README/singal.jpg)

![幅频特性](README/幅频特性.jpg)



*目前此项目更新暂缓*

*注意：工程采用GB2312编码*



## 基本功能 

* STM32F103 (停留在较旧的版本并停止更新，仅支持以下功能)

- [x] 基本示波器实现（包括波形判断，频率测量，峰峰值测量，占空比测量）
- [x] 任意频率正弦波输出
- [x] 多窗口支持
- [x] 集中式Log



* STM32F407

- [x] 基本示波器实现（包括波形判断，频率测量，峰峰值测量，占空比测量）

- [x] 任意频率正弦波输出

- [x] 多窗口支持

- [x] 集中式Log

- [x] 幅频特性仪移植

- [ ] 上位机通讯重构

- [x] 函数信号发生器

- [ ] 示波器触发模式

- [ ] 电池管理

  

---

# 设计文档

## 一、工作原理

### 1. 软件部分

采用HAL库支持的C语言程序实现，通过无操作系统的形式直接运行在ARM架构STM32F407单片机上。通过 ARM Cortex M4 额外支持的硬件FPU、DSP指令集，更快的FSMC，给予程序更高的运行效率、数据处理效率以及更高的显示速度。

程序流程通过初始化+主循环的结果，GUI交互回调相应的模式。稳定性好。显示方案采用STemWin图形库实现，并支持电阻屏触摸，自己移植的图形库配合HAL库的框架工程开源在[framist/STemWinForHAL: 移植emWin与HAL库结合。（正点原子项目结构） (github.com)](https://github.com/framist/STemWinForHAL)。

项目工程已开源在 [framist/E-kit: E-kit 一体化电子工具箱 (github.com)](https://github.com/framist/E-kit)

总体使用到的版级接口：

TFTLCD <--排线--> TFTLCD(>=3.5')

VREF  <-跳线帽-- 3.3V

PA5  <-------- adc输入

PA4  --------> dac输出

#### 1.1 示波器部分



通过通用定时器TIM5定时中断，控制ADC按精准的时间间隔取样。

采用DSP库中的FFT算法得出频率，作为后续处理的前提参考。微分法首先判别是否为方波，积分法判断波形和测占空比，同时还能实时测量信号的峰峰值、偏置电压等等。 

示波器还采用自动量程。通过当前计算的波形频率自动调整ADC的采样周期，已适应更大的频率范围

 

#### 1.2 信号发生器部分

使用DMA的方法把内存中的波形数据依次传递到DAC控制器，基本定时器TIM6控制DMA的速度来控制输出波形的频率。

通过软件控制波形、峰峰值、偏置电压、占空比。

DAC输出0~3.3V的电平信号再经过外围电路的转换得到-5V到+5V范围的信号并进行输出。



#### 1.3 幅频特性仪部分

比较输入输出信号的峰峰值可以计算得出滤波器的增益，

通过扫频的方式连续测量可以绘制出未知滤波器的幅频特性曲线。

通过幅频特性曲线可以判别出滤波器的类型，目前可自动判别以下几种类型的滤波器：

0：低通 HPF 1：高通 LPF 2：带通 BPF 3：带阻BRF 



#### 1.4 IOT上位机部分

蓝牙串口透传模块实现。可转换实现与上位机的无线信号传输。

通过其他各种数据分析软件，我们可以同步在上位机观察波形并进行数据分析

![image-20210914214415461](README/image-20210914214415461.png)

 

### 2. 硬件部分



#### 2.1电源部分

 2.1.USB   接口模块部分 

 2.1.2   充电蓄电模块部分 

 2.1.3   负压生成部分 

 

#### 2.2扩幅电路部分

 2.2.2   信号输入部分 

 2.2.2   信号输出部分 

 

##  二、    仪器制作过程 

###  1.   软件部分



####  1.1   主函数部分 

主函数主要实现 init 与 main loop。具体实现伪代码如下：

```c
int main(void)
{
    HAL_Init();                   	//初始化HAL库  
    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(9600);             	//初始化USART
    
    TIM3_Init(999,83); 	            //1KHZ 定时器3设置为1ms
    TIM4_Init(999,839);             //触摸屏扫描速度,100HZ.
	TIM5_Init(999,0);               //采样用
    Wave_Output_Init();             //初始化 dac-DMA-tim6 输出 
	LED_Init();						//初始化LED	
	KEY_Init();						//初始化KEY
	TFTLCD_Init();           	    //初始化LCD FSMC接口
    TP_Init();				        //触摸屏初始化
    RNG_Init();	 		            //初始化随机数发生器

    MY_ADC_Init();
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	my_mem_init(SRAMCCM);			//初始化CCM内存池
    
    __HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟，否则STemWin不能使用

	WM_SetCreateFlags(WM_CF_MEMDEV);//为重绘操作自动使用存储设备
	GUI_Init();
    GUI_CURSOR_Show();

    CreatemainFramewin();
    
    mainLogPrint("\ninit OK!");
    
    while(1)
	{
		GUI_Delay(100); 
        GUI_Exec();
        
        //测量
        Measure();
        //绘制
        True_mV_To_aPoints();
        plot_aPoint(hWin_oscilloscopeFramewin);
        //显示数据
        refresh_Measure(hWin_oscilloscopeFramewin);
	} 
}


```





####  1.2   屏幕部分 

屏幕采用 3.5' TFTLCD 作显示，并可作为电阻屏输入触摸，支持手写笔。

1.2.1 初始化

 ![image-20210914224600397](README/image-20210914224600397.png)

先生成一个主窗口框架 mainFramewin ，再在此窗口框架中通过 Multipage 窗口小组件生成四页选项卡，每页分别包含一个窗口。

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





1.2.2 图形界面

 按功能分别包含4个子窗口，功能分别作示波器、波形发生器、幅频特性测试仪、系统日志的窗口：



![示波器UI](README/image-20210730202408105.png)

![波形发生器](README/image-20210907223205490.png)



![幅频特性UI](README/image-20210730202505206.png)



 

日志窗口只是一个输出日志的文本框。



#### 1.3 信号获取与处理部分

 

1.3.1 ADC 获取数据

主要程序结构如下：

 ```c
 ADC_ChannelConfTypeDef ADC1_ChanConf;
 ADC1_ChanConf.Channel=ADC_CHANNEL_5;                        //通道 5 (注意)
 ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
 ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_3CYCLES;         //采样时间    改为最快           
 HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
 HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
 //定时器溢出频率计算: F = Ft/((arr+1)*(psc+1)) us.
 // arr  ; psc = 0 ; 1ms
 //Ft=定时器工作频率,F1:72Mhz; F4:84Mhz
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
 
 HAL_TIM_Base_Stop_IT(&TIM5_Handler);      //关闭
 
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
     if(htim==(&TIM5_Handler)){
         //定时单次取样
         if(iNumMeasurePoints >= NumMeasurePoints) return;
         //返回最近一次ADC1规则组的转换结果; 4095-值 是因为外置电路的妥协
         OrginalV[iNumMeasurePoints] = 4095 - (u16)HAL_ADC_GetValue(&ADC1_Handler);     
         iNumMeasurePoints++;        
     }
 }
 ```



1.3.2 FFT 处理数据

> STM32F4 采用 Cortex-M4 内核，相比 Cortex-M3 系列除了内置硬件 FPU 单元，在数字信号处理方面还增加了 DSP 指令集，支持诸如单周期乘加指令（MAC），优化的单指令多数据指令（SIMD），饱和算数等多种数字信号处理指令集。相比 Cortex-M3，Cortex-M4 在数字信号处 理能力方面得到了大大的提升。Cortex-M4 执行所有的 DSP 指令集都可以在单周期内完成，而 Cortex-M3 需要多个指令和多个周期才能完成同样的功能。

主要采用

```c
void arm_cfft_radix4_f32(
    const arm_cfft_radix4_instance_f32 * S,
    float32_t * pSrc);
void arm_cmplx_mag_f32(
    float32_t * pSrc,
    float32_t * pDst,
    uint32_t numSamples);
```



完成FFT计算。（但主要这两个函数目前不再推荐使用）

 主要程序结构如下：

```c
float wave_frequency_calculate(void)
{
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

```



1.3.3 信号显示

1.3.3.1 时域波形的显示

一次采样玩立即就刷新屏幕显示数据

1.3.3.2 幅频特性的显示

可采取线性显示和对数显示



#### 1.4 信号的产生与输出部分

1.4.1 DAC产生信号

DAC+TIM6+DMA 的配置由 cube-MX 配置生成。

1.4.1.1 DAC 的初始化

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



1.4.1.2 DMA 传输数据

 ```c
 /* DMA controller clock enable */
 __HAL_RCC_DMA1_CLK_ENABLE();
 
 /* DMA interrupt init */
 /* DMA1_Stream5_IRQn interrupt configuration */
 HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
 HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
 ```

1.4.1.3 TIM6 提供中断源

```c
htim6.Instance = TIM6;
htim6.Init.Prescaler = 0;
htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
htim6.Init.Period = 9999;
```



1.4.1.4 不同波形的产生

控制传入的枚举类型以及其他参数可以方便的配置输出

```c
enum Wave_Form
{
  Wave_Form_NA,     
  Wave_Form_SIN,      //正弦波  ~~~~
  Wave_Form_TRI,      //三角波  VVVV
  Wave_Form_SQU,      //方波    _||_
  Wave_Form_SAW_1,    //锯齿波1 /|/|  
  Wave_Form_SAW_2,    //锯齿波2 |\|\|  
  Wave_Form_RAD,      //噪波    ????
  Wave_Form_DC        //直流    ----
};
```

```c
/**
 * @brief Wave_Output_Config
 * 
 * @param Output_Wave_Form 类型：enum Wave_Form 
 * @param f 单位 Hz
 * @param Vpp  单位 V
 * @param offset  单位 V
 * @param duty 单位 %
 * @return int 
 */
int Wave_Output_Config(enum Wave_Form Output_Wave_Form, float f, float Vpp, float offset, int duty)
```

 

 

1.4.2上位机传输数据

1.4.2.1无线传输模块

采用蓝牙串口透传。

1.4.2.2 上位机接收

有蓝牙的设备都可以通过蓝牙的串口信号接收，包括手机电脑等。

 

### 2. 硬件部分

1.1  电路设计

1.1.1 电源部分

 

1.1.2 扩幅电路部分

 

1.2  PCB 设计

 

1.3  电子元件的焊接

 

 

## 三、实验仪器的应用

### 1. 整体说明

在转置的前端是显示屏幕，可以直接在屏幕上直接触摸来对各种参数进行设置，旁边分别是信号的输入和输出端口。在装置的后端是USB电源接口和指示灯。

 

### 2. 示波器功能

![main](README/main.jpg)



当信号经由输入端口进入仪器以后即可在屏幕上得到其波形，将对其波形进行识别并做出判断。同时显示出其峰峰值、频率，如果是自动判断为方波将自动测量占空比，或者手动点击`DR Measure`按钮也可以自动测量占空比。

对于波形显示，可以对其水平灵敏度和垂直灵敏度进行调节，也可以点击`Stop/Run`进行波形冻结。



 

### 3. 信号发生器功能



![singal](README/singal.jpg)

**输出波形选择：**

可以选择输出：

* OFF,      不启用DAC输出功能
* SIN,      正弦波  ~~~~
* TRI,      三角波  VVVV
* SQU,      方波    _||_
* SAW_1,    锯齿波1 /|/|  
* SAW_2,    锯齿波2 |\|\|  
* RAD,      噪波    ????
* DC,       直流    ----


**捷径配置选择：**

控制在示波器界面的捷径控制框可以改变的参数。



**输出波形参数配置：**

并且可以方便实时地改变 占空比、峰峰值、偏置电压、占空比（如果输出是方波）。



**调整步进配置：**

调整微调按钮的步进大小



**输出按钮：**

快速控制是否输出。也可通过输出按钮的颜色判断当前是否有输出。



### 4. 幅频特性仪功能

![幅频特性](README/幅频特性.jpg)



选择 `single measurement` 可测量滤波器在单个频率下的增益。

选择 `auto measurement` 会扫频并自动进行测量，并会绘制出滤波器的幅频特性曲线

勾选 `Log coordinate` 复选框可以以对数方式绘制幅频特性曲线



### 5. 上位机传输功能

 在示波器界面选择IOT复选框，可开启上位机传输功能，上传当前屏幕显示的波形，并与屏幕刷新同步。

 

## 四、  仪器性能指标分析

### 1. 仪器整体概况

1.1总体尺寸

长度：196.2mm

宽度：175.8mm

高度：68.6mm

1.2屏幕尺寸

长度：74.4mm

宽度：49.3mm

1.3仪器总重

### 2.示波器

2.1频率范围、误差、精度

频率范围：50Hz——100kHZ

频率精度：1Hz

频率误差：

 

2.2幅值范围、误差、精度

幅值范围：-5V——+5V

幅值精度：10mV

幅值误差：

 

2.3水平灵敏度、垂直灵敏度

水平灵敏度范围：1us/div~1000us/div可调

垂直灵敏度范围：50mV/div~500mV/div可调

垂直精度：2.4mV

水平精度：5us

 

 

### 3.信号发生器功能

3.1 频率范围、误差、精度

频率范围：20Hz——50kHZ

频率精度：1Hz

频率误差：

 

3.2 幅值范围、误差、精度

 

幅值范围：-5V——+5V

幅值精度：1mV

幅值误差：

 

3.3 可产生的波形类型

 

方波、三角波、正弦波、锯齿波、直流、噪声

 

### 4.幅频特性仪功能

4.1频率范围、误差、精度

在转置

4.2幅值范围、误差、精度

在转置

### 5.上位机传输功能

波特率：9600

 

## 附录

### 参考外围电路设计

from [yuexiavqiufeng](https://github.com/yuexiavqiufeng)

![image-20210915161056175](README/image-20210915161056175.png)









---



## 一些经验

移植cube-MX生成的工程时注意`stm32f4xx_it.c `文件也会生成一些代码



![image-20210906003413804](README/image-20210906003413804.png)
