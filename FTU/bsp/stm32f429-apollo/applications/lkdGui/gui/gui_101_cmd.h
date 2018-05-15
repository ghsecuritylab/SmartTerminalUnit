
#ifndef __GUI101CMD_H
#define	__GUI101CMD_H

#include "stdint.h"

/* ������y ������ x */
#define  GUI_LCM_XMAX	160
#define  GUI_LCM_YMAX	160
#define  GUIXMAX	GUI_LCM_XMAX
#define  GUIYMAX	GUI_LCM_YMAX

/* ������������ */
typedef int32_t lkdCoord;
/* ������ɫ���� */
typedef uint8_t lkdColour;

/* ������뷽ʽ */
typedef enum{
	FONT_LEFT,	/* ����� */
	FONT_MID,		/* ���� */
	FONT_RIGHT,	/* �Ҷ��� */
}FontFlag;

/* ������ */
typedef struct{
	int16_t x;
	int16_t y;
	uint16_t hight;
	uint16_t max;
	uint16_t lump;
}SCROLL;

/* ����λͼ�ṹ�� */
typedef struct{
	uint8_t number;
	uint8_t *bitmap;/* λͼ���� */
	uint16_t wide;/* λͼ�� */
	uint16_t high;/* λͼ�� */
	uint16_t beginx;/* λͼ��ʼ���� */
	uint16_t beginy;/* λͼ��ʼ���� */
}lkdBitmap;

/* ����ö�� */
enum KeyStatus{
	NoKey,
	UpKey,
	DownKey,
	LeftKey,
	RightKey,
	OkKey,
	CancelKey
};


#define PACKBUFFMAX 1024
typedef struct{
	uint8_t state;
	uint8_t packBuff[PACKBUFFMAX];
	uint16_t pIn;
	uint8_t cmdNum;
}Gui101CmdControl;

/* ����ǰ��ɫ/����ɫ */
extern lkdColour forecolor,backcolor;
/* ����cmd101�����¼� */
extern struct rt_event Cmd101SendEvent;

void GuiBiasLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdCoord y1, lkdColour color);
void GuiHPointLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, uint8_t interval, lkdColour color);
void GuiRPointLine(lkdCoord x0, lkdCoord y0, lkdCoord y1,uint8_t interval, lkdColour color);
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color);
void GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color);
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str);
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide, FontFlag flag,uint8_t *str);
void GuiUpdateDisplayAll(void);
void GuiSetbackcolor(lkdColour colour);
void GuiSetForecolor(lkdColour colour);
void GuiExchangeColor(void);
void GuiAreaBitMap(lkdBitmap *bitMap,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag);
void OpenLcdDisplay(void);
void CloseLcdDisplay(void);
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag);
void GuiVScroll(SCROLL *pScroll);

/* ���������� */
enum KeyStatus GetKeyStatus(void);
void SetKeyStatus(enum KeyStatus keyIs);
void SetKeyIsNoKey(void);




void TestMain(void);

extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
extern uint8_t DLT634_HMI_SLAVE_C_REPLY(uint8_t drvid,uint8_t *pbuf);

#endif /* __GUI101CMD_H */

/* END */
