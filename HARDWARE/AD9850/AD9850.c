/**
 * @file AD9850.c
 * @author framist (framist@163.com)
 * @brief 同时支持AD9850，AD9851，默认为9850
 * @version 0.1
 * @date 2021-07-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "sys.h"
#include "ad9850.h"

#define AD9850_PORT GPIOF
#define AD9850_WCLK GPIO_PIN_1
#define AD9850_RESET GPIO_PIN_2
#define AD9850_FQUD GPIO_PIN_3 // Also called FQ_UD (Frequency Update)
#define AD9850_DATA GPIO_PIN_4


// static const float AD9850_REF = 125e6; //125MHz
static const float AD9850_REF = 180e6; //180MHz

void AD9850_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_Initure.Pin = AD9850_WCLK | AD9850_RESET | AD9850_DATA | AD9850_FQUD;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD9850_PORT, &GPIO_Initure);
}

void AD9850_Reset(void)
{
    // Set everything low first
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_WCLK,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_RESET,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_FQUD,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_DATA,GPIO_PIN_RESET);

    // Pulse reset
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_RESET,GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_RESET,GPIO_PIN_RESET);

    // Pulse clock
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_WCLK,GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_WCLK,GPIO_PIN_RESET);

    // Pulse load
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_FQUD,GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_FQUD,GPIO_PIN_RESET);
}

void AD9850_Write(uint8_t byte)
{
    uint8_t i;
    uint8_t bit;
    for(i = 0; i < 8; i++) {
        bit = ((byte >> i) & 1);
        if(bit == 1)
            HAL_GPIO_WritePin(AD9850_PORT,AD9850_DATA,GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(AD9850_PORT,AD9850_DATA,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AD9850_PORT,AD9850_WCLK,GPIO_PIN_SET);
        HAL_GPIO_WritePin(AD9850_PORT,AD9850_WCLK,GPIO_PIN_RESET);
    }
}

void AD9850_Update_Freq(float freq)
{
    // Updates DDS output frequency. Supply frequency in Hz.

    uint32_t tuning_word = (freq * 4294967295UL) / AD9850_REF;
    AD9850_Write(tuning_word & 0xFF);
    AD9850_Write((tuning_word >> 8) & 0xFF);
    AD9850_Write((tuning_word >> 16) & 0xFF);
    AD9850_Write((tuning_word >> 24) & 0xFF);
    AD9850_Write(1); //AD9851 开启6倍频：1;否则0:频率freq*=6

    HAL_GPIO_WritePin(AD9850_PORT,AD9850_FQUD,GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD9850_PORT,AD9850_FQUD,GPIO_PIN_RESET);
}
