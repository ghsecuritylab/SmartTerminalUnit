#ifndef __GWGUIMENU_H
#define	__GWGUIMENU_H

#include "gwGuiDriver.h"

#undef NULL
#define NULL 0
#define MENU_MAXDEEP 16		/* �˵������� */

struct DrawMenuInfo{
	uint16_t beginX;		/* ��ʼ���� */
	uint16_t beginY;
	uint16_t endX;			/* �������� */
	uint16_t endY;
	uint16_t hight;			/* �߶� */
	uint16_t currentItem;	/* ��ǰItem */
	uint16_t beginItem;		/* ��ʼ����Item */
};
typedef struct DrawMenuInfo MenuInfo;
struct ItemInfo{
	uint8_t flag1;/*  */
	uint8_t *name;/* ���� */
	void(*ItemFunction)(void);/* Item��Ӧ�ķ������ */ 
};
struct GuiMenuTree{
	struct ItemInfo thisInfo;/* �˽ڵ���Ϣ */
	struct GuiMenuTree *firstChild;/* �ӽڵ� */
	struct GuiMenuTree *nextSibling;/* �ֵܽڵ� */	
};
typedef struct GuiMenuTree MenuItem;

struct GuiMenuManage{
	uint8_t topMenu;
	uint16_t currentItem[MENU_MAXDEEP];
	uint16_t beginItem[MENU_MAXDEEP];
	MenuItem *userMenu[MENU_MAXDEEP];
};
typedef struct GuiMenuManage MenuManage;

struct GuiMenuHandle{
	MenuManage menuStack;
	MenuInfo drawinfo;
	MenuItem *rootNode;
};
typedef struct GuiMenuHandle MenuHandle;
#endif /* __GWGUIMENU_H */

/* END */
