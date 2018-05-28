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
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();           //����GPIOHʱ��,���Ź�
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PB0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    GPIO_Initure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOG,&GPIO_Initure);     //���Ź�
    
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0��1 
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);	//PH3��1  
    
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
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//PB0��0
    }
    else{
       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0��0 
    }
}

