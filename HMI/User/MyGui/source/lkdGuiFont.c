/**
  * @file   gwGuiFont.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/03/27
  * @brief  gwGui�����ļ�
  */
 
#include "lkdGuiFont.h"
#include "GT21L16S2W.h"
#include "string.h"

lkdFont defaultFont,*thisFont;

/**
  *@brief 12x12���庯��
  *@param  x,y ��ʼ����
  *@param  str �ַ���
  *@retval None
  */
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str)
{
	uint8_t DZData[24];//12*16/8
	while(*str != '\0'){
		if(*str > 0x7f){
			gt_12_GetData(*str,*(str+1),DZData);
			GuiBitMap(DZData,x,y,12,12, backcolor);
			str += 2;
			x += 12;	
		}
		else{
			ASCII_GetData(*str,ASCII_6X12,DZData);
			GuiBitMap(DZData,x,y + 1,8,12, backcolor);
			str ++;
			x += 6;
		}
		if(x >= GUIXMAX){
			x = 0;
			y += 12;
		}
		if(y + 12 >= GUIYMAX){
			break;
		}
	}
}

/**
  *@brief 12x12���������ʾ����
  *@param  x,y ��ʼ����
  *@param  wide ���
  *@param  flag �����־
  *@param  str �ַ���
  *@retval None
  */
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide, FontFlag flag,uint8_t *str)
{ 
	uint8_t DZData[24];//12*16/8
	uint16_t offest;
	uint16_t strPixLen;
	uint16_t xMax;
	lkdBitmap bipmap;
	
	/* ����ƫ�Ƴ��� */
	strPixLen = strlen((char *) str) * 6;
	if(strPixLen <= wide){
		if(flag == FONT_MID){
			offest = (wide - strPixLen) / 2;
		}
		else if(flag == FONT_RIGHT){
			offest = wide - strPixLen;
		}
		else{
			offest = 0;
		}
	}
	else{
		offest = 0;
	}
	
	/* ��ʼ����ַ� */
	xMax = x + wide;
	while(*str != '\0'){
		if(*str > 0x7f){
			gt_12_GetData(*str,*(str+1),DZData);
			GuiBitMap(DZData,x + offest,y,12,12, backcolor);
			str += 2;
			x += 12;
		}
		else{
			ASCII_GetData(*str, ASCII_6X12, DZData);
			GuiBitMap(DZData,x + offest, y + 1, 6, 12, backcolor);
			str ++;
			x += 6;
		}
		if(*str > 0x7f && x + 12 >= xMax){
			gt_12_GetData(*str,*(str+1),DZData);
			bipmap.beginx = 0;
			bipmap.beginy = 0;
			bipmap.bitmap = DZData;
			bipmap.high = 12;
			bipmap.wide = 12;
			GuiAreaBitMap(&bipmap, x + offest, y, xMax, y + 12, backcolor);
			break;
		}
		else if(*str != '\0' && x + 6 >= xMax){
			bipmap.beginx = 0;
			bipmap.beginy = 0;
			bipmap.bitmap = DZData;
			bipmap.high = 12;
			bipmap.wide = 8;
			ASCII_GetData(*str, ASCII_6X12, DZData);
			GuiAreaBitMap(&bipmap, x + offest, y + 1, xMax, y + 12, backcolor);
			break;
		}
	}
}

/* END */
