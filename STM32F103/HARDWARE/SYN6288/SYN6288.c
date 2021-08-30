#include "syn6288.h"
#include "string.h"
/***********************************************************
SYN6288 ģ��
UART_WORDLENGTH_8B  ��������Ϊ8λ
UART_PARITY_NONE    ��У��λ
**********************************************************/

UART_HandleTypeDef uartHandler;

/**
 * @brief SYN6288_init�����ڴ��ڳ�ʼ��9600������֮�����������Ĳ����ʺ���Ҫ���¸��Ĵ��ڲ�����
 * ����
 * ```
 * uart_init(9600);					//��ʼ������
 * SYN6288_init(UART1_Handler,19200);  //��ʼ������ģ��
 * uart_init(19200);					//��ʼ������
 * ```
 * 
 * @param UARTx_Handler 
 * @param bound �����ʣ�ֻ����9600/19200
 */
void SYN6288_init(UART_HandleTypeDef UARTx_Handler, unsigned int bound){
	//ѡ�񴮿�
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
		while (1);//δѡ����ȷ�Ĵ���
	}
}

/**
 * @brief YS-SYN6288 �ı��ϳɺ���
 *        ����һ�������ı�
 * 
 * @param VoiceText 
 */
int SYN6288_SendVoiceText(const char * VoiceText)
{
	//const char VoiceText[] = "sounda soundp soundq soundr";
	unsigned char textLength = strlen(VoiceText); //���� <=200
	if(textLength > 200){
		return -1;
	}

	unsigned char ecc = 0; //����У���ֽ�
	unsigned int i = 0;
	unsigned char Frame_Info[5] = {
		0xfd, //֡ͷ
		0x00, textLength + 3, //����������<=203
		0x01, //�����ϳɲ�������
		0x00  //GB2312����
	};

	// У������㣺���У�� 
	for (i = 0; i < 5; i++) { ecc = ecc ^ (Frame_Info[i]); }
	for (i = 0; i < textLength; i++) { ecc = ecc ^ (VoiceText[i]); }

	//�����ڽ����ô��ڣ���HAL���ȷ���5�����ٷ������֣���ecc(У��λ)
	HAL_UART_Transmit(&uartHandler, (uint8_t *)Frame_Info, 5, 0xFFFF);//���η��͹���õ�5��֡ͷ�ֽ�
	HAL_UART_Transmit(&uartHandler, (uint8_t *)VoiceText, textLength, 0xFFFF);//���η��ʹ��ϳɵ��ı�����
	HAL_UART_Transmit(&uartHandler, (uint8_t *)&ecc, 1, 0xFFFF);
    
	return 0;
}
