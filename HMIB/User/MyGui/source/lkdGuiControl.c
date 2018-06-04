/**
  * @file   gwGuiControl.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/10
  * @brief  lkdGui�ؼ�
  */

#include "lkdGuiControl.h"
#include "lkdGuiFont.h"

/**
  *@brief ��ֱ������
  *@param  pScroll Scroll handle
  *@retval None
  */
void GuiVScroll(SCROLL *pScroll)
{
	uint8_t lumpsize;
	uint8_t lumpAdd;
	float eachSize;
	if(pScroll->x + 3 > GUIXMAX || pScroll->y + pScroll->hight > GUIYMAX){
		return;
	}
	if(pScroll->lump < 1){
		pScroll->lump = 1;
	}
	else if(pScroll->lump > pScroll->max){
		pScroll->lump = pScroll->max;
	}	
	GuiFillRect(pScroll->x,pScroll->y,pScroll->x + 3,\
		pScroll->y + pScroll->hight,GuiGetForecolor());
	if(pScroll->max > 5){//������������ѡ��
		lumpsize = pScroll->hight/7;
	}
	else if(pScroll->max <= 1){
		return;
	}
	else{
		lumpsize = pScroll->hight/pScroll->max;
	}
	GuiRPointLine(pScroll->x+1,pScroll->y + 1,pScroll->y + pScroll->hight - 1,2,GuiGetbackcolor());
	GuiRPointLine(pScroll->x + 2,pScroll->y + 2,pScroll->y + pScroll->hight-1,2,GuiGetbackcolor());
	/* �����ȿ� */
	eachSize = ((float)(pScroll->hight-lumpsize))/pScroll->max;
	lumpAdd = pScroll->y + (uint8_t)((pScroll->lump - 1) * eachSize);
	if(pScroll->lump < pScroll->max){
		GuiFillRect(pScroll->x+1,lumpAdd,pScroll->x + 2,lumpAdd + lumpsize,GuiGetForecolor());
	}
	else{
		GuiFillRect(pScroll->x+1,pScroll->y+pScroll->hight - lumpsize,\
			pScroll->x + 2,pScroll->y+pScroll->hight - 1,GuiGetForecolor());
	}
}

/**
  *@brief ����
  *@param  x��y ��ʼ����
  *@param  keyName ��������
  *@param  flag 0��ѡ�� 1ѡ��
  *@retval None
  */
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag)
{
	if(flag == 0){//��ѡ��
		GuiFillRect(x,y,x + 44,y + 17, GuiGetbackcolor());
		GuiRect(x,y,x + 43,y + 16, GuiGetForecolor());
		GuiFont12(x + 10,y + 3,keyName);
		GuiRLine(x + 44,y+1,y + 16,GuiGetForecolor());
		GuiHLine(x+1,y + 17,x + 44,GuiGetForecolor());
	}
	else{
		GuiFillRect(x,y,x + 44,y + 17, GuiGetbackcolor());
		GuiRect(x,y,x + 44,y + 17, GuiGetForecolor());
		GuiFillRect(x + 2,y + 2,x + 42,y + 15, GuiGetForecolor());
		GuiExchangeColor();
		GuiFont12(x + 11,y + 3,keyName);
		GuiExchangeColor();
	}
}

/* END */
