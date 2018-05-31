#include "led.h"

uint32_t LedCount = 0;
uint8_t LedcountFlag = 0;

/**
  * @Description: led��ʼ��
  * @param:  None
  * @return: None
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	LED_CLOCK_ENABLE();
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_LOW;
	
	/* LED0 */
	GPIO_Initure.Pin = PIN_LED0;
	HAL_GPIO_Init(PORT_LED0,&GPIO_Initure);
	
	/* ��ʼ�� */
	HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_SET);	//PB0��1 
	LedCount = 0;
	LedcountFlag = 1;
}
/**
  * @Description: led���еƷ�ת
  * @param:  None
  * @return: None
  */
void LedRun(void)
{
	if(LedcountFlag == 1){
		LedCount ++;
		if(LedCount > 100){
			if(HAL_GPIO_ReadPin(PORT_LED0, PIN_LED0)){
				HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_RESET);	//PB0��0
			}
			else{
				HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_SET);	//PB0��0 
			}
			LedCount = 0;
		}
	}   
}

/**
  * @Description: ledʹ��ɱ��
  * @param:  None
  * @return: None
  */
void ledUseKill(void)
{
	LED_CLOCK_DISABLE();
}

/* END */
