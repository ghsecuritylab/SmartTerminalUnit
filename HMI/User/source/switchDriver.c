/**
  * @file   switchDriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/16
  * @brief  ������������
  */

#include "switchDriver.h"
#include "inoutUser.h"

/* ������Ϣ�ṹ�� */
struct SwitchInfo switchInfo;
/* ���ر仯����ص� */
void (* SwitchChangeDealWith)(uint8_t switchNum,uint8_t state);

/**
  *@brief  ����Ӳ����ʼ��
  *@param  None
  *@retval None
  */
static void SwitchGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SWITCH, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	/* switch0 */
	GPIO_InitStructure.GPIO_Pin = PIN_SWITCH0;
	GPIO_Init(PORT_SWITCH0, &GPIO_InitStructure);
	/* switch1 */
	GPIO_InitStructure.GPIO_Pin = PIN_SWITCH1;
	GPIO_Init(PORT_SWITCH1, &GPIO_InitStructure);
	/* switch2 */
	GPIO_InitStructure.GPIO_Pin = PIN_SWITCH2;
	GPIO_Init(PORT_SWITCH2, &GPIO_InitStructure);
	/* switch3 */
	GPIO_InitStructure.GPIO_Pin = PIN_SWITCH3;
	GPIO_Init(PORT_SWITCH3, &GPIO_InitStructure);
	/* switch4 */
	GPIO_InitStructure.GPIO_Pin = PIN_SWITCH4;
	GPIO_Init(PORT_SWITCH4, &GPIO_InitStructure);
	/* switch5 */
	GPIO_InitStructure.GPIO_Pin = PIN_SWITCH5;
	GPIO_Init(PORT_SWITCH5, &GPIO_InitStructure);
}

/**
  *@brief  ��ȡ����Ӳ��״̬
  *@param  switchNum ���غ�
  *@retval ����״̬
  */
static uint8_t GetSwitchStatus(uint8_t switchNum)
{
	uint8_t tSwitchStatus = 0;
	switch(switchNum){
		case 0:tSwitchStatus = GPIO_ReadInputDataBit(PORT_SWITCH0,PIN_SWITCH0);break;
		case 1:tSwitchStatus = GPIO_ReadInputDataBit(PORT_SWITCH1,PIN_SWITCH1);break;
		case 2:tSwitchStatus = GPIO_ReadInputDataBit(PORT_SWITCH2,PIN_SWITCH2);break;
		case 3:tSwitchStatus = GPIO_ReadInputDataBit(PORT_SWITCH3,PIN_SWITCH3);break;
		case 4:tSwitchStatus = GPIO_ReadInputDataBit(PORT_SWITCH4,PIN_SWITCH4);break;
		case 5:tSwitchStatus = GPIO_ReadInputDataBit(PORT_SWITCH5,PIN_SWITCH5);break;
		default:break;
	}
	return tSwitchStatus;
}

/**
  *@brief  ��ȡ���ص�ǰ״̬
  *@param  switchNum ���غ�
  *@retval ����״̬
  */
uint8_t GetCurrentSwitchStatus(uint8_t switchNum)
{
	if(switchNum < SWITCH_NUM_ALL){
		return switchInfo.currentStatus[switchNum];
	}
	return 0;
}

/**
  *@brief ����״̬ɨ��
  *@param  None
  *@retval None
  */
void ScanSwitchStatus(void)
{
	uint8_t i,tState;
	for(i = 0; i < SWITCH_NUM_ALL; i ++){//ѭ��ɨ��
		tState = GetSwitchStatus(i);
		if(tState != switchInfo.currentStatus[i] && switchInfo.filterFlag[i] == 0){//��������
			switchInfo.filterCount[i] = GetTimer1Tick();
			switchInfo.filterFlag[i] = 1;
		}
		else if(tState != switchInfo.currentStatus[i] && switchInfo.filterFlag[i] == 1){
			if(GetTimer1IntervalTick(switchInfo.filterCount[i]) > 15){//��ʵ��λ
				switchInfo.currentStatus[i] = tState;
				#if	USE_SWITCHCHANGEDEALWITCH			
				(SwitchChangeDealWith)(i,tState);
				#endif
				switchInfo.filterFlag[i] = 0;
			}
		}
		else if(switchInfo.filterFlag[i] == 1){//�󴥷�,
			switchInfo.filterFlag[i] = 0;
		}
	}
}

/**
  *@brief ���س�ʼ��
  *@param  None
  *@retval None
  */
void SwitchDriverInit(void)
{
	SwitchGpioInit();
	SwitchChangeDealWith = UserSwitchChangeDeal;
}

/* END */
