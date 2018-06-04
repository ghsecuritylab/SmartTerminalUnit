/**
  * @file   lkdGuiDriver.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGui����
  */

#include "lkdGuiDriver.h"
/* lcd�����ļ����� */
#include "UC1698Driver.h"

/* Gui��ʾ������ */
static uint8_t  lkdGuiBuff[GUIYMAX][GUIXMAX/8 + GUIXMAX%8];
/* ��λ��ȡ���� */
const static uint8_t colBit[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

/**
  *@brief ��Gui������������ʾ��lcd��
  *@param  None
  *@retval None
  */
void GuiUpdateDisplayAll(void)
{
	uint32_t x,y;
	LCD_CS_LOW();
	SetLcdCol(0x25);
	SetLcdRow(0);
//	WriteLcdCommand(0x60);	//row address LSB     
//	WriteLcdCommand(0x70);	//row address MSB	
//	WriteLcdCommand(0x05);	//Column address LSB 
//	WriteLcdCommand(0x12);	//Column address MSB
	for(y = 0; y < GUIYMAX; y++){
		for(x = 0;x < 20; x++){
			Write8DotsUC1698U(lkdGuiBuff[y][x]);
		}
		WriteLcdData(0x00);
	}
	LCD_CS_HIGH();
}

/**
  *@brief ����
  *@param  x,y ����
  *@param  color ��ɫ <Ŀǰֻ�кڰ�>
  *@retval None
  */
void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color)
{
	if(color == CWHITLE){//��ɫ
		lkdGuiBuff[y][x >> 3] &= ~colBit[x & 0x07];
	}
	else{//��ɫ
		lkdGuiBuff[y][x >> 3] |= colBit[x & 0x07];
	}
}

/**
  *@brief ����
  *@param  x,y ����
  *@param  color ��ɫ <Ŀǰֻ�кڰ�>
  *@retval None
  */
void  GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor)
{
	
}
