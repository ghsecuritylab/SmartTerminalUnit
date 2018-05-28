#ifndef __LKDGUIBASIC_H
#define	__LKDGUIBASIC_H

#include "lkdGuiColour.h"

/* ������y ������ x */
#define  GUI_LCM_XMAX	160
#define  GUI_LCM_YMAX	160
#define  GUIXMAX	GUI_LCM_XMAX
#define  GUIYMAX	GUI_LCM_YMAX

/* ������������ */
typedef int32_t lkdCoord;

/* ����λͼ�ṹ�� */
typedef struct{
	uint8_t *bitmap;/* λͼ���� */
	uint16_t wide;/* λͼ�� */
	uint16_t high;/* λͼ�� */
	uint16_t beginx;/* λͼ��ʼ���� */
	uint16_t beginy;/* λͼ��ʼ���� */
}lkdBitmap;

/* ���� */
void GuiPoint(lkdCoord x, lkdCoord y, lkdColour color);
/* ��б�� */
void GuiBiasLine(lkdCoord x0, lkdCoord y0,lkdCoord x1, lkdCoord y1, lkdColour color);
/* ��ˮƽ�� */
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color);
/* ����ֱ�� */
void GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color);
/* ��ˮƽ���� */
void GuiHPointLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, uint8_t interval, lkdColour color);
/* ����ֱ���� */
void GuiRPointLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, uint8_t interval, lkdColour color);
/* ������ */
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
/* �������� */
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
/* ��λͼ */
void GuiBitMap(uint8_t *bitMap,lkdCoord x0, lkdCoord y0,uint32_t xlen,uint32_t ylen,uint8_t flag);

void GuiAreaBitMap(lkdBitmap *bitMap,lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag);


/* ---���ⲿ�����ṩ--- */

/* �ײ��㺯�� */
extern void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color);
/* �ײ���㺯�� */
extern void GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor);

#endif /* __GWGUIBASIC_H */

/* END */
