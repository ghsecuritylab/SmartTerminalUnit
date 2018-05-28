/**
  * @file   ledDriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   �źŵ�Ӳ������
  */

#include "ledDriver.h"

static uint8_t LedRunFlag;
/**
  *@brief �źŵ�Ӳ����ʼ��
  *@param  None
  *@retval None
  */
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	/* ��Դ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  *@brief ȫ�ֱ�����ʼ��
  *@param  None
  *@retval None
  */
static void variableInit(void)
{
	LED_POWER_ON();
	LedRunFlag = 1;
}

/**
  *@brief �ܳ�ʼ���������ⲿ�ļ�����
  *@param  None
  *@retval None
  */
void LedInit(void)
{
	GPIOInit();
	variableInit();
}

/**
  *@brief �����źŵƿ���
  *@param  None
  *@retval None
  */
void LedRun(void)
{
	static uint16_t LedRunCount;
	if(LedRunFlag == 1){
		LedRunCount ++;
		if(LedRunCount >= 1000){
			if(LED_RUN_GETSTATUS() == 1){
				LED_RUN_ON();
			}
			else{
				LED_RUN_OFF();
			}
			LedRunCount = 0;
		}
	}
}

void BootPinCheck(void)
{
	
}

/**
  *@brief ledGpio��ʼ��
  *@param  None
  *@retval None
  */
void LedGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	/* LED0 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED0;
	GPIO_Init(PORT_LED0, &GPIO_InitStructure);
	/* LED1 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED1;
	GPIO_Init(PORT_LED1, &GPIO_InitStructure);
	/* LED2 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED2;
	GPIO_Init(PORT_LED2, &GPIO_InitStructure);
	/* LED3 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED3;
	GPIO_Init(PORT_LED3, &GPIO_InitStructure);
	/* LED4 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED4;
	GPIO_Init(PORT_LED4, &GPIO_InitStructure);
	/* LED5 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED5;
	GPIO_Init(PORT_LED5, &GPIO_InitStructure);
	/* LED6 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED6;
	GPIO_Init(PORT_LED6, &GPIO_InitStructure);
	/* LED7 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED7;
	GPIO_Init(PORT_LED7, &GPIO_InitStructure);
	/* LED8 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED8;
	GPIO_Init(PORT_LED8, &GPIO_InitStructure);
	/* LED9 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED9;
	GPIO_Init(PORT_LED9, &GPIO_InitStructure);
	/* LED10 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED10;
	GPIO_Init(PORT_LED10, &GPIO_InitStructure);
}

/**
  *@brief ��ȡled��״̬
  *@param  ledNum �ƺ�
  *@param  ledIs led״̬
  *@retval None
  */
void SetLedState(uint8_t ledNum, LedStatus ledIs)
{
#if LED_HARDWARE_NO
	if(ledIs == LEDON)
#else
	if(ledIs == LEDOFF)
#endif
	{
		switch(ledNum){
			case 0:GPIO_SetBits(PORT_LED0, PIN_LED0);break;
			case 1:GPIO_SetBits(PORT_LED1, PIN_LED1);break;
			case 2:GPIO_SetBits(PORT_LED2, PIN_LED2);break;
			case 3:GPIO_SetBits(PORT_LED3, PIN_LED3);break;
			case 4:GPIO_SetBits(PORT_LED4, PIN_LED4);break;
			case 5:GPIO_SetBits(PORT_LED5, PIN_LED5);break;
			case 6:GPIO_SetBits(PORT_LED6, PIN_LED6);break;
			case 7:GPIO_SetBits(PORT_LED7, PIN_LED7);break;
			case 8:GPIO_SetBits(PORT_LED8, PIN_LED8);break;
			case 9:GPIO_SetBits(PORT_LED9, PIN_LED9);break;
			case 10:GPIO_SetBits(PORT_LED10, PIN_LED10);break;
			default:break;
		}
	}
#if LED_HARDWARE_OFF
	else if(ledIs == LEDON)
#else
	else if(ledIs == LEDOFF)
#endif
	{
		switch(ledNum){
			case 0:GPIO_ResetBits(PORT_LED0, PIN_LED0);break;
			case 1:GPIO_ResetBits(PORT_LED1, PIN_LED1);break;
			case 2:GPIO_ResetBits(PORT_LED2, PIN_LED2);break;
			case 3:GPIO_ResetBits(PORT_LED3, PIN_LED3);break;
			case 4:GPIO_ResetBits(PORT_LED4, PIN_LED4);break;
			case 5:GPIO_ResetBits(PORT_LED5, PIN_LED5);break;
			case 6:GPIO_ResetBits(PORT_LED6, PIN_LED6);break;
			case 7:GPIO_ResetBits(PORT_LED7, PIN_LED7);break;
			case 8:GPIO_ResetBits(PORT_LED8, PIN_LED8);break;
			case 9:GPIO_ResetBits(PORT_LED9, PIN_LED9);break;
			case 10:GPIO_ResetBits(PORT_LED10, PIN_LED10);break;
			default:break;
		}
	}
}

/**
  *@brief ��ȡled��״̬
  *@param  ledNum �ƺ�
  *@retval led״̬
  */
LedStatus GetLedStatus(uint8_t ledNum)
{
	uint8_t tledstatus = 2;
	switch(ledNum){
		case 0:tledstatus = GPIO_ReadOutputDataBit(PORT_LED0, PIN_LED0);break;
		case 1:tledstatus = GPIO_ReadOutputDataBit(PORT_LED1, PIN_LED1);break;
		case 2:tledstatus = GPIO_ReadOutputDataBit(PORT_LED2, PIN_LED2);break;
		case 3:tledstatus = GPIO_ReadOutputDataBit(PORT_LED3, PIN_LED3);break;
		case 4:tledstatus = GPIO_ReadOutputDataBit(PORT_LED4, PIN_LED4);break;
		case 5:tledstatus = GPIO_ReadOutputDataBit(PORT_LED5, PIN_LED5);break;
		case 6:tledstatus = GPIO_ReadOutputDataBit(PORT_LED6, PIN_LED6);break;
		case 7:tledstatus = GPIO_ReadOutputDataBit(PORT_LED7, PIN_LED7);break;
		case 8:tledstatus = GPIO_ReadOutputDataBit(PORT_LED8, PIN_LED8);break;
		case 9:tledstatus = GPIO_ReadOutputDataBit(PORT_LED9, PIN_LED9);break;
		case 10:tledstatus = GPIO_ReadOutputDataBit(PORT_LED10, PIN_LED10);break;
		default:tledstatus = 2;break;
	}
	if(tledstatus == LED_HARDWARE_NO){
		return LEDON;
	}
	else if(tledstatus == LED_HARDWARE_OFF){
		return LEDOFF;
	}
	else{
		return LEDERROR;
	}
}

void LedDriverInit(void)
{
	LedGpioInit();
}
/* END */
