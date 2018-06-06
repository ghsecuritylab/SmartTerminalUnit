/**
  * @file   ds18B20.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   �¶Ȼ�ȡ�ļ�
  */
 
#ifndef __DS18B20_H
#define	__DS18B20_H

#include "stm32f10x.h"

#define CYCTRANSFORMVAULE 1000 /* ѭ��ת��ʱ��*/

#define HIGH  1
#define LOW   0

#define DS18B20_CLK     RCC_APB2Periph_GPIOE
#define DS18B20_PIN     GPIO_Pin_2                  
#define DS18B20_PORT	GPIOE 

/* ����ߵ�ƽ��͵�ƽ */
#define DS18B20_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);\
					else		\
					GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN)
/* ��ȡ���ŵĵ�ƽ */
#define  DS18B20_DATA_IN()	GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)


/* ��ȡ�¶� */
float GetTemperature(void);
/* ������ */
void Ds18B20Main(void);
/* Ds18B20��ʼ�� */
void DS18B20_Init(void);
					
#endif /* __DS18B20_H */
