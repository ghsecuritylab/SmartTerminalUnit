
#ifndef __LKDGUIWINDOW_H
#define	__LKDGUIWINDOW_H

#include "stdint.h"

#undef  NULL
#define NULL 0

/* ���� */
struct WINDOW_
{
	int16_t x;							// ��ʼ����
	int16_t y;							// ��ʼ����
	int16_t wide;						// ���
	int16_t hight;						// �߶�
	struct WINDOW_  *fatherMenu;			// ��������
	uint8_t   *title;		// ���������ָ��(NULLΪ����ʾ)
	void(*WindowFunction)(void);	// ���ڶ�Ӧ�ķ������
} ;
typedef  struct WINDOW_ WINDOW;


/* ���ڹ��� */
struct WindowManage{
	uint8_t pWin;
	WINDOW *userWindow[12];	
};

void userGUITopWindowHide(void);
void userGUIWindowAdd(WINDOW *pWindow);
void userGUITopWindowDisplay(void);
void userGUITopWindowRedraw(void);
void WinManageInit(void);

#endif /* __LKDGUIMENU_H */

/* END */
