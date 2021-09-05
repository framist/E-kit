#include "wave_output.h" 
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

//////////////////////////////////////////////////////////////////////////////////
// 此项目的控制输出 基于cube-mx生成
//////////////////////////////////////////////////////////////////////////////////

void SystemClock_Config(void);

#define N 99
uint16_t Sine12bit[N] = {1756, 1835, 1913, 1991, 2068, 2143, 2217, 2289, 2359, 2427, 2492, 2554, 2612, 2668, 2719, 2767,
                         2811, 2850, 2885, 2915, 2941, 2962, 2978, 2989, 2996, 2997, 2993, 2984, 2971, 2952, 2929, 2901, 2868, 2831, 2789, 2744,
                         2694, 2640, 2583, 2523, 2460, 2393, 2325, 2253, 2180, 2106, 2029, 1952, 1874, 1795, 1716, 1638, 1560, 1482, 1406, 1331,
                         1258, 1187, 1118, 1052, 989, 928, 871, 818, 768, 722, 681, 644, 611, 583, 560, 541, 527, 519, 515, 516, 522, 533, 550, 571,
                         596, 627, 662, 701, 745, 792, 844, 899, 958, 1020, 1085, 1152, 1222, 1295, 1369, 1444, 1521, 1599, 1677};

void Wave_Output_Init(void)
{

    float f = 1000; //对应输出频率，单位Hz
    uint16_t arr;       //对应重置装载

    //SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_DAC_Init();
    MX_TIM6_Init();
    
	HAL_TIM_Base_Start(&htim6);
    HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)Sine12bit,N,DAC_ALIGN_12B_R);
    // Tout=((arr+1)*(psc+1))/Ft us. Ft=定时器工作频率,单位:Mhz
    // psc=0 ; Ft=84 ; Tout/10^6=1/(N*f)
    arr = (uint16_t)(1000000 * 84 / (N * f) - 1); //已验证正确
    TIM6->ARR = arr;
    //HAL_DAC_Stop(&hdac,DAC_CHANNEL_1);
}
/**
 * @brief 
 * 
 * @param f 输出频率，单位Hz
 */
void Wave_Output_F(float f)
{
    TIM6->ARR = (uint16_t)(1000000 * 84 / (N * f) - 1);
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
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}
