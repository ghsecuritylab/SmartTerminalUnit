#ifndef __LKDGUIDRIVER_H
#define	__LKDGUIDRIVER_H

#include "lkdGuiBasic.h"

/* ���� */
void GuiUpdateDisplayAll(void);
/* ���� */
void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color);
/* ���� */
void  GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor);

#endif /* __LKDGUIDRIVER_H */

/* END */
