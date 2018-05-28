#include "usart.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
#include "IAP.h"
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���   

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((UART5->SR&0X40)==0);//ѭ������,ֱ���������   
	UART5->DR = (u8) ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   //���ʹ���˽���  	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART5_Handler; //UART���

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	//UART ��ʼ������
	__HAL_RCC_UART5_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
						
	/**USART1 GPIO Configuration    
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX 
				*/
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
				
	

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	UART5_Handler.Instance = UART5;
	UART5_Handler.Init.BaudRate = 115200;
	UART5_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	UART5_Handler.Init.StopBits = UART_STOPBITS_1;
	UART5_Handler.Init.Parity = UART_PARITY_NONE;
	UART5_Handler.Init.Mode = UART_MODE_TX_RX;
	UART5_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART5_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART5_Handler);
	
	__HAL_UART_DISABLE_IT(&UART5_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_RXNE);//ʹ���ж�
	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_IDLE);//ʹ���ж�
	HAL_NVIC_SetPriority(UART5_IRQn,3,2); //�������ȼ�
	HAL_NVIC_EnableIRQ(UART5_IRQn); //ʹ���ж�
}

UART_HandleTypeDef UART3_Handler; //UART���
void uart3_init(u32 bound)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
						
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		 
	UART3_Handler.Instance = USART3;
	UART3_Handler.Init.BaudRate = 115200;
	UART3_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	UART3_Handler.Init.StopBits = UART_STOPBITS_1;
	UART3_Handler.Init.Parity = UART_PARITY_NONE;
	UART3_Handler.Init.Mode = UART_MODE_TX_RX;
	UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART3_Handler);
	
	__HAL_UART_DISABLE_IT(&UART3_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);//ʹ���ж�
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_IDLE);//ʹ���ж�
	HAL_NVIC_SetPriority(USART3_IRQn,3,1); //�������ȼ�
	HAL_NVIC_EnableIRQ(USART3_IRQn); //ʹ���ж�
}

UART_HandleTypeDef USART2_Handler; //UART���
void uart2_init(u32 bound)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
						
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		 
	USART2_Handler.Instance = USART2;
	USART2_Handler.Init.BaudRate = 115200;
	USART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	USART2_Handler.Init.StopBits = UART_STOPBITS_1;
	USART2_Handler.Init.Parity = UART_PARITY_NONE;
	USART2_Handler.Init.Mode = UART_MODE_TX_RX;
	USART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART2_Handler);
	
	__HAL_UART_DISABLE_IT(&USART2_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&USART2_Handler,UART_IT_RXNE);//ʹ���ж�
	__HAL_UART_ENABLE_IT(&USART2_Handler,UART_IT_IDLE);//ʹ���ж�
	HAL_NVIC_SetPriority(USART2_IRQn,3,1); //�������ȼ�
	HAL_NVIC_EnableIRQ(USART2_IRQn); //ʹ���ж�
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	

}



extern uint8_t Rxbuffer[];
extern uint32_t RxBuffersize;
extern uint8_t rxfinish;

//����1�жϷ������
void UART5_IRQHandler(void)                	
{ 
	if((__HAL_UART_GET_FLAG(&UART5_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        Rxbuffer[RxBuffersize++] = (uint8_t)(UART5_Handler.Instance->DR & (uint8_t)0x00FF);   		 
	}
	if((__HAL_UART_GET_FLAG(&UART5_Handler,UART_FLAG_IDLE)!=RESET)){
		UART5_Handler.Instance->DR;
		rxfinish = 1;
	}
} 
void USART2_IRQHandler(void)                	
{ 
	if((__HAL_UART_GET_FLAG(&USART2_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        Rxbuffer[RxBuffersize++] = (uint8_t)(USART2_Handler.Instance->DR & (uint8_t)0x00FF);   		 
	}
	if((__HAL_UART_GET_FLAG(&USART2_Handler,UART_FLAG_IDLE)!=RESET)){
		USART2_Handler.Instance->DR;
		rxfinish = 1;
	}
}
extern uint8_t USART3_Rxbuffer[];
extern uint8_t USART3_RxBuffsize ;
extern uint8_t USART3_Rxfinish ;

void USART3_IRQHandler(void)                	
{ 
	if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET)) 
	{
        USART3_Rxbuffer[USART3_RxBuffsize++] = (uint8_t)(UART3_Handler.Instance->DR & (uint8_t)0x00FF);   		 
	}
	if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_IDLE)!=RESET)){
		UART3_Handler.Instance->DR;
		USART3_Rxfinish = 1;
	}
} 

#endif	



