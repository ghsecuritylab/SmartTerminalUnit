/**
  * @file    keyDriver.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/16
  * @brief  
  */

#ifndef _KEYDRIVER_H_
#define _KEYDRIVER_H_

#include "stm32f10x.h"

#define KEY_NUM_ALL 10

#define RCC_APB2Periph_KEY (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE)

#define PORT_KEY0		GPIOB
#define PIN_KEY0		GPIO_Pin_12
#define PORT_KEY1		GPIOB
#define PIN_KEY1		GPIO_Pin_14
#define PORT_KEY2		GPIOB
#define PIN_KEY2		GPIO_Pin_13
#define PORT_KEY3		GPIOE
#define PIN_KEY3		GPIO_Pin_5
#define PORT_KEY4		GPIOE
#define PIN_KEY4		GPIO_Pin_3
#define PORT_KEY5		GPIOE
#define PIN_KEY5		GPIO_Pin_4

#define PORT_KEY6		GPIOB
#define PIN_KEY6		GPIO_Pin_8
#define PORT_KEY7		GPIOB
#define PIN_KEY7		GPIO_Pin_7
#define PORT_KEY8		GPIOB
#define PIN_KEY8		GPIO_Pin_6
#define PORT_KEY9		GPIOB
#define PIN_KEY9		GPIO_Pin_5

/* ��Ӳ����� */
#define KEY_HARDWARE_UP			1	
#define LED_HARDWARE_DOWN		0

/* ����״̬ */
typedef enum{
	KEYUP,
	KEYDOWN,
	KEYERROR
}KeyStatus;

/* �������� */
struct KeyInfo{
	uint32_t filterCount[KEY_NUM_ALL];
	uint8_t  stepFlag[KEY_NUM_ALL];
	uint8_t  currentStatus[KEY_NUM_ALL];
};

void ScanKeyStatus(void);
void KeyDriverInit(void);

/* �����ⲿ���� */
extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);

#endif /* END _KEYDRIVER_H_ */
