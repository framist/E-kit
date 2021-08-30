#include "syn6288.h"
#include "string.h"
/***********************************************************
SYN6288 模块
UART_WORDLENGTH_8B  串口数据为8位
UART_PARITY_NONE    无校验位
**********************************************************/

UART_HandleTypeDef uartHandler;

/**
 * @brief SYN6288_init必须在串口初始化9600波特率之后，设置其他的波特率后需要重新更改串口波特率
 * 例如
 * ```
 * uart_init(9600);					//初始化串口
 * SYN6288_init(UART1_Handler,19200);  //初始化语音模块
 * uart_init(19200);					//初始化串口
 * ```
 * 
 * @param UARTx_Handler 
 * @param bound 波特率，只能是9600/19200
 */
void SYN6288_init(UART_HandleTypeDef UARTx_Handler, unsigned int bound){
	//选择串口
	uartHandler = UARTx_Handler; 
	//unsigned char Frame_9600[] = {0xfd, 0x00, 0x03, 0x31, 0x00, 0xcf};
	unsigned char Frame_19200[] = {0xfd, 0x00, 0x03, 0x31, 0x01, 0xce};

	if(uartHandler.Init.BaudRate == 9600){
		if(bound == 19200) {
			HAL_UART_Transmit(&uartHandler, Frame_19200, sizeof(Frame_19200), 0xFFFF);
		}else{
			while (1);
		}
	}else{
		while (1);//未选择正确的串口
	}
}

/**
 * @brief YS-SYN6288 文本合成函数
 *        发送一段语音文本
 * 
 * @param VoiceText 
 */
int SYN6288_SendVoiceText(const char * VoiceText)
{
	//const char VoiceText[] = "sounda soundp soundq soundr";
	unsigned char textLength = strlen(VoiceText); //长度 <=200
	if(textLength > 200){
		return -1;
	}

	unsigned char ecc = 0; //定义校验字节
	unsigned int i = 0;
	unsigned char Frame_Info[5] = {
		0xfd, //帧头
		0x00, textLength + 3, //数据区长度<=203
		0x01, //语音合成播报命令
		0x00  //GB2312编码
	};

	// 校验码计算：异或校验 
	for (i = 0; i < 5; i++) { ecc = ecc ^ (Frame_Info[i]); }
	for (i = 0; i < textLength; i++) { ecc = ecc ^ (VoiceText[i]); }

	//不能在近期用串口！！HAL库先发送5个，再发送文字，再ecc(校验位)
	HAL_UART_Transmit(&uartHandler, (uint8_t *)Frame_Info, 5, 0xFFFF);//依次发送构造好的5个帧头字节
	HAL_UART_Transmit(&uartHandler, (uint8_t *)VoiceText, textLength, 0xFFFF);//依次发送待合成的文本数据
	HAL_UART_Transmit(&uartHandler, (uint8_t *)&ecc, 1, 0xFFFF);
    
	return 0;
}
