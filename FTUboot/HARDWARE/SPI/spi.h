#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#include "stm32f4xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//SPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/16/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern SPI_HandleTypeDef SPI5_Handler;  //SPI���

void SPI5_Init(void);
void SPI5_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI5_ReadWriteByte(u8 TxData);



extern SPI_HandleTypeDef SPI4_Handler;  //SPI���

void SPI4_Init(void);
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI4_ReadWriteByte(u8 TxData);

void SpiUseKill(void);


extern SPI_HandleTypeDef SPI2_Handler;  //SPI���

void SPI2_Init(void);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI2_ReadWriteByte(u8 TxData);


#endif
