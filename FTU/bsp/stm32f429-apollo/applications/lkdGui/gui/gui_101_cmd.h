
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
typedef uint8_t lkdColour;
typedef enum{
	FONT_LEFT,	/* ����� */
	FONT_MID,		/* ���� */
	FONT_RIGHT,	/* �Ҷ��� */
}FontFlag;

#define PACKBUFFMAX 1024
typedef struct{
	uint8_t state;
	uint8_t packBuff[PACKBUFFMAX];
	uint16_t pIn;
	uint8_t cmdNum;
}Gui101CmdControl;

void TestMain(void);

extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
extern uint8_t DLT634_HMI_SLAVE_C_REPLY(uint8_t drvid,uint8_t *pbuf);
#endif /* __GUI101CMD_H */

/* END */
