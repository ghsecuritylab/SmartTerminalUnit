/**
  * @file   keyDriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/16
  * @brief  ������������
  */

#include "keyDriver.h"
#include "inoutUser.h"

/* ������Ϣ */
struct KeyInfo keyInfo;
/* �����仯����ص� */
void (* KeyChangeDealWith)(uint8_t keyNum,uint8_t state);

/**
  *@brief  ����Gpio��ʼ��
  *@param  keyNum ������
  *@retval ����״̬
  */
static void KeyGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_KEY, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	/* key0 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY0;
	GPIO_Init(PORT_KEY0, &GPIO_InitStructure);
	/* key1 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY1;
	GPIO_Init(PORT_KEY1, &GPIO_InitStructure);
	/* key2 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY2;
	GPIO_Init(PORT_KEY2, &GPIO_InitStructure);
	/* key3 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY3;
	GPIO_Init(PORT_KEY3, &GPIO_InitStructure);
	/* key4 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY4;
	GPIO_Init(PORT_KEY4, &GPIO_InitStructure);
	/* key5 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY5;
	GPIO_Init(PORT_KEY5, &GPIO_InitStructure);
	/* key6 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY6;
	GPIO_Init(PORT_KEY6, &GPIO_InitStructure);
	/* key7 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY7;
	GPIO_Init(PORT_KEY7, &GPIO_InitStructure);
	/* key8 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY8;
	GPIO_Init(PORT_KEY8, &GPIO_InitStructure);
	/* key9 */
	GPIO_InitStructure.GPIO_Pin = PIN_KEY9;
	GPIO_Init(PORT_KEY9, &GPIO_InitStructure);
}

/**
  *@brief  ��ȡ����Ӳ��״̬
  *@param  keyNum ������
  *@retval ����״̬
  */
static KeyStatus GetKeyStatus(uint8_t keyNum)
{
	uint8_t tKeyStatus = 0;
	switch(keyNum){
		case 0:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY0,PIN_KEY0);break;
		case 1:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY1,PIN_KEY1);break;
		case 2:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY2,PIN_KEY2);break;
		case 3:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY3,PIN_KEY3);break;
		case 4:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY4,PIN_KEY4);break;
		case 5:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY5,PIN_KEY5);break;
		case 6:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY6,PIN_KEY6);break;
		case 7:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY7,PIN_KEY7);break;
		case 8:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY8,PIN_KEY8);break;
		case 9:tKeyStatus = GPIO_ReadInputDataBit(PORT_KEY9,PIN_KEY9);break;
		default:break;
	}
	if(tKeyStatus == LED_HARDWARE_DOWN){
		return KEYDOWN;
	}
	else{
		return KEYUP;
	}
}

/**
  *@brief  ����ɨ��
  *@param  None
  *@retval None
  */
void ScanKeyStatus(void)
{
	uint8_t i;
	KeyStatus tState;
	
	for(i = 0; i < KEY_NUM_ALL; i ++){//ѭ��ɨ��
		tState = GetKeyStatus(i);
		if(tState == KEYDOWN && keyInfo.stepFlag[i] == 0){//��������
			keyInfo.filterCount[i] = GetTimer1Tick();
			keyInfo.stepFlag[i] = 1;
		}
		else if(tState == KEYDOWN && keyInfo.stepFlag[i] == 1){
			if(GetTimer1IntervalTick(keyInfo.filterCount[i]) > 20){//��ʵ��λ
				keyInfo.currentStatus[i] = KEYDOWN;
				KeyChangeDealWith(i,KEYDOWN);//�ص�
				keyInfo.stepFlag[i] = 2;
			}
		}
		else if(tState == KEYUP && keyInfo.stepFlag[i] != 0){//�ȴ�̧��
			keyInfo.currentStatus[i] = KEYUP;
			if(keyInfo.stepFlag[i] == 2){
				KeyChangeDealWith(i,KEYUP);//�ص�
			}
			keyInfo.stepFlag[i] = 0;
		}
	}
}

/**
  *@brief  ����������ʼ��
  *@param  None
  *@retval None
  */
void KeyDriverInit(void)
{
	KeyGpioInit();
	KeyChangeDealWith = UserKeyChangeDeal;
}
