
#ifndef __LKDGUIMENU_H
#define	__LKDGUIMENU_H

#include "stdint.h"

#undef  NULL
#define NULL 0

#define MAXMENUITEM  10		//�˵��������ɵĲ˵�������
#define MENU_BEGINHIGH 15 	//�˵���ʼ����ƫ��
#define MENU_BEGINWIDE 0 	//�˵���ʼ����ƫ��
#define MENUITEM_HIGH	15		//�˵���ĸ߶�
#define MENUITEM_WIDE	12*6	//�˵���Ŀ��
#define MAX_NEST_MENU 2

/* �˵����� */
struct MENU_
{
	int16_t x;							// ��ʼ����
	int16_t y;							// ��ʼ����
	struct MENU_  *fatherMenu;			// ��������
	uint8_t  currentItem;				// ��ǰ�˵���
	uint8_t  num;						// �˵������
	uint8_t Itemflag[MAXMENUITEM];		// �˵�����������
	uint8_t  *itemName[MAXMENUITEM];	// �˵�������
	void(*MenuFunction)(void);	// �˵����Ӧ�ķ������
};
typedef  struct MENU_ MENU;

/* �˵����� */
struct MenuManage{
	uint8_t pMenu;
	MENU *userMenu[12];
};


void GUIMenuDraw(MENU *pMenu);

/* ���ز˵� */
void userGUIMenuHide(void);
/* ��Ӳ˵� */
void userGUIMenuAdd(MENU *pMenu);
/* �˵���Ӧ�������� */
void userGUIMenuDisplay(void);
/* �ػ�˵� */
void userGUIMenuRedraw(void);
/* �˵�ѡ�����޸� */
void userGUIMenuIremModfiy(uint8_t flag);
/* ��ȡpMenu */
uint8_t GetMenuManagepMenu(void);

void MenuManageInit(void);
#endif /* __LKDGUIMENU_H */

/* END */
