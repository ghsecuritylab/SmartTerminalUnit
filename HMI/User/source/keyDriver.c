/**
  * @file   keyDriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/16
  * @brief  按键驱动程序
  */

#include "keyDriver.h"
#include "inoutUser.h"

/* 按键信息 */
struct KeyInfo keyInfo;
/* 按键变化处理回调 */
void (* KeyChangeDealWith)(uint8_t keyNum,uint8_t state);

/**
  *@brief  按键Gpio初始化
  *@param  keyNum 按键号
  *@retval 按键状态
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
  *@brief  获取按键硬件状态
  *@param  keyNum 按键号
  *@retval 按键状态
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
  *@brief  按键扫描
  *@param  None
  *@retval None
  */
void ScanKeyStatus(void)
{
	uint8_t i;
	KeyStatus tState;
	
	for(i = 0; i < KEY_NUM_ALL; i ++){//循环扫描
		tState = GetKeyStatus(i);
		if(tState == KEYDOWN && keyInfo.stepFlag[i] == 0){//启动消抖
			keyInfo.filterCount[i] = GetTimer1Tick();
			keyInfo.stepFlag[i] = 1;
		}
		else if(tState == KEYDOWN && keyInfo.stepFlag[i] == 1){
			if(GetTimer1IntervalTick(keyInfo.filterCount[i]) > 20){//真实变位
				keyInfo.currentStatus[i] = KEYDOWN;
				KeyChangeDealWith(i,KEYDOWN);//回调
				keyInfo.stepFlag[i] = 2;
			}
		}
		else if(tState == KEYUP && keyInfo.stepFlag[i] != 0){//等待抬起
			keyInfo.currentStatus[i] = KEYUP;
			if(keyInfo.stepFlag[i] == 2){
				KeyChangeDealWith(i,KEYUP);//回调
			}
			keyInfo.stepFlag[i] = 0;
		}
	}
}

/**
  *@brief  按键驱动初始化
  *@param  None
  *@retval None
  */
void KeyDriverInit(void)
{
	KeyGpioInit();
	KeyChangeDealWith = UserKeyChangeDeal;
}
