#ifndef __LKDGUICOLOUR_H
#define	__LKDGUICOLOUR_H

#include "stdint.h"

typedef uint8_t lkdColour;

#define CWHITLE	0x00/* ��ɫ */
#define CBLACK	0x01/* ��ɫ */

extern lkdColour forecolor,backcolor;

/* ��ȡǰ��ɫ */
lkdColour GuiGetForecolor(void);
/* ����ǰ��ɫ */
void GuiSetForecolor(lkdColour colour);
/* ��ȡ����ɫ */
lkdColour GuiGetbackcolor(void);
/* ���ñ���ɫ */
void GuiSetbackcolor(lkdColour colour);
/* ����ǰ��ɫ�ͱ���ɫ */
void GuiExchangeColor(void);

#endif /* __GWGUIDriver_H */

/* END */
