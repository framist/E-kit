#include "wave_output.h" 
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

//////////////////////////////////////////////////////////////////////////////////
// 此项目的控制输出 基于cube-mx生成
// STM32F407ZGT6 的数据手册查到 的典型值为 3us，最大是 6us。所以 DAC 的转换速度最快是 333K 左右。
//////////////////////////////////////////////////////////////////////////////////

void SystemClock_Config(void);



//DMA用 预赋值仅测试
uint16_t DAC_DMA_Data[N_WaveData] = {1756, 1835, 1913, 1991, 2068, 2143, 2217, 2289, 2359, 2427, 2492, 2554, 2612, 2668, 2719, 2767,
                         2811, 2850, 2885, 2915, 2941, 2962, 2978, 2989, 2996, 2997, 2993, 2984, 2971, 2952, 2929, 2901, 2868, 2831, 2789, 2744,
                         2694, 2640, 2583, 2523, 2460, 2393, 2325, 2253, 2180, 2106, 2029, 1952, 1874, 1795, 1716, 1638, 1560, 1482, 1406, 1331,
                         1258, 1187, 1118, 1052, 989, 928, 871, 818, 768, 722, 681, 644, 611, 583, 560, 541, 527, 519, 515, 516, 522, 533, 550, 571,
                         596, 627, 662, 701, 745, 792, 844, 899, 958, 1020, 1085, 1152, 1222, 1295, 1369, 1444, 1521, 1599, 1677, 0}; //最后一个数字仅为填充至100用
// -1~1 确定波形占空比 缩放平移之前
float FormData[N_WaveData];

//正弦波 -1~1
float SinWaveData[N_WaveData] = {0.00000000e+00,  6.27905195e-02,  1.25333234e-01,  1.87381315e-01,
                                2.48689887e-01,  3.09016994e-01,  3.68124553e-01,  4.25779292e-01,
                                4.81753674e-01,  5.35826795e-01,  5.87785252e-01,  6.37423990e-01,
                                6.84547106e-01,  7.28968627e-01,  7.70513243e-01,  8.09016994e-01,
                                8.44327925e-01,  8.76306680e-01,  9.04827052e-01,  9.29776486e-01,
                                9.51056516e-01,  9.68583161e-01,  9.82287251e-01,  9.92114701e-01,
                                9.98026728e-01,  1.00000000e+00,  9.98026728e-01,  9.92114701e-01,
                                9.82287251e-01,  9.68583161e-01,  9.51056516e-01,  9.29776486e-01,
                                9.04827052e-01,  8.76306680e-01,  8.44327926e-01,  8.09016994e-01,
                                7.70513243e-01,  7.28968627e-01,  6.84547106e-01,  6.37423990e-01,
                                5.87785252e-01,  5.35826795e-01,  4.81753674e-01,  4.25779292e-01,
                                3.68124553e-01,  3.09016994e-01,  2.48689887e-01,  1.87381315e-01,
                                1.25333234e-01,  6.27905196e-02,  8.97931843e-11, -6.27905194e-02,
                                -1.25333233e-01, -1.87381314e-01, -2.48689887e-01, -3.09016994e-01,
                                -3.68124553e-01, -4.25779291e-01, -4.81753674e-01, -5.35826795e-01,
                                -5.87785252e-01, -6.37423990e-01, -6.84547106e-01, -7.28968627e-01,
                                -7.70513243e-01, -8.09016994e-01, -8.44327925e-01, -8.76306680e-01,
                                -9.04827052e-01, -9.29776486e-01, -9.51056516e-01, -9.68583161e-01,
                                -9.82287251e-01, -9.92114701e-01, -9.98026728e-01, -1.00000000e+00,
                                -9.98026728e-01, -9.92114701e-01, -9.82287251e-01, -9.68583161e-01,
                                -9.51056516e-01, -9.29776486e-01, -9.04827053e-01, -8.76306680e-01,
                                -8.44327926e-01, -8.09016994e-01, -7.70513243e-01, -7.28968628e-01,
                                -6.84547106e-01, -6.37423990e-01, -5.87785252e-01, -5.35826795e-01,
                                -4.81753674e-01, -4.25779292e-01, -3.68124553e-01, -3.09016995e-01,
                                -2.48689887e-01, -1.87381315e-01, -1.25333234e-01, -6.27905197e-02};

//锯齿波的上升阶段-1->1，可生成锯齿波、三角波
float SawWaveData[N_WaveData] = {-1.        , -0.97979798, -0.95959596, -0.93939394, -0.91919192,
                                -0.8989899 , -0.87878788, -0.85858586, -0.83838384, -0.81818182,
                                -0.7979798 , -0.77777778, -0.75757576, -0.73737374, -0.71717172,
                                -0.6969697 , -0.67676768, -0.65656566, -0.63636364, -0.61616162,
                                -0.5959596 , -0.57575758, -0.55555556, -0.53535354, -0.51515152,
                                -0.49494949, -0.47474747, -0.45454545, -0.43434343, -0.41414141,
                                -0.39393939, -0.37373737, -0.35353535, -0.33333333, -0.31313131,
                                -0.29292929, -0.27272727, -0.25252525, -0.23232323, -0.21212121,
                                -0.19191919, -0.17171717, -0.15151515, -0.13131313, -0.11111111,
                                -0.09090909, -0.07070707, -0.05050505, -0.03030303, -0.01010101,
                                0.01010101,  0.03030303,  0.05050505,  0.07070707,  0.09090909,
                                0.11111111,  0.13131313,  0.15151515,  0.17171717,  0.19191919,
                                0.21212121,  0.23232323,  0.25252525,  0.27272727,  0.29292929,
                                0.31313131,  0.33333333,  0.35353535,  0.37373737,  0.39393939,
                                0.41414141,  0.43434343,  0.45454545,  0.47474747,  0.49494949,
                                0.51515152,  0.53535354,  0.55555556,  0.57575758,  0.5959596 ,
                                0.61616162,  0.63636364,  0.65656566,  0.67676768,  0.6969697 ,
                                0.71717172,  0.73737374,  0.75757576,  0.77777778,  0.7979798 ,
                                0.81818182,  0.83838384,  0.85858586,  0.87878788,  0.8989899 ,
                                0.91919192,  0.93939394,  0.95959596,  0.97979798,  1.};

void Wave_Output_Start(void) {
    HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)DAC_DMA_Data,N_WaveData,DAC_ALIGN_12B_R);
}

void Wave_Output_Stop(void) {
    HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_1);
}

void Wave_Output_Init(void)
{
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_DAC_Init();
    MX_TIM6_Init();
    
    HAL_TIM_Base_Start(&htim6);
    
    Wave_Output_Config(Wave_Form_SIN,1000,5,0,-1);
}

/**
 * @brief 暂时使用
 * 
 * @param f 输出频率，单位Hz
 */
void Wave_Output_Config_F(float f) {
    // Tout=((arr+1)*(psc+1))/Ft us. Ft=定时器工作频率,单位:Mhz
    // psc=0 ; Ft=84 ; Tout/10^6=1/(N_WaveData*f)
    TIM6->ARR = (uint16_t)(1000000 * 84 / (N_WaveData * f) - 1); //已验证正确
}

//确定波形占空比
void Wave_Output_Config_Form(enum Wave_Form Output_Wave_Form, int duty)
{
    int i;
    //输出波形,Vpp,offset
    switch (Output_Wave_Form) {
    case Wave_Form_NA:
        Wave_Output_Stop();
        break;
    case Wave_Form_SIN:
        for ( i = 0; i < N_WaveData; i++) {
            FormData[i] = SinWaveData[i];
        }
        break;
    case Wave_Form_TRI:
        // TODO 此处有边界点误差
        for ( i = 0; i < N_WaveData/2; i++) {
            FormData[i] = SawWaveData[i*2];
            FormData[N_WaveData-i-1] = SawWaveData[i*2];
        }
        break;
    case Wave_Form_SQU:
        for ( i = 0; i < duty; i++) {
            FormData[i] = 1;
        }
        for ( i = duty; i < N_WaveData; i++) {
            FormData[i] = -1;
        }
        break;
    case Wave_Form_SAW_1:
        for ( i = 0; i < N_WaveData; i++) {
            FormData[i] = SawWaveData[i];
        }
        break;
    case Wave_Form_SAW_2:
        for ( i = 0; i < N_WaveData; i++) {
            FormData[i] = SawWaveData[N_WaveData-i-1];
        }
        break;
    case Wave_Form_RAD:
        // TODO
        break;
    case Wave_Form_DC:
        for ( i = 0; i < N_WaveData; i++) {
            FormData[i] = 0;
        }
        break;
    default:
        break;
    }
    Wave_Output_Config_VppOffset(5, 0); //Test
}

/**
 * @brief 缩放平移占空比
 * 
 * @param Vpp 单位 V
 * @param offset 单位 V
 */
void Wave_Output_Config_VppOffset(float Vpp, float offset)
{
    int i;
    float transTrue;
    int temp;
    for ( i = 0; i < N_WaveData; i++) {
        transTrue = FormData[i] * Vpp + offset;
        //5.0f : 外置电路最大电压
        temp = (int)(transTrue/5.0f*2048.0f+2048.0f); //0位不精确
        temp = temp > 4095 ? 4095 : temp;
        temp = temp < 0 ? 0 : temp;
        DAC_DMA_Data[i] = (uint16_t)(temp); 
    }
}

int Wave_Output_Config(enum Wave_Form Output_Wave_Form, float f, float Vpp, float offset, int duty)
{

    Wave_Output_Stop();
    if(Output_Wave_Form == Wave_Form_NA) return -1;

    //输出频率
    Wave_Output_Config_F(f);

    //输出波形,Vpp,offset
    Wave_Output_Config_Form(Output_Wave_Form,duty);

    Wave_Output_Config_VppOffset(Vpp,offset);


    Wave_Output_Start();
    return 0;
}





/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

