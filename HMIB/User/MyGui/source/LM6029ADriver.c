/**
  * @file   LM6029ADriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/03/15
  * @brief  Lcd����
  * ���ļ�LcdΪLM6029ACW
  */

#include "LM6029ADriver.h"

/**
  *@brief ���ų�ʼ��
  *@param  None
  *@retval None
  */
static void LcdGpioConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);
	
	/* D0~D7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |  GPIO_Pin_1 |\
		GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |\
		GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* RD WE CS BLA */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* RS RES */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	LCD_BLA_ON();
	LCD_CS_HIGH();
}
static void delay(void)
{
	uint8_t i;
	for(i = 0;i < 50; i++);
}

/**
  *@brief д����
  *@param  cmd ����
  *@retval None
  */
void WriteLcdCommand(uint8_t cmd)
{
	LCD_RS_LOW();
	LCD_RD_HIGH();
	LCD_DATABUS(cmd);
	delay();
	LCD_WR_LOW();
	delay();
	LCD_WR_HIGH();
}

/**
  *@brief д����
  *@param  data ����
  *@retval None
  */
void WriteLcdData(uint8_t data)
{
	LCD_RS_HIGH();
	LCD_RD_HIGH();
	LCD_DATABUS(data);
	delay();
	LCD_WR_LOW();
	delay();
	LCD_WR_HIGH();
}

/**
  *@brief ������ʼҳ
  *@param  page ҳ
  *@retval None
  */
void SetLcdPage(uint8_t page)
{
	page = page & 0x07;
	page = page | 0xB0;
	WriteLcdCommand(page);
}

/**
  *@brief ������ʼ��
  *@param  col �е�ַ
  *@retval None
  */
void SetLcdCol(uint8_t col)
{
	uint8_t tempCol;
	tempCol = (col >> 4)&0x0F;
	tempCol |= 0x10;
	WriteLcdCommand(tempCol);
	tempCol = col&0x0F;
	WriteLcdCommand(tempCol);
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void OpenLcdDisplay(void)
{
	LCD_CS_LOW();
	WriteLcdCommand(OPEN_LCDDISPLAY);
	LCD_CS_HIGH();
	LCD_BLA_ON();
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void CloseLcdDisplay(void)
{
	LCD_CS_LOW();
	WriteLcdCommand(CLOSE_LCDDISPLAY);
	LCD_CS_HIGH();
	LCD_BLA_OFF();
}

/**
  *@brief LCD��ʼ��
  *@param  None
  *@retval None
  */
void LcdInit(void)
{
	LcdGpioConfig();
	LCD_CS_LOW();
	LCD_RES_LOW();
	delay();
	LCD_RES_HIGH();
	WriteLcdCommand(0xA0);/* SEG ���� 0xA1 ���� */
	WriteLcdCommand(0xC8);/* COM ���� 0xC0 ���� */
	WriteLcdCommand(0xA2);/* ƫѹ�� */
	WriteLcdCommand(0x2F);/* �ڲ���Դ�������� */
	WriteLcdCommand(0x81);/* �Աȶȵ��������� */
	WriteLcdCommand(0x29);/*  */
	WriteLcdCommand(0x40);/* DDRAM��ʼ�е�ַ */
	WriteLcdCommand(OPEN_LCDDISPLAY);/* ����ʾ */
}

/* END */
