/**
  * @file   gwGuiMenu.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/02
  * @brief  gwGui�˵��ļ�
  */
#include "gwGuiMenu.h"
#include "gwGuiBasic.h"
#include "gwGuiFont.h"

/**
  *@brief �������˵�
  *@param  info �������˵�����
  *@param  node ���ڵ�
  *@retval None
  */
void GuiDrawMenu(MenuInfo *info, MenuItem *node)
{
	uint16_t getBeginNode = 0;
	uint8_t forecolor ,backcolor ;
	MenuItem *ptempNode;
	uint16_t tempX,tempY,tempWide,tempItem;
	forecolor = GuiGetForecolor();
	backcolor = GuiGetbackcolor();
	tempX = info->beginX + 1;
	tempY = info->beginY;
	tempWide = info->endX - info->beginX - 1;
	ptempNode = node->firstChild;
	tempItem = info->beginItem;
	
	while(1){//��ȡ��ʼ���ƽڵ�
		if(getBeginNode == tempItem){
			break;
		}
		ptempNode = ptempNode->nextSibling;
		getBeginNode ++;
	}
	GuiFillRect(info->beginX, info->beginY, info->endX,info->endY,backcolor);
	while(1){
		if(ptempNode == NULL || tempY + info->hight > info->endY){
			break;
		}
		if(tempItem == info->currentItem){
			GuiFillRect(tempX, tempY + 1, info->endX - 1,tempY + info->hight,forecolor);
			GuiFont12Align(tempX, tempY + 2, tempWide, COL_MID_DISPLAY, ptempNode->thisInfo.name);
		}
		else{
			GuiFont12Align(tempX, tempY + 2, tempWide, COL_MID_DISPLAY, ptempNode->thisInfo.name);
		}
		GuiHLine(tempX, tempY + info->hight, info->endX - 1, forecolor);
		ptempNode = ptempNode->nextSibling;
		tempItem ++;
		tempY = tempY + info->hight;
	}
}

/**
  *@brief ����ӽڵ�˵�
  *@param  hd �˵����
  *@param  pMenu Ҫ���Ľڵ�
  *@retval 0 �ɹ� 1 ʧ��
  */
//uint8_t GuiMenuNodeAdd(MenuHandle *hd, MenuItem *pMenu)
//{
//	if(hd->MenuStack.pMenu >= MENU_MAXDEEP){
//		return 1;/* �ﵽ����ջ��ȣ�����ʧ�� */
//	}
//	hd->menuStack.beginItem[hd->menuStack.pMenu - 1] = hd->drawinfo.beginItem;
//	hd->menuStack.currentItem[hd->menuStack.pMenu - 1] = hd->drawinfo.currentItem;
//	hd->menuStack.userMenu[MenuStack.pMenu] = pMenu;
//	hd->MenuStack.pMenu ++;
//	
//	return 0;
//}

///**
//  *@brief ��������ӽڵ�˵�
//  *@param  hd �˵����
//  *@param  pMenu Ҫ���Ľڵ�
//  *@retval 0 �ɹ� 1 ʧ��
//  */
//uint8_t GuiMenuTopNodeHide(MenuHandle *hd, MenuItem *pMenu)
//{
//	if(hd->MenuStack.pMenu <= 0){
//		return 1;/* û�п����ٵģ�����ʧ�� */
//	}
//	hd->menuStack.userMenu[MenuStack.pMenu - 1] = NULL;
//	hd->MenuStack.pMenu --;
//	
//	return 0;
//}

//void GuiMenuItem
