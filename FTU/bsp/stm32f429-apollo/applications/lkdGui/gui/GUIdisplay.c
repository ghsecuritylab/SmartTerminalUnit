/**
  * @file   GUIdisplay.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI��ʾ
  */

#include "gui_common.h"
#include "GUIdisplay.h"
#include "GUIdisplaydef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "userVariable.h"

const static uint8_t *modfiyKey[14] = {/* ���� */
	"0","1","2","3","4",".","esc","5","6","7","8","9","<-","ok"};
static enum KeyStatus keyStatus;	/* ����ֵ */
static uint8_t userGUIBuff[1024*3]; /* �����ڴ���� */
static struct DZModfiy dZModfiy;	/* ��ֵ�޸� */
static struct Message MessageIs;	/* ��Ϣ���� */
static struct PassWordPipe passWordPipe;		/* ������� */
static enum DingZhiSetOrSee dingZhiSetOrSee;	/* ��ֵ��ѯ�����޸ı�־ */

/**
  *@brief  ��ȡʱ��
  *@param  None
  *@retval ��ǰʱ�ӵδ�
  */	
static uint32_t getCurrentTick(void)
{
	return GetTimer1Tick();
}

/**
  *@brief  ��ȡ���ʱ��
  *@param  ��ʼʱ�ӵδ�
  *@retval ���ʱ��
  */
static uint32_t GetIntervalTick(uint32_t beginTick)
{
	return GetTimer1IntervalTick(beginTick);
}

/**
  *@brief �޸Ķ�ֵ���û��������־
  *@param  setOrSee ���á���� ö��
  *@retval None
  */
static void SetDingZhiSetOrSee(enum DingZhiSetOrSee setOrSee)
{
	dingZhiSetOrSee = setOrSee;
}

/**
  *@brief ��ȡ��ֵ���û��������־
  *@param  None
  *@retval DingZhiSetOrSee ���á���� ö��
  */
static enum DingZhiSetOrSee GetDingZhiSetOrSee(void)
{
	return dingZhiSetOrSee;
}

/**
  *@brief lcd��ʾ����
  *@param  key ����״̬
  *@retval lcd��ʾ״̬ LCDDISPLAYON LCDDISPLAYOFF
  */
static uint8_t GUIDisplayONOFF(enum KeyStatus *key)
{
	static uint32_t LcdDisPlayTick;//��ʱ
	static uint8_t flag;
	static uint8_t lcdONOFF;
	if(flag == 0){
		LcdDisPlayTick = getCurrentTick();
		lcdONOFF = LCDDISPLAYON;
		flag = 1;
	}
	if(flag == 1){
		if(*key != NoKey){
			LcdDisPlayTick = getCurrentTick();
			if(lcdONOFF == LCDDISPLAYOFF){
				lcdONOFF = LCDDISPLAYON;
				*key = NoKey;
				OpenLcdDisplay();
			}
		}
		else{
			if(lcdONOFF == LCDDISPLAYON){
				if(GetIntervalTick(LcdDisPlayTick) > LCDDISPLAYOFFTIME){//ʱ��
					lcdONOFF = LCDDISPLAYOFF;
					CloseLcdDisplay();
				}
			}
		}
	}
	return lcdONOFF;
}


/**
  *@brief GUI��������
  *@param  None
  *@retval None
  */
void GUIStartInterface(void)
{
	uint8_t i,j;
	lkdBitmap tImage;
	GuiSetbackcolor(0);
	GuiSetForecolor(1);
	tImage.number = 1;
	tImage.wide = 160;
	tImage.high = 30;
	tImage.beginx = 0;
	tImage.beginy = 0;
	tImage.bitmap = NULL;
	GuiAreaBitMap(&tImage,1, 25, 159, 55, 0);
	j = 110;
	for(i=1;i<24;i+=2){
		GuiHPointLine(1,i,j,2,forecolor);
		j -= 2;
	}
	for(i=25;i<55;i+=2){
		GuiHPointLine(1,i,159,2,forecolor);
	}
	j = 70;
	for(i=55;i<78;i+=2){
		GuiHPointLine(j,i,159,2,forecolor);
		j -= 2;
	}
	GuiHLine(1,78,158,forecolor);
	GuiFillRect(1,81,159,94, forecolor);
	GuiHLine(1,96,158,forecolor);
	GuiExchangeColor();
	GuiFont12Align(2,82,156,FONT_MID,"��������......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI���߸���
  *@param  None
  *@retval None
  */
void GUIUpdataProgram(void)
{
	uint8_t i,j;
	lkdBitmap tImage;
	
	GuiFillRect(1,1,158,158, backcolor);
	tImage.number = 1;
	tImage.wide = 160;
	tImage.high = 30;
	tImage.beginx = 0;
	tImage.beginy = 0;
	tImage.bitmap = NULL;
	GuiAreaBitMap(&tImage,1, 25, 159, 55, 0);
	j = 110;
	for(i=1;i<24;i+=2){
		GuiHPointLine(1,i,j,2,forecolor);
		j -= 2;
	}
	for(i=25;i<55;i+=2){
		GuiHPointLine(1,i,159,2,forecolor);
	}
	j = 70;
	for(i=55;i<78;i+=2){
		GuiHPointLine(j,i,159,2,forecolor);
		j -= 2;
	}
	GuiHLine(1,78,158,forecolor);
	GuiFillRect(1,81,159,94, forecolor);
	GuiHLine(1,96,158,forecolor);
	GuiExchangeColor();
	GuiFont12Align(2,82,156,FONT_MID,"���ڸ���......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI�ȴ�ͼ��
  *@param  x,y ��ʼ����
  *@retval None
  */
void GUIWiatMessage(uint8_t x,uint8_t y)
{
	GuiFillRect(x,y,x + 88,y + 16, backcolor);
	GuiHLine(x + 2,y + 16,x + 86,forecolor);
	GuiHLine(x + 3,y + 17,x + 87,forecolor);
	GuiRLine(x + 86,y + 2,y + 16,forecolor);
	GuiRLine(x + 87,y + 3,y + 16,forecolor);
	GuiFont12Align(x + 20,y + 2,70,FONT_LEFT,"������.....");
	GuiFillRect(x + 2,y + 2,x + 14,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiFillRect(x + 2,y + 2,32 + 12,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiUpdateDisplayAll();
}
/**
  *@brief GUI��ʼ�������������
  *@param  None
  *@retval None
  */
void GUIDisplayInit(void)
{
	MenuManageInit();
	WinManageInit();
	userGUIWindowAdd(&HomeWindow);
}

/**
  *@brief GUI����������������������
  *@param  None
  *@retval None
  */
void GUIDisplayMian(void)
{
//	static uint8_t flag;
//	uint8_t upDataProgram;
//	//upDataProgram = GetDtatUpdateStatus();
//	keyStatus = GetKeyStatus();//��ȡ����״̬
//	SetKeyIsNoKey();
//	if(flag == 0 && upDataProgram.bit.programUpdate != 0){
//		OpenLcdDisplay();
//		GUIUpdataProgram();//���߸���
//		flag = 1;
//	}
//	else if(flag == 1 && upDataProgram.bit.programUpdate == 0){
//		flag = 0;
//		//NVIC_SystemReset();//����
//	}
//	else if(flag == 0  && GUIDisplayONOFF(&keyStatus) == LCDDISPLAYON){
//		userGUITopWindowDisplay();
//	}
}

/**
  *@brief �������봰�����ֵ
  *@param  type ��������
  *@param  returnflag ����״ָ̬��
  *@retval None
  */
void SetPassWordWin(uint8_t type,uint8_t *returnflag)
{
	passWordPipe.passWordType = type;
	passWordPipe.returnFlag = returnflag;
}

/**
  *@brief ���봰��
  *@param  None
  *@retval None
  */
void PassWordFun(void)
{	
	static uint8_t flag;
	static uint8_t passWordStr[8];
	static int8_t currentNum;
	uint8_t tempStr[2];
	uint8_t i;
	if(flag == 0){
		currentNum = 0;
		for(i=0;i<7;i++){
			passWordStr[i] = '0';
		}
		passWordStr[7] = '\0';
		GuiFillRect(PassWordWin.x+1,PassWordWin.y+16,\
			PassWordWin.x + PassWordWin.wide - 2,PassWordWin.hight - 1, backcolor);
		GuiHLine(PassWordWin.x+1,PassWordWin.y+18,\
			PassWordWin.x + PassWordWin.wide - 1,forecolor);
		GuiFont12Align(PassWordWin.x+1,PassWordWin.y +20,PassWordWin.wide - 1,FONT_LEFT,\
			PassWordItems[passWordPipe.passWordType].name);//��ʾ��
		GuiHPointLine(PassWordWin.x+1,PassWordWin.y +34,\
			PassWordWin.x + PassWordWin.wide - 1,2,forecolor);
		flag = 1;
	}
	if(flag == 1){
		for(i=0;i<6;i++){
			if(i == currentNum){
				tempStr[0] = passWordStr[currentNum];
				tempStr[1] = '\0';
				GuiFillRect(PassWordWin.x + 25 + i*10 - 1,PassWordWin.y +45,\
					PassWordWin.x + 25 + i*10 + 6,PassWordWin.y +57, forecolor);
				GuiExchangeColor();
				GuiFont12Align(PassWordWin.x + 25 + i*10,PassWordWin.y +45,12,FONT_LEFT,tempStr);
				GuiExchangeColor();
			}
			else{
				GuiFillRect(PassWordWin.x + 25 + i*10 - 1,PassWordWin.y +45,\
					PassWordWin.x + 25 + i*10 + 6,PassWordWin.y +57, backcolor);
				GuiFont12Align(PassWordWin.x + 25 + i*10,PassWordWin.y +45,12,FONT_LEFT,"*");
			}		
		}
		GuiUpdateDisplayAll();
		flag = 2;
	}
	switch(keyStatus){
	case LeftKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = 5;
		}
		flag = 1;
		break;
	case DownKey:
		passWordStr[currentNum] --;
		if(passWordStr[currentNum]<'0'){
			passWordStr[currentNum] = '9';
		}
		flag = 1;
		break;
	case RightKey:
		currentNum ++;
		if(currentNum >= 6){
			currentNum = 0;
		}
		flag = 1;
		break;
	case UpKey:
		passWordStr[currentNum] ++;
		if(passWordStr[currentNum] > '9'){
			passWordStr[currentNum] = '0';
		}
		flag = 1;
		break;
	case OkKey:
		if(strncmp((char *)passWordStr,\
			(char *)PassWordItems[passWordPipe.passWordType].passWord,6) == 0){
			*passWordPipe.returnFlag = 1;
		}
		else{
			*passWordPipe.returnFlag = 0;
		}//��Ҫִ��CancelKey��������û��break
	case CancelKey:
		flag = 0; 
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}
/**
  *@brief ������Ϣ������Ӧ����
  *@param  x ,y ��ʼ����
  *@param  pStr ˵���ַ���
  *@param  flag ���ر�־ 0 ȡ�� 1 ȷ��
  *@retval None
  */
void SetMessageWin(int16_t x,int16_t y,uint8_t *pStr,uint8_t *flag)
{
	MessageWin.x = x;
	MessageWin.y = y;
	MessageIs.str = pStr;
	MessageIs.flag = flag;
}
/**
  *@brief ��Ϣ���ڴ�����
  *@param  None
  *@retval None
  */
void MessageFun(void)
{
	static uint8_t flag;
	static int8_t keyFlag;
	uint8_t winWide;
	uint8_t winHight;
	if(flag == 0){
		keyFlag = 0;
		flag = 1;
	}
	if(flag == 1){
		GuiFillRect(MessageWin.x+1,MessageWin.y+16,\
			MessageWin.x + MessageWin.wide - 2,MessageWin.hight - 1, backcolor);
		GuiHLine(MessageWin.x+1,MessageWin.y+18,\
			MessageWin.x + MessageWin.wide - 1,forecolor);
		winHight =  (MessageWin.hight - 41 - 12)/2;
		GuiFont12Align(MessageWin.x+1,winHight + MessageWin.y +19,\
			MessageWin.wide - 1,FONT_MID,MessageIs.str);//��ʾ��
		winHight = MessageWin.y + MessageWin.hight - 23;
		GuiHPointLine(MessageWin.x+1,winHight,\
			MessageWin.x + MessageWin.wide - 1,2,forecolor);
		if(keyFlag == 0){
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"ȷ��",0);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"ȡ��",1);
		}
		else{
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"ȷ��",1);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"ȡ��",0);
		}
		GuiUpdateDisplayAll();
	}
	switch(keyStatus){
	case LeftKey:
		keyFlag--;
		if(keyFlag < 0){
			keyFlag = 1;
		}break;
	case UpKey:break;
	case RightKey:
		keyFlag++;
		if(keyFlag > 1){
			keyFlag = 0;
		}break;
	case DownKey:break;
	case OkKey:
		*MessageIs.flag = keyFlag;
		flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	case CancelKey:
		*MessageIs.flag = 0;
		flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	default:break;
	}
}

/**
  *@brief  ���ö�ֵ�޸���ر���
  *@param  pDingZhiItems ��ֵ�ṹ��ָ��
  *@param  pDingZhiItems modbusָ��
  *@param  flag ��־�ֽڣ�δʹ��
  *@param  itemIs ��ֵ��ǰItem
  *@param  str Ҫ�޸��ַ�����ָ��
  *@retval None
  */
static void SetDingZhiModfiy(const struct DingZhi1Item *pDingZhiItems,\
	const struct ModbusCmd_ *cmd,uint8_t flag,uint8_t itemIs,uint8_t *str)
{
	dZModfiy.Items = pDingZhiItems;
	dZModfiy.cmd = cmd;
	dZModfiy.flag = flag;
	dZModfiy.itemIs = itemIs;
	dZModfiy.str = str;	
}

///**
//  *@brief ��ֵ�޸ĳ�ʼ��
//  *@param  None
//  *@retval None
//  */
//void DZModfiyInit(void)
//{
//	int16_t x,y;
//	GuiRect(DZModfiyWin.x,DZModfiyWin.y,DZModfiyWin.x + DZModfiyWin.wide, \
//		DZModfiyWin.y + DZModfiyWin.hight, forecolor);
//	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 17,150,FONT_LEFT,\
//		dZModfiy.Items[dZModfiy.itemIs].name);
//	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 30,\
//		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + DZModfiyWin.hight - 16,\
//		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);//״̬��
//	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + DZModfiyWin.hight - 14,80,FONT_LEFT,"��ǰ״̬:");
//	if(dZModfiy.Items[dZModfiy.itemIs].flag){//����
//		x = DZModfiyWin.x + DZModfiyWin.wide;
//		y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
//		GuiFillRect(DZModfiyWin.x+9,y,x - 10,y + 17, backcolor);
//		GuiFillRect(DZModfiyWin.x+17,y + 2,x - 20,y + 15, forecolor);
//		GuiFont12Align(DZModfiyWin.x+7,y+4,10,FONT_LEFT,"<");
//		GuiFont12Align(x - 14,y+4,10,FONT_LEFT,">");
//	}
//	else{//float
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 32,40,FONT_LEFT,"���ֵ");
//		GuiFont12Align(DZModfiyWin.x + 50,\
//			DZModfiyWin.y + 32,80,FONT_MID,dZModfiy.Items[dZModfiy.itemIs].max);
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 45,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 47,40,FONT_LEFT,"��Сֵ");
//		GuiFont12Align(DZModfiyWin.x + 50,\
//			DZModfiyWin.y + 47,80,FONT_MID,dZModfiy.Items[dZModfiy.itemIs].min);
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 60,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 62,40,FONT_LEFT,"��ǰֵ");
//		
//		GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 62,\
//			DZModfiyWin.x + 128,DZModfiyWin.y + 74, backcolor);
//		GuiFont12Align(DZModfiyWin.x + 50,\
//			DZModfiyWin.y + 62,80,FONT_MID,dZModfiy.str);
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 75,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 77,40,FONT_LEFT,"�޸�Ϊ");
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 90,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiRPointLine(DZModfiyWin.x + 47,DZModfiyWin.y + 30,\
//			DZModfiyWin.y + 90,2,forecolor);	
//	}
//}

///**
//  *@brief ����ֵ�޸İ���
//  *@param  None
//  *@retval None
//  */
//void DrawModfiyKey(uint8_t key)
//{
//	uint8_t i;
//	int16_t x,y;
//	for(i=0;i<14;i++){
//		if(i < 7){
//			x = DZModfiyWin.x + i*18;
//			y = DZModfiyWin.y + 93;
//		}
//		else{
//			x = DZModfiyWin.x + (i - 7)*18;
//			y = DZModfiyWin.y + 105;
//		}
//		if(i == key){
//			GuiFillRect(x,y,x + 18,y + 11, forecolor);
//			GuiExchangeColor();
//			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
//			GuiExchangeColor();
//		}
//		else{
//			GuiFillRect(x,y,x + 18,y + 11, backcolor);
//			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
//		}
//		GuiRLine(x,y,y + 10,forecolor);
//	}
//	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 92,\
//		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
//	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 104,\
//		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
//	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 116,\
//		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
//}

///**
//  *@brief ��ֵ�޸Ĵ��ں���
//  *@param  None
//  *@retval None
//  */
//void DZModfiyFun(void)
//{
//	static uint8_t flag = 0;//����
//	static uint8_t modfiyStr[16];//�޸��ݴ�buff
//	static uint8_t modfiyStrP = 0;
//	static uint8_t keyIs = 0;//������¼
//	static uint32_t inputCursorTick;//�����˸��ʱ
//	static uint8_t getDataFlag;
//	int16_t x,y;
//	uint8_t tempRetrun;
//	static union{
//		float f32;
//		uint16_t u16[2];
//	}ftrans16;
//	float fValue;
//	if(flag == 0){//��ʼ��
//		DZModfiyInit();
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){
//			keyIs = dZModfiy.str[0];
//		}
//		flag = 1;
//		getDataFlag = 0;
//	}
//	if(flag == 1){//��ʾ
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){//���ִ���
//			GuiExchangeColor();
//			x = DZModfiyWin.x + DZModfiyWin.wide;
//			y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
//			GuiFillRect(DZModfiyWin.x+18,y + 3,\
//				x - 20,y + 15, backcolor);
//			GuiFont12Align(DZModfiyWin.x+18,y + 3,x - DZModfiyWin.x - 40,\
//				FONT_MID,dZModfiy.Items[dZModfiy.itemIs].pContent[keyIs]);
//			GuiExchangeColor();
//			flag = 3;
//		}
//		else{//float����
//			DrawModfiyKey(keyIs);
//			GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
//				DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 50,\
//				DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
//			flag = 2;
//		}
//		GuiUpdateDisplayAll();
//	}
//	if(flag == 2){//�����������
//		if(dZModfiy.Items[dZModfiy.itemIs].flag == 0){
//			if(GetIntervalTick(inputCursorTick) < 500){
//				if(modfiyStr[modfiyStrP] != '|'){
//					modfiyStr[modfiyStrP] = '|';
//					modfiyStr[modfiyStrP + 1] = '\0';
//					flag = 1;
//				}
//			}
//			else if(GetIntervalTick(inputCursorTick) < 1000){
//				if(modfiyStr[modfiyStrP] != ' '){
//					modfiyStr[modfiyStrP] = ' ';
//					modfiyStr[modfiyStrP + 1] = '\0';
//					flag = 1;
//				}
//			}
//			else{
//				inputCursorTick = getCurrentTick();
//			}				
//		}
//	}
//	if(flag == 3){//�ȴ�
//	}
//	if(flag == 4){//��������
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){//����		
//			ftrans16.f32 = (float)modfiyStr[0];
//			GuiFillRect(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,\
//				DZModfiyWin.x + DZModfiyWin.wide - 2,\
//				DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"�޸���...");
//			flag = 5;
//		}
//		else{
//			modfiyStr[modfiyStrP] = '\0';
//			fValue = atof((const char *)modfiyStr);
//			if(fValue <= atof((const char *)dZModfiy.Items[dZModfiy.itemIs].max) &&\
//				fValue >= atof((const char *)dZModfiy.Items[dZModfiy.itemIs].min)){
//				ftrans16.f32 = fValue;
//				GuiFillRect(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,\
//					DZModfiyWin.x + DZModfiyWin.wide - 2,\
//					DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//				GuiFont12Align(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"�޸���...");
//				flag = 5;
//			}
//			else{//������Χ
//				GuiFillRect(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,\
//					DZModfiyWin.x + DZModfiyWin.wide - 2,\
//					DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//				GuiFont12Align(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"������Χ");
//				flag = 1;
//			}
//		}
//	}
//	if(flag == 5){//�������ȴ��ظ�
//		tempRetrun = SetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			dZModfiy.cmd->addr.addr16 +(dZModfiy.Items[dZModfiy.itemIs].addr -\
//			dZModfiy.cmd->mapAddr.addr16)*2,2,&ftrans16.u16[0]);
//		if(tempRetrun == ISTURE){//�޸ĳɹ�
//			if(dZModfiy.Items[dZModfiy.itemIs].flag){
//				dZModfiy.str[0] = modfiyStr[0];
//			}
//			else{
//				strcpy((char *)dZModfiy.str,(char *)modfiyStr);
//			}
//			GuiFillRect(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,\
//				DZModfiyWin.x + DZModfiyWin.wide - 2,\
//				DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"�޸ĳɹ�");
//			flag = 0;
//		}
//		else if(tempRetrun == ISFAILTURE){//�޸�ʧ��
//			getDataFlag = 0;
//			GuiFillRect(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,\
//				DZModfiyWin.x + DZModfiyWin.wide - 2,\
//				DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"�޸�ʧ��");
//			flag = 1;
//		}
//	}
//	switch(keyStatus){
//	case UpKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag == 0){
//			if(keyIs < 7)
//				keyIs += 7;
//			else
//				keyIs -= 7;
//		}
//		flag = 1;break;
//	case DownKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag == 0){
//			if(keyIs >= 7)
//				keyIs -= 7;
//			else
//				keyIs += 7;
//		}
//		flag = 1;break;
//	case LeftKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){
//			if(keyIs == 0)
//				keyIs = dZModfiy.Items[dZModfiy.itemIs].flag - 1;
//			else
//				keyIs --; 
//		}
//		else{
//			if(keyIs == 0 || keyIs == 7){	
//				keyIs += 6;
//			}
//			else{
//				keyIs --;
//			}
//		}
//		flag = 1;break;
//	case RightKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){
//			if(keyIs == (dZModfiy.Items[dZModfiy.itemIs].flag - 1))
//				keyIs = 0;
//			else
//				keyIs ++; 
//		}
//		else{
//			if(keyIs == 6 || keyIs == 13)
//				keyIs -= 6;
//			else
//				keyIs ++;
//		}
//		flag = 1;break;
//	case OkKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){//�����
//			modfiyStr[0] = keyIs;
//			flag = 4;
//		}
//		else{
//			if(keyIs < 12 && keyIs != 6){
//				if(modfiyStrP < 12){
//					modfiyStr[modfiyStrP] = *modfiyKey[keyIs];
//					modfiyStrP ++;
//				}
//			}
//			else if(keyIs == 6){//esc
//				modfiyStr[1] = '\0';
//				modfiyStrP = 0;
//				keyIs = 0;
//				flag = 0;
//				userGUITopWindowHide();
//			}
//			else if(keyIs == 12){
//				if(modfiyStrP > 0){
//					modfiyStr[modfiyStrP] = '\0';
//					modfiyStrP --;
//				}			
//			}
//			else if(keyIs == 13){//ok
//				flag = 4;//�����
//			}
//		}break;
//	case CancelKey:
//		modfiyStr[1] = '\0';
//		modfiyStrP = 0;
//		keyIs = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		break;
//	default: break;
//	}
//}


///**
//  *@brief ��ֵ���ݴ���
//  *@param  None
//  *@retval None
//  */
//void DingZhiDataResult(uint8_t *pData,uint8_t *outData,const struct DingZhi1Item *items,\
//	uint8_t itemsNum,const struct ModbusCmd_ *modbusCmd)
//{
//	union{
//		float f32;
//		uint8_t u8[4];
//	}tempFloat;
//	
//	uint16_t mapAddr;
//	uint8_t i;
//	
//	for(i=0;i<itemsNum;i++){
//		mapAddr = items[i].addr - modbusCmd->mapAddr.addr16;
//		tempFloat.u8[3] = pData[4*mapAddr];
//		tempFloat.u8[2] = pData[4*mapAddr + 1];
//		tempFloat.u8[1] = pData[4*mapAddr + 2];
//		tempFloat.u8[0] = pData[4*mapAddr + 3];
//		if(items[i].flag){//����
//			if((uint32_t)tempFloat.f32 < items[i].flag){
//				outData[i*16] = (uint32_t)tempFloat.f32;
//			}
//			else{
//				outData[i*16] = 0;
//			}
//		}
//		else{
//			sprintf((char *)&outData[i*16],"%.3f",tempFloat.f32);
//		}
//	}
//}

///**
//  *@brief ͨ�ö�ֵ��ʾ����
//  *@param  Items ��ֵ�ṹָ��
//  *@param  itemsNum ��ֵ��
//  *@param  cmd ����
//  *@param  flag �����־
//  *@retval None
//  */
//void DZModfiyDisplay(const struct DingZhi1Item *Items,\
//	uint8_t itemsNum,const struct ModbusCmd_ *modbusCmd,uint8_t *flag)
//{
//	const uint8_t listCol[2][DZDISPLAYCOL] = {//���������
//		{73,63,20},{COL_LEFT_DISPLAY,COL_MID_DISPLAY,COL_MID_DISPLAY}
//	};
//	static SCROLL *Scroll;//������
//	static ListColSet colset;
//	static LIST  *list;//�б�ؼ�
//	static uint8_t **pText;//�б�����ָ��
//	static uint8_t *col1Data;//��ʾ����ָ��
//	static uint8_t getDataFlag;
//	uint8_t tempRetrun;
//	uint8_t i;
//	uint16_t j ;
//	uint16_t memMall;//�����ڴ����
//	uint32_t pData;
//	if(*flag == 0){//��ʼ��
//		memMall = 0;
//		list = (LIST  *)&userGUIBuff[memMall];
//		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		pText = (uint8_t **)&userGUIBuff[memMall];
//		memMall += itemsNum*DZDISPLAYCOL*4;
//		col1Data = &userGUIBuff[memMall];
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = itemsNum;
//		Scroll->lump = 1;
//		colset.colWide = (uint8_t *)listCol[0];
//		colset.colFlag = (uint8_t *)listCol[1];
//		list->x = 0;
//		list->y = 18;
//		list->wide = 156;
//		list->hight = 141;
//		list->row = itemsNum;
//		list->col = DZDISPLAYCOL;
//		list->drawRow = 0;
//		list->currentRow = 0;
//		list->flag = LIST_USEBORDER_H;
//		list->content = (uint8_t **)pText;
//		list->colSet = &colset;
//		*flag = 1;
//		for(j=0;j<itemsNum*16;j++){
//			col1Data[j] = 0;
//		}
//		getDataFlag = 0;
//	}
//	if(*flag == 1){
//		tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			modbusCmd->addr.addr16,modbusCmd->length.len16,&pData);
//		if(tempRetrun == ISTURE){//�����������		
//			DingZhiDataResult((uint8_t *)pData,col1Data,Items,itemsNum,modbusCmd);
//			*flag = 2;
//		}
//		else if(tempRetrun == ISFAILTURE){
//			getDataFlag = 0;
//		}
//		else{
//			GUIWiatMessage(30,65);
//		}
//	}
//	if(*flag == 2){//ָ���������ָ��
//		for(i=0;i<itemsNum;i++){
//			*(pText + i*DZDISPLAYCOL + 0) = Items[i].name;
//			if(Items[i].flag){//����
//				*(pText + i*DZDISPLAYCOL + 1) = \
//					Items[i].pContent[col1Data[i*16]];	
//			}
//			else{//float value
//				*(pText + i*DZDISPLAYCOL + 1) = &col1Data[i*16];
//			}
//			*(pText + i*DZDISPLAYCOL + 2) = Items[i].unit[0];
//		}
//		if((list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW + DISPLAYLISTROW <= list->row){
//			list->drawRow = (list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW;
//		}
//		else{
//			if(list->row < DISPLAYLISTROW){
//				list->drawRow = 0;
//			}
//			else{
//				list->drawRow = list->row - DISPLAYLISTROW;
//			}	
//		}
//		DrawList(list);
//		Scroll->lump = list->currentRow + 1;
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		*flag = 3;
//	}
//	if(keyStatus != CancelKey && *flag <= 1){
//		keyStatus = NoKey;
//	}
//	switch(keyStatus){
//	case UpKey:
//		if(list->currentRow > 0){
//			list->currentRow --;
//		}
//		*flag = 2;break;
//	case DownKey:
//		if(list->currentRow < list->row - 1){
//			list->currentRow ++;
//		}
//		*flag = 2;break;
//	case LeftKey:
//		if(list->currentRow >= DISPLAYLISTROW){
//			list->currentRow -= DISPLAYLISTROW;
//		}
//		else{
//			list->currentRow =0;
//		}
//		*flag = 2;break;
//	case RightKey:
//		if(list->currentRow + DISPLAYLISTROW < list->row){
//			list->currentRow += DISPLAYLISTROW;
//		}
//		else{
//			list->currentRow = list->row - 1;
//		}
//		*flag = 2;break;
//	case OkKey:
//		if(GetDingZhiSetOrSee() == SET_DINGZHI){//��ֵ�޸�
//			SetDingZhiModfiy(Items,(void *)modbusCmd,1,list->currentRow,\
//				&col1Data[list->currentRow*16]);
//			userGUIWindowAdd(&DZModfiyWin);
//			*flag = 2;
//		}break;
//	case CancelKey:
//		*flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief HomeWindow deal with function
//  *@param  None
//  *@retval None
//  */
//void HomeWindowFun(void)
//{
//	static uint8_t flag = 0;
//	static uint8_t currentTime[24];
//	struct YaoXinData_  tempyaoXin;
//	uint8_t i,j;
//	if(flag == 0){//������ҳ
//		GUI_LoadPic1(HomeWindow.x+1, HomeWindow.y+25,sojoImage, 158,30);
//		j = 110;
//		for(i=1;i<24;i+=2){
//			GuiHPointLine(HomeWindow.x+1,i,j,2,forecolor);
//			j -= 2;
//		}
//		for(i=25;i<55;i+=2){
//			GuiHPointLine(HomeWindow.x+1,i,159,2,forecolor);
//		}
//		j = 70;
//		for(i=55;i<78;i+=2){
//			GuiHPointLine(HomeWindow.x+j,i,159,2,forecolor);
//			j -= 2;
//		}
//		GuiFont12Align(HomeWindow.x+5,1,152,FONT_RIGHT,(uint8_t *)HMiVersion);
//		GuiHLine(HomeWindow.x+1,78,158,forecolor);
//		GuiFillRect(HomeWindow.x+1,81,159,94, forecolor);
//		GuiHLine(HomeWindow.x+1,96,158,forecolor);
//		
//		GuiFont12Align(HomeWindow.x+9,102,16,FONT_LEFT,"��");
//		GuiFont12Align(HomeWindow.x+9,121,16,FONT_LEFT,"Ϣ");
//		GuiFont12Align(HomeWindow.x+9,140,16,FONT_LEFT,"��");
//		GuiRLine(HomeWindow.x+30,96,158,forecolor);
//		
//		for(i=0;i<4;i++){
//			GuiFont12Align(HomeWindow.x+31,96+2+i*16,90,FONT_MID,"----");
//			GuiRPointLine(HomeWindow.x+31+91,96,96+15+i*16,2,forecolor);
//			GuiFont12Align(HomeWindow.x+31+93,96+2+i*16,34,FONT_MID,"--");
//			GuiHPointLine(HomeWindow.x+30,96+15+i*16,159,2,forecolor);
//		}
//		
//		GuiFont12Align(HomeWindow.x+31,96+34,90,FONT_MID,"����״̬");
//		flag = 1;
//	}
//	if(flag == 1){
//		TimeToString(currentTime);
//		GuiExchangeColor();
//		GuiFont12Align(HomeWindow.x+2,82,156,FONT_MID,currentTime);
//		GuiExchangeColor();
//		tempyaoXin = GetYaoxinStatus();
//		if(tempyaoXin.YaoXinData1.Bit.switchClose_h == 1){
//			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"��");
//		}
//		else{
//			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"��");
//		}
//		GuiUpdateDisplayAll();
//	}
//	switch(keyStatus){
//	case UpKey:
//	case DownKey:
//		userGUIWindowAdd(&YaoCe2Win);
//		flag = 0;
//		break;
//	case LeftKey:
//	case RightKey:
//		userGUIWindowAdd(&YaoxinWin);
//		flag = 0;
//		break;
//	case OkKey:
//		flag = 0;
//		userGUIWindowAdd(&MenuWindow);
//		break;
//	case CancelKey:break;
//	default: break;
//	}
//}

///**
//  *@brief Menu Window deal with function
//  *@param  None
//  *@retval None
//  */
//static void MenuWindowFun(void)
//{
//	if(GetMenuManagepMenu() == 0){
//		userGUIMenuAdd(&mianMenu);
//	}
//	userGUIMenuDisplay();
//}

///**
//  *@brief mianMenu deal with function ���˵�
//  *@param  None
//  *@retval None
//  */
//static void mianMenuFun(void)
//{
//	static uint8_t flag;
//	static uint8_t passWordState;
//	
//	if(flag == 0){
//		passWordState = 0;
//	}
//	if(flag == 1){
//		switch(mianMenu.currentItem){
//		case 0://��Ϣ��ѯ
//		case 4://�汾��Ϣ
//			flag = 3;
//			break;
//		case 1://��ֵ����
//		case 2://��������
//		case 3://�����·�
//			SetPassWordWin(0,&passWordState);
//			userGUIWindowAdd(&PassWordWin);
//			flag = 2;
//			break;
//		default:break;
//		}			
//	}
//	else if(flag == 2){
//		if(passWordState == 1){
//			flag = 3;
//		}
//		else{
//			flag = 0;
//		}
//	}
//	if(flag == 3){
//		switch(mianMenu.currentItem){
//		case 0:userGUIMenuAdd(&MenuM0);break;//��Ϣ��ѯ
//		case 1:userGUIMenuAdd(&MenuM1);break;//��ֵ����
//		case 2:userGUIMenuAdd(&MenuM2);break;//��������
//		case 3:userGUIWindowAdd(&CmdSendWin);break;//�����·�
//		case 4:userGUIWindowAdd(&VersionWin);break;//�汾��Ϣ
//		default:break;
//		}
//	}
//	switch(keyStatus){
//	case UpKey:
//		userGUIMenuIremModfiy(0);
//		break;
//	case DownKey:
//		userGUIMenuIremModfiy(1);
//		break;
//	case LeftKey:
//	case CancelKey:
//		flag = 0;
//		userGUIMenuHide();
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		break;
//	case RightKey:
//	case OkKey:
//		if(flag == 0){
//			flag = 1;
//		}
//		break;
//	}
//	if(flag == 3){
//		flag = 0;
//	}
//}

///**
//  *@brief MenuM0 deal with function ��Ϣ��ѯ
//  *@param  None
//  *@retval None
//  */
//static void MenuM0Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:userGUIMenuIremModfiy(0);break;	
//	case DownKey:userGUIMenuIremModfiy(1);break;	
//	case LeftKey:
//	case CancelKey:
//		SetDingZhiSetOrSee(NULL_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��
//		userGUIMenuHide();break;
//	case RightKey:
//	case OkKey:
//		SetDingZhiSetOrSee(SEE_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��ѯ
//		switch(MenuM0.currentItem){
//		case 0:userGUIWindowAdd(&YaoxinWin);break;//ң�Ų�ѯ
//		case 1:userGUIMenuAdd(&MenuM0S1);break;//ң���ѯ
//		case 2:userGUIWindowAdd(&SOEWin);break;//SOE��ѯ
//		case 3:userGUIWindowAdd(&FaultEventWin);break;//�����¼�
//		case 4:userGUIMenuAdd(&MenuM0S4);break;//��ֵ��ѯ
//		case 5:userGUIMenuAdd(&MenuM0S5);break;//���ò�ѯ
//		default:break;
//		}
//	}
//}

///**
//  *@brief MenuM1 deal with function ��ֵ����
//  *@param  None
//  *@retval None
//  */
//static void MenuM1Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:
//		userGUIMenuIremModfiy(0);
//		break;
//	case DownKey:
//		userGUIMenuIremModfiy(1);
//		break;
//	case LeftKey:
//	case CancelKey:
//		SetDingZhiSetOrSee(NULL_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��
//		userGUIMenuHide();
//		break;
//	case RightKey:
//	case OkKey:
//		SetDingZhiSetOrSee(SET_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ����
//		switch(MenuM1.currentItem){
//		case 0:userGUIMenuAdd(&MenuM1S0);break;//��������
//		case 1:userGUIWindowAdd(&LogicalFunWin);break;//�߼�����
//		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//Խ�߱���
//		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//�ع���
//		case 4:userGUIWindowAdd(&OverLoadWin);break;//������
//		case 5:userGUIWindowAdd(&OverVoltageWin);break;//����ѹ
//		case 6:userGUIWindowAdd(&BatterySetWin);break;//�������
//		case 7:userGUIWindowAdd(&AutoResetWin);break;//�Զ�����
//		case 8:userGUIMenuAdd(&MenuM1S8);break;//��������
//		default:break;
//		}
//	}
//}

///**
//  *@brief  MenuM2 deal with function ��������
//  *@param  None
//  *@retval None
//  */
//static void MenuM2Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:
//		userGUIMenuIremModfiy(0);
//		break;
//	case DownKey:
//		userGUIMenuIremModfiy(1);
//		break;
//	case LeftKey:
//	case CancelKey:
//		SetDingZhiSetOrSee(NULL_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��
//		userGUIMenuHide();
//		break;
//	case RightKey:
//	case OkKey:
//		SetDingZhiSetOrSee(SET_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ����
//		switch(MenuM2.currentItem){
//		case 0:userGUIWindowAdd(&BasicSetWin);break;
//		case 1:userGUIWindowAdd(&ZeroDriftWin);break;
//		case 2:userGUIWindowAdd(&DeadZoneWin);break;
//		default:break;
//		}
//	}
//}

///**
//  *@brief yaoce deal with function
//  *@param  None
//  *@retval None
//  */
//static void MenuM0S1Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:userGUIMenuIremModfiy(0);break;
//	case DownKey:userGUIMenuIremModfiy(1);break;
//	case LeftKey:
//	case CancelKey:
//		userGUIMenuHide();break;
//	case RightKey:
//	case OkKey:
//		switch(MenuM0S1.currentItem){
//		case 0:userGUIWindowAdd(&YaoCe1Win);break;//һ��ң��
//		case 1:userGUIWindowAdd(&YaoCe2Win);break;//����ң��
//		case 2:userGUIWindowAdd(&HarmonicWin);break;//г��
//		default:break;
//		}
//	}
//}

///**
//  *@brief MenuM0S4 deal with function ��ֵ��ѯ
//  *@param  None
//  *@retval None
//  */
//static void MenuM0S4Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:
//		userGUIMenuIremModfiy(0);
//		break;
//	case DownKey:
//		userGUIMenuIremModfiy(1);
//		break;
//	case LeftKey:
//	case CancelKey:
//		userGUIMenuHide();
//		break;
//	case RightKey:
//	case OkKey:
//		switch(MenuM0S4.currentItem){
//		case 0:userGUIMenuAdd(&MenuM0S4S0);break;//��������
//		case 1:userGUIWindowAdd(&LogicalFunWin);break;//�߼�����
//		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//Խ�߱���
//		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//�ع���
//		case 4:userGUIWindowAdd(&OverLoadWin);break;//������
//		case 5:userGUIWindowAdd(&OverVoltageWin);break;//����ѹ
//		case 6:userGUIWindowAdd(&BatterySetWin);break;//�������
//		case 7:userGUIWindowAdd(&AutoResetWin);break;//�Զ�����
//		case 8:userGUIMenuAdd(&MenuM0S4S8);break;//��������
//		default:break;
//		}
//	}
//}

///**
//  *@brief MenuM0S4 deal with function ���ò�ѯ
//  *@param  None
//  *@retval None
//  */
//static void MenuM0S5Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:
//		userGUIMenuIremModfiy(0);
//		break;
//	case DownKey:
//		userGUIMenuIremModfiy(1);
//		break;
//	case LeftKey:
//	case CancelKey:
//		userGUIMenuHide();
//		break;
//	case RightKey:
//	case OkKey:
//		switch(MenuM0S5.currentItem){
//		case 0:userGUIWindowAdd(&BasicSetWin);break;
//		case 1:userGUIWindowAdd(&ZeroDriftWin);break;
//		case 2:userGUIWindowAdd(&DeadZoneWin);break;
//		default:break;
//		}
//	}
//}

///**
//  *@brief MenuM0S4S0 deal with function ��������
//  *@param  None
//  *@retval None
//  */
//static void MenuM0S4S0Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:userGUIMenuIremModfiy(0);break;	
//	case DownKey:userGUIMenuIremModfiy(1);break;	
//	case LeftKey:
//	case CancelKey:
//		userGUIMenuHide();break;
//	case RightKey:
//	case OkKey:
//		switch( MenuM0S4S0.currentItem){
//		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//������
//		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//�������
//		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//��·��
//		default:break;
//		}
//	}
//}
///**
//  *@brief MenuM1S0 deal with function ��������
//  *@param  None
//  *@retval None
//  */
//static void MenuM1S0Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:userGUIMenuIremModfiy(0);break;	
//	case DownKey:userGUIMenuIremModfiy(1);break;	
//	case LeftKey:
//	case CancelKey:
//		userGUIMenuHide();break;
//	case RightKey:
//	case OkKey:
//		switch( MenuM1S0.currentItem){
//		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//������
//		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//�������
//		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//��·��
//		default:break;
//		}
//	}
//}

///**
//  *@brief MenuM1S8 deal with function ����
//  *@param  None
//  *@retval None
//  */
//static void MenuM1S8Fun(void)
//{
//	switch(keyStatus){
//	case UpKey:userGUIMenuIremModfiy(0);break;	
//	case DownKey:userGUIMenuIremModfiy(1);break;	
//	case LeftKey:
//	case CancelKey:
//		userGUIMenuHide();break;
//	case RightKey:
//	case OkKey:
//		switch( MenuM1S8.currentItem){
//		case 0:userGUIWindowAdd(&RingUniteWin);break;//�ϻ�
//		case 1:userGUIWindowAdd(&BreakDownWin);break;//����Ͷ��
//		default:break;
//		}
//	}
//}

///**
//  *@brief CmdSend deal with function �����·�
//  *@param  None
//  *@retval None
//  */
//static void CmdSendWinFun(void)
//{
//	const  uint8_t listCol[2][1] = {{156},{COL_MID_DISPLAY}};
//	static uint8_t flag = 0;//�����־
//	static SCROLL *Scroll;//������
//	static ListColSet colset;
//	static LIST  *list;//�б�ؼ�
//	static uint8_t **pText;//�б�����ָ��
//	
//	static uint8_t messageFlag = 0;//�����־
//	uint16_t memMall;
//	static int8_t currentNum;
//	uint8_t i;
//	if(flag == 0){//��ʼ���������ڴ�
//		memMall = 0;	
//		list = (LIST  *)&userGUIBuff[memMall];
//		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		pText = (uint8_t **)&userGUIBuff[memMall];
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = CmdSend_NUM;
//		Scroll->lump = 1;
//		colset.colWide = (uint8_t *)listCol[0];
//		colset.colFlag = (uint8_t *)listCol[1];
//		list->x = 0;
//		list->y = 18;
//		list->wide = 156;
//		list->hight = 141;
//		list->row = CmdSend_NUM;
//		list->col = 1;
//		list->drawRow = 0;
//		list->currentRow = 0;
//		list->flag = LIST_USEBORDER_H;
//		list->content = (uint8_t **)pText;
//		list->colSet = &colset;	
//		currentNum = 0;
//		flag = 1;
//	}
//	if(flag == 1){//��ȡ��Ӧ����	
//		for(i = 0;i < CmdSend_NUM;i++){
//			*(pText + i) = CmdSendItems[i].name;
//		}
//		flag = 2;
//	}
//	if(flag == 2){//��ʾ
//		list->currentRow = currentNum;
//		Scroll->lump = currentNum + 1;
//		if((list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW + DISPLAYLISTROW <= list->row){
//			list->drawRow = (list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW;
//		}
//		else{
//			if(list->row < DISPLAYLISTROW){
//				list->drawRow = 0;
//			}
//			else{
//				list->drawRow = list->row - DISPLAYLISTROW;
//			}	
//		}
//		DrawList(list);
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		flag = 3;
//	}
//	if(flag == 3){
//	}
//	if(flag == 4){
//		if(messageFlag == 1){
//			FillYaoKongFIFO(ModBusSlaveAddr1,\
//				&ModbuscmdYaoKong[CmdSendItems[currentNum].M_mapNum]);
//		}
//		flag = 2;
//	}
//	switch(keyStatus){
//	case LeftKey:
//	case UpKey:
//		currentNum --;
//		if(currentNum < 0){
//			currentNum = Scroll->max - 1;
//		}
//		flag = 2;
//		break;
//	case RightKey:
//	case DownKey:
//		currentNum ++;
//		if(currentNum > Scroll->max - 1){
//			currentNum = 0;
//		}
//		flag = 2;
//		break;
//	case OkKey:
//		flag = 4;
//		messageFlag = 0;
//		SetMessageWin(23,40,"ȷ��ִ��?",&messageFlag);
//		userGUIWindowAdd(&MessageWin);
//		break;
//	case CancelKey:
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief  ���в������ݴ���
//  *@param  pData ����������
//  *@param  outData �����������
//  *@param  eachSize ������С
//  *@retval None
//  */
//static void ConstParDataResult(uint8_t *pData,uint8_t *outData,uint8_t eachSize)
//{
//	uint8_t i;
//	int8_t j;
//	for(i=0;i<CONSTPARDISPLAYROW;i++){
//		strncpy((char *)&outData[i*(eachSize+1)],(char *)&pData[i*eachSize],eachSize);
//		for(j = 23;j >= 0;j--){
//			if(outData[i*eachSize + j] == ' '){
//				outData[i*eachSize + j] = '\0';
//			}
//			else{
//				outData[i*(eachSize+1) + 24] = '\0';
//				break;
//			}	
//		}
//	}
//}

///**
//  *@brief  �汾��Ϣ
//  *@param  None
//  *@retval None
//  */
//static void VersionWinFun(void)
//{
//	static uint8_t flag = 0;//�����־
//	static SCROLL *Scroll;//������
//	static uint8_t *col1Data;//��ʾ����ָ��		
//	static uint8_t getDataFlag;
//	static int8_t currentNum;
//	uint32_t pData;
//	uint8_t tempRetrun;
//	uint8_t len;
//	uint16_t memMall;
//	uint8_t i,y;
//	if(flag == 0){//��ʼ���������ڴ�
//		memMall = 0;
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		col1Data = &userGUIBuff[memMall];
//		
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = ConstParameter_NUM/CONSTPARDISPLAYROW +\
//			(ConstParameter_NUM%CONSTPARDISPLAYROW >0 ?1:0);
//		Scroll->lump = 1;
//		currentNum = 0;
//		getDataFlag = 0;
//		flag = 1;
//	}
//	if(flag == 1){//��ȡ��Ӧ����
//		len = ModbuscmdDingZhi[ConstParametereNum].length.len16*CONSTPARDISPLAYROW;
//		tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			ModbuscmdDingZhi[ConstParametereNum].addr.addr16 +\
//			currentNum*CONSTPARDISPLAYROW,len,&pData);//�˴�24�ֽ�һ����ַ
//		if(tempRetrun == ISTURE){//�����������
//			ConstParDataResult((uint8_t *)pData,col1Data,\
//				ModbuscmdDingZhi[ConstParametereNum].length.len16*2);
//			flag = 2;
//		}
//		else if(tempRetrun == ISFAILTURE){
//			getDataFlag = 0;
//		}
//		else{
//			GUIWiatMessage(30,65);
//		}
//	}
//	if(flag == 2){//��ʾ
//		y = VersionWin.y + 18;
//		GuiFillRect(VersionWin.x+1,y,155,158, backcolor);
//		for(i=0;i<CONSTPARDISPLAYROW;i++){
//			if((i + currentNum*CONSTPARDISPLAYROW) >= ConstParameter_NUM){
//				GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//ˮƽ��
//				break;
//			}
//			GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//ˮƽ��		
//			GuiFillRect(VersionWin.x+1,y + i*28+1,155,y + i*28+14, forecolor);
//			GuiExchangeColor();
//			GuiFont12Align(VersionWin.x + 2,y + i*28+2,\
//				133,FONT_LEFT,ConstParItems[i+currentNum*CONSTPARDISPLAYROW].name);
//			GuiExchangeColor();
//			GuiFont12Align(VersionWin.x+2,y + i*28+15,153,\
//				FONT_MID,&col1Data[i*25]);
//		}
//		Scroll->lump = currentNum + 1;
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		flag = 3;
//	}
//	if(flag == 3){
//	}
//	if(keyStatus != CancelKey && flag <= 1){
//		keyStatus = NoKey;
//	}
//	switch(keyStatus){
//	case LeftKey:
//	case UpKey:
//		currentNum --;
//		if(currentNum < 0){
//			currentNum = Scroll->max - 1;
//		}
//		flag = 1;
//		break;
//	case RightKey:
//	case DownKey:
//		currentNum ++;
//		if(currentNum > Scroll->max - 1){
//			currentNum = 0;
//		}
//		flag = 1;
//		break;
//	case OkKey:break;
//	case CancelKey:
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief  ������
//  *@param  None
//  *@retval None
//  */
//static void InterphaseOverFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(InterphaseOverItems,InterphaseOver_NUM - 3,\
//		&ModbuscmdDingZhi[InterphaseOverNum],&flag);
//}

///**
//  *@brief �������
//  *@param  None
//  *@retval None
//  */
//static void ZeroSequenceOverFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(ZeroSequenceOverItems,ZeroSequenceOver_NUM,\
//		&ModbuscmdDingZhi[ZeroSequenceOverNum],&flag);
//}

///**
//  *@brief ��·��
//  *@param  None
//  *@retval None
//  */
//static void CutoutSwitchFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(CutoutSwitchItems,CutoutSwitch_NUM,\
//		&ModbuscmdDingZhi[CutoutSwitchNum],&flag);
//}

///**
//  *@brief �߼�����
//  *@param  None
//  *@retval None
//  */
//static void LogicalFunFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(LoadSwitchItems,LoadSwitch_NUM,\
//		&ModbuscmdDingZhi[LoadSwitchNum],&flag);
//}

///**
//  *@brief Խ�߱���
//  *@param  None
//  *@retval None
//  */
//static void OverLineWarnFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(ThresholdAlarmItems,ThresholdAlarm_NUM,\
//		&ModbuscmdDingZhi[ThresholdAlarmNum],&flag);
//}

///**
//  *@brief �ع���
//  *@param  None
//  *@retval None
//  */
//static void OverLoadMuchFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(OverloadMuchItems,OverloadMuch_NUM,\
//		&ModbuscmdDingZhi[OverloadNum],&flag);
//}

///**
//  *@brief ������
//  *@param  None
//  *@retval None
//  */
//static void OverLoadFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(OverloadItems,Overload_NUM,\
//		&ModbuscmdDingZhi[Overload2Num],&flag);
//}

///**
//  *@brief ����ѹ
//  *@param  None
//  *@retval None
//  */
//static void OverVoltageFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(OverVoltageItems,OverVoltage_NUM,\
//		&ModbuscmdDingZhi[OverVoltageNum],&flag);
//}

///**
//  *@brief �������
//  *@param  None
//  *@retval None
//  */
//static void BatterySetFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(BatterySetItems,BatterySet_NUM,\
//		&ModbuscmdDingZhi[BatteryActivationNum],&flag);
//}

///**
//  *@brief �Զ�����
//  *@param  None
//  *@retval None
//  */
//static void AutoResetFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(AutoResetItems,AutoReset_NUM,\
//		&ModbuscmdDingZhi[AutoResetNum],&flag);
//}

///**
//  *@brief �ϻ�
//  *@param  None
//  *@retval None
//  */
//static void RingUniteFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(RingUniteItems,RingUnite_NUM,\
//		&ModbuscmdDingZhi[RingUniteNum],&flag);
//}

///**
//  *@brief ����Ͷ��
//  *@param  None
//  *@retval None
//  */
//static void BreakDownFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(BreakDownItems,BreakDown_NUM,\
//		&ModbuscmdDingZhi[BreakDownNum],&flag);
//}

///**
//  *@brief ң�Ŵ�����
//  *@param  None
//  *@retval None
//  */
//static void YaoxinFun(void)
//{
//	const  uint8_t listCol[2][2] = {{90,66},{COL_LEFT_DISPLAY,COL_MID_DISPLAY}};
//	static uint8_t flag = 0;//�����־
//	static SCROLL *Scroll;//������
//	static ListColSet colset;
//	static LIST  *list;//�б�ؼ�
//	static uint8_t **pText;//�б�����ָ��
//	static uint8_t *col1Data;//��ʾ����ָ��	
//	static struct YaoXinData_ *yaoXin;
//	struct YaoXinData_ tempYaoXin;
//	
//	uint16_t memMall;
//	uint8_t i;
//	uint8_t itemsNum;
//	if(flag == 0){//��ʼ���������ڴ�
//		memMall = 0;
//		itemsNum = sizeof(yaoXinItems)/sizeof(struct YaoXinItem);
//		
//		list = (LIST  *)&userGUIBuff[memMall];
//		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		pText = (uint8_t **)&userGUIBuff[memMall];
//		memMall += itemsNum*2*4;
//		yaoXin = (struct YaoXinData_ *)&userGUIBuff[memMall];
//		memMall += sizeof(struct YaoXinData_) + (4 - sizeof(struct YaoXinData_)%4);	
//		col1Data = &userGUIBuff[memMall];

//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = itemsNum/DISPLAYLISTROW+(itemsNum%DISPLAYLISTROW >0 ?1:0);
//		Scroll->lump = 1;
//		colset.colWide = (uint8_t *)listCol[0];
//		colset.colFlag = (uint8_t *)listCol[1];
//		list->x = 0;
//		list->y = 18;
//		list->wide = 156;
//		list->hight = 141;
//		list->row = itemsNum;
//		list->col = 2;
//		list->drawRow = 0;
//		list->currentRow = 255;
//		list->flag = LIST_USEBORDER_H;
//		list->content = (uint8_t **)pText;
//		list->colSet = &colset;	
//		*yaoXin = GetYaoxinStatus();//��ȡң��
//		flag = 1;
//	}
//	if(flag == 1){//��ȡ��Ӧ����
//		itemsNum = sizeof(yaoXinItems)/sizeof(struct YaoXinItem);
//		for(i = 0;i < itemsNum;i++){
//			col1Data[i] = (yaoXin->YaoXinData1.All>>i)&0x01;
//		}	
//		for(i = 0;i < itemsNum;i++){
//			*(pText + i*2 + 0) = yaoXinItems[i].name;
//			if(col1Data[i] == 0)
//				*(pText + i*2 + 1) = yaoXinItems[i].status[0];
//			else
//				*(pText + i*2 + 1) = yaoXinItems[i].status[1];
//		}
//		flag = 2;
//	}
//	if(flag == 2){//��ʾ
//		if(list->drawRow == list->row - DISPLAYLISTROW){
//			Scroll->lump = Scroll->max;
//		}
//		else{
//			Scroll->lump = list->drawRow/DISPLAYLISTROW + 1;
//		}
//		DrawList(list);
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		flag = 3;
//	}
//	if(flag == 3){//���ң�ű仯
//		tempYaoXin = GetYaoxinStatus();
//		if(tempYaoXin.YaoXinData1.All != yaoXin->YaoXinData1.All){
//			*yaoXin = tempYaoXin;
//			flag = 1;
//		}
//	}	
//	switch(keyStatus){
//	case UpKey:
//		if(list->drawRow > DISPLAYLISTROW)
//			list->drawRow -= DISPLAYLISTROW;
//		else
//			list->drawRow = 0;
//		flag = 2;
//		break;	
//	case DownKey:	
//		list->drawRow += DISPLAYLISTROW;
//		if(list->drawRow + DISPLAYLISTROW > list->row){
//			list->drawRow = list->row - DISPLAYLISTROW;
//		}
//		flag = 2;
//		break;
//	case LeftKey:break;
//	case RightKey:break;
//	case OkKey:break;
//	case CancelKey:
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief ң�����ݴ���
//  *@param  pData ����������
//  *@param  outData �����������
//  *@param  items ң����ʾ��
//  *@param  itemsNum ң������
//  *@param  items modbus����
//  *@retval None
//  */
//void yaoCeDataResult(uint8_t *pData,uint8_t *outData,const struct YaoCeItem *items,\
//	uint8_t itemsNum,const struct ModbusCmd_ *modbusCmd)
//{
//	union{
//		float f32;
//		uint8_t u8[4];
//	}tempFloat;
//	
//	uint16_t mapAddr;
//	uint8_t i;
//	
//	for(i=0;i<itemsNum;i++){
//		mapAddr = items[i].addr - modbusCmd->mapAddr.addr16;
//		tempFloat.u8[3] = pData[4*mapAddr];
//		tempFloat.u8[2] = pData[4*mapAddr + 1];
//		tempFloat.u8[1] = pData[4*mapAddr + 2];
//		tempFloat.u8[0] = pData[4*mapAddr + 3];
//		if(((uint8_t)isnan(tempFloat.f32)) != 0){
//			sprintf((char *)&outData[i*16],"%s","NaN");
//		}
//		else{
//			sprintf((char *)&outData[i*16],"%.3f",tempFloat.f32);
//		}
//		
//	}
//}

///**
//  *@brief ң�⴦����ʾ
//  *@param  items ң����ʾ��
//  *@param  itemsNum ң������
//  *@param  items modbus����
//  *@retval None
//  */
//void yaoCeDisplay(const struct YaoCeItem *items,uint8_t itemsNum,\
//	const struct ModbusCmd_ *modbusCmd)
//{
//	const uint8_t listCol[2][YaoCeDISPLAYCOL] = {//���������
//		{50,75,31},{COL_LEFT_DISPLAY,COL_MID_DISPLAY,COL_MID_DISPLAY}};
//	static uint8_t flag = 0;//�����־
//	static SCROLL *Scroll;//������
//	static ListColSet colset;
//	static LIST  *list;//�б�ؼ�
//	static uint8_t **pText;//�б�����ָ��
//	static uint8_t *col1Data;//��ʾ����ָ��		
//	static uint32_t YaoCeTick;//ѭ����ʱ
//	static uint8_t getDataFlag;
//	
//	uint16_t memMall;
//	uint8_t tempRetrun;
//	uint8_t i;
//	uint32_t pData;
//		
//	if(flag == 0){//��ʼ���������ڴ�
//		memMall = 0;	
//		list = (LIST  *)&userGUIBuff[memMall];
//		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		pText = (uint8_t **)&userGUIBuff[memMall];
//		memMall += itemsNum*YaoCeDISPLAYCOL*4;
//		col1Data = &userGUIBuff[memMall];
//		
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = itemsNum/DISPLAYLISTROW+(itemsNum%DISPLAYLISTROW >0 ?1:0);
//		Scroll->lump = 1;
//		colset.colWide = (uint8_t *)listCol[0];
//		colset.colFlag = (uint8_t *)listCol[1];
//		list->x = 0;
//		list->y = 18;
//		list->wide = 156;
//		list->hight = 141;
//		list->row = itemsNum;
//		list->col = YaoCeDISPLAYCOL;
//		list->drawRow = 0;
//		list->currentRow = 255;
//		list->flag = LIST_USEBORDER_H;
//		list->content = (uint8_t **)pText;
//		list->colSet = &colset;
//		//��ȡ������
//		flag = 1;
//		getDataFlag = 0;
//	}
//	if(flag == 1){
//		tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			modbusCmd->addr.addr16,\
//			modbusCmd->length.len16,&pData);
//		if(tempRetrun == ISTURE){//�����������		
//			yaoCeDataResult((uint8_t *)pData,col1Data,items,itemsNum,modbusCmd);
//			flag = 2;
//		}
//		else if(tempRetrun == ISFAILTURE){
//			getDataFlag = 0;
//		}
//		else{
//				GUIWiatMessage(30,65);
//		}
//		for(i = 0;i < itemsNum;i++){
//			*(pText + i*YaoCeDISPLAYCOL + 0) =items[i].name;
//			*(pText + i*YaoCeDISPLAYCOL + 1) = &col1Data[i*16];
//			*(pText + i*YaoCeDISPLAYCOL + 2) = items[i].unit[0];
//		}
//	}
//	if(flag == 2){//��ʾ	
//		if(list->drawRow == list->row - DISPLAYLISTROW){
//			Scroll->lump = Scroll->max;
//		}
//		else{
//			Scroll->lump = list->drawRow/DISPLAYLISTROW + 1;
//		}
//		DrawList(list);
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		flag = 3;
//	}
//	if(flag == 3){
//		if(GetIntervalTick(YaoCeTick) > YAOCE_UPDATATIME){//������
//			tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//				modbusCmd->addr.addr16,\
//				modbusCmd->length.len16,&pData);
//			if(tempRetrun == ISTURE){//�����������
//				yaoCeDataResult((uint8_t *)pData,col1Data,items,itemsNum,modbusCmd);
//				YaoCeTick = getCurrentTick();
//				flag = 2;
//			}
//			else if(tempRetrun == ISFAILTURE){
//				getDataFlag = 0;
//			}
//		}
//	}
//	switch(keyStatus){
//	case UpKey:
//		if(flag > 1){
//			if(list->drawRow > DISPLAYLISTROW)
//				list->drawRow -= DISPLAYLISTROW;
//			else
//				list->drawRow = 0;
//			flag = 2;
//			break;
//		}	
//	case DownKey:
//		if(flag > 1){
//			list->drawRow += DISPLAYLISTROW;
//			if(list->drawRow + DISPLAYLISTROW > list->row){
//				list->drawRow = list->row - DISPLAYLISTROW;
//			}
//			flag = 2;
//			break;
//		}
//	case LeftKey:break;
//	case RightKey:break;
//	case OkKey:break;
//	case CancelKey:
//		getDataFlag = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief һ��ң�⴦����
//  *@param  None
//  *@retval None
//  */
//static void YaoCe1Fun(void)
//{
//	yaoCeDisplay(yaoCe1Items,YAOCE1_NUM,&ModbuscmdYaoCe[0]);
//}

///**
//  *@brief ����ң�⴦����
//  *@param  None
//  *@retval None
//  */
//static void YaoCe2Fun(void)
//{
//	yaoCeDisplay(yaoCe2Items,YAOCE2_NUM,&ModbuscmdYaoCe[0]);
//}
///**
//  *@brief г��������
//  *@param  None
//  *@retval None
//  */
//static void HarmonicFun(void)
//{
//	yaoCeDisplay(yaoCe3Items,HARMONIC_NUM,&ModbuscmdYaoCe[1]);
//}

///**
//  *@brief SOE���ݴ���
//  *@param  pData Ҫ���������
//  *@param  eachSoeSize ��λ��С
//  *@param  *SOE[] ������
//  *@retval None
//  */
//static void SOEDataResult(uint8_t *pData,uint8_t eachSoeSize,struct SOEDisplay *SOE[])
//{
//	uint32_t tempaddr,tempaddr2;
//	uint8_t i,j;
//	
//	for(j=0;j<SOEDISPLAYROW;j++){
//		tempaddr = (pData[j*eachSoeSize+0]<<8)+pData[j*eachSoeSize+1];
//		if(pData[j*eachSoeSize+2]>=0x20){//״̬λ����϶�
//			if((pData[j*eachSoeSize+2]&0x10) == 1){
//				tempaddr2 = 0x00060000|tempaddr;//ң��ȡ��
//			}
//			else{
//				switch(pData[j*eachSoeSize+2]&0x03){
//				case 0x01://��բ
//					switch(pData[j*eachSoeSize+2]&0x08){
//					case 0x00://ѡ��
//						tempaddr2 = 0x00000000|tempaddr;
//						break;
//					case 0x08://ִ��
//						tempaddr2 = 0x00010000|tempaddr;
//						break;;
//					}break;
//				case 0x02://��բ
//					switch(pData[j*eachSoeSize+2]&0x08){
//					case 0x00://ѡ��
//						tempaddr2 = 0x00020000|tempaddr;
//						break;
//					case 0x08://ִ��
//						tempaddr2 = 0x00030000|tempaddr;
//						break;
//					}break;
//				case 0x03://�ϻ�
//					switch(pData[j*eachSoeSize+2]&0x08){
//					case 0x00://ѡ��
//						tempaddr2 = 0x00040000|tempaddr;
//						break;
//					case 0x08://ִ��
//						tempaddr2 = 0x00050000|tempaddr;
//						break;
//					}break;
//				}
//			}
//			for(i = 0;i<SOE_NUM;i++){
//				if(tempaddr2 == SOEItems[i].addr){
//					SOE[j]->SOEItem = i;
//					SOE[j]->itemValue = (pData[j*eachSoeSize+2]&0x04)?1:0;
//					break;
//				}
//			}
//			if(i == SOE_NUM){
//				SOE[j]->SOEItem = 255;//255��ʾ��Ч
//			}
//		}
//		else{
//			tempaddr2 = (uint32_t)(tempaddr&0x0000FFFF);
//			for(i = 0;i<SOE_NUM;i++){
//				if(tempaddr2 == SOEItems[i].addr){
//					SOE[j]->SOEItem = i;
//					SOE[j]->itemValue = pData[j*eachSoeSize+2] - 1;//�� 1��2 ����״̬ת���� 0 1 �����������
//					if(SOE[j]->itemValue > 2){
//						SOE[j]->itemValue = 0;
//					}
//					break;
//				}
//			}
//			if(i == SOE_NUM){
//				SOE[j]->SOEItem = 255;//255��ʾ��Ч
//			}
//		}
//		sprintf((char *)SOE[j]->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
//			(pData[j*eachSoeSize+9]%100),(pData[j*eachSoeSize+8]&0x3F),\
//			pData[j*eachSoeSize+7]&0x1F,(pData[j*eachSoeSize+6]&0x3F),\
//			(pData[j*eachSoeSize+5]&0x3F),\
//			((pData[j*eachSoeSize+4]<<8) + pData[j*eachSoeSize+3])/1000,
//			((pData[j*eachSoeSize+4]<<8) + pData[j*eachSoeSize+3])%1000);
//	}
//}

///**
//  *@brief SOE������
//  *@param  None
//  *@retval None
//  */
//static void SOEFun(void)
//{
//	static struct SOEDisplay *SOE[SOEDISPLAYROW];
//	static uint8_t flag = 0;
//	static SCROLL Scroll ={156,18,141,2,1};//������
//	static uint8_t getDataFlag = 0;
//	static uint16_t offestAddr = 0;//SOEƫ�Ƶ�ַ
//	static int16_t currentNum;
//	
//	uint32_t pData;//��Ž�������ָ��
//	union yaoXinTimeflag dataUpdateFlag;
//	uint8_t tempRetrun;
//	uint16_t offest;
//	uint8_t i;
//	uint16_t y;
//	Scroll.max = GetSoeAllNum();
//	if(Scroll.max == 0){
//		GuiFont12Align(SOEWin.x+2,SOEWin.y + 30,\
//			SOEWin.x+SOEWin.wide-4,FONT_MID,"��ǰû��SOE");
//		GuiUpdateDisplayAll();
//	}
//	else{
//		if(flag == 0){//��ʼ���������ڴ�
//			for(i=0;i<SOEDISPLAYROW;i++){//��SOE������ʾ������
//				SOE[i] = (struct SOEDisplay *)&userGUIBuff[i*sizeof(struct SOEDisplay)];
//			}
//			flag = 1;
//			getDataFlag = 0;
//		}
//		if(flag == 1){//��������
//			offestAddr = currentNum*(ModbuscmdFIFO[FIFO_SOE_NUM].length.len16/2)*SOEDISPLAYROW;
//			tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//				ModbuscmdFIFO[FIFO_SOE_NUM].addr.addr16+offestAddr,\
//				ModbuscmdFIFO[FIFO_SOE_NUM].length.len16*SOEDISPLAYROW,&pData);
//			if(tempRetrun == ISTURE){
//				SOEDataResult((uint8_t *)pData,ModbuscmdFIFO[FIFO_SOE_NUM].length.len16,SOE);
//				offest = offestAddr/(ModbuscmdFIFO[FIFO_SOE_NUM].length.len16/2) + 1;
//				for(i=0;i<SOEDISPLAYROW;i++){
//					sprintf((char *)SOE[i]->itemNum,"%d",offest + i);
//				}
//				flag = 2;
//			}
//			else if(tempRetrun == ISFAILTURE){
//				getDataFlag = 0;
//			}
//			else{
//				GUIWiatMessage(30,65);
//			}
//		}
//		if(flag == 2){
//			y = SOEWin.y + 18;
//			GuiFillRect(SOEWin.x+1,y,155,158, backcolor);
//			for(i=0;i<SOEDISPLAYROW;i++){
//				if(SOE[i]->SOEItem == 255){//��⵽��ЧSOE,����
//					GuiHLine(SOEWin.x,y+i*28,155,forecolor);
//					break;
//				}
//				GuiHLine(SOEWin.x,y+i*28+0,155,forecolor);//ˮƽ��
//				GuiFillRect(SOEWin.x+1,y+i*28,SOEWin.x+20,y+i*28+13, forecolor);
//				GuiExchangeColor();
//				GuiFont12Align(SOEWin.x+1,y + i*28+2,19,\
//					FONT_MID,SOE[i]->itemNum);//���
//				GuiExchangeColor();	
//				GUI_RPointLine(SOEWin.x+20,y+i*28+1,y+i*28+13,2,forecolor);//��ֱ��
//				GuiFont12Align(SOEWin.x + 21,y + i*28+2,133,FONT_RIGHT,SOE[i]->time);
//				GuiHPointLine(SOEWin.x,y+i*28+13,155,2,forecolor);
//				GuiFont12Align(SOEWin.x+2,y + i*28+15,72,\
//					FONT_MID,SOEItems[SOE[i]->SOEItem].name);
//				GUI_RPointLine(SOEWin.x+10+72,y+i*28+15,y+i*28+27,2,forecolor);
//				if(SOE[i]->itemValue){
//					GuiFillRect(SOEWin.x+10+73,y+i*28+14,158,y+i*28+28, forecolor);
//					GuiExchangeColor();
//					GuiFont12Align(SOEWin.x+10+73,y+i*28+15,70,FONT_RIGHT,\
//						SOEItems[SOE[i]->SOEItem].status[SOE[i]->itemValue]);
//					GuiExchangeColor();
//				}
//				else{
//					GuiFont12Align(SOEWin.x+10+73,y+i*28+15,70,FONT_RIGHT,\
//						SOEItems[SOE[i]->SOEItem].status[SOE[i]->itemValue]);
//				}
//					
//			}
//			Scroll.lump = currentNum + 1;
//			DrawVScroll(&Scroll);
//			GuiUpdateDisplayAll();
//			flag = 3;
//		}
//		if(flag == 3){
//			 dataUpdateFlag = GetDtatUpdateStatus();
//			if(dataUpdateFlag.bit.newSoe == 1){//SOE�и���
//				ClearDtatUpdateNewSoe();//������±�־
//				currentNum = 0;
//				flag = 1;
//			}
//		}
//	}
//	
//	switch(keyStatus){
//	case UpKey:
//	case LeftKey:
//		if(flag > 1){
//			currentNum --;
//			if(currentNum < 0){
//				currentNum = Scroll.max - 1;
//			}
//		}
//		flag = 1;
//		break;
//	case DownKey:
//	case RightKey:
//		if(flag > 1){
//			currentNum ++;
//			if(currentNum > Scroll.max - 1){
//				currentNum = 0;
//			}
//		}
//		flag = 1;
//		break;
//	case OkKey:break;
//	case CancelKey:
//		currentNum = 0;
//		offestAddr = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief �����¼����ݴ���
//  *@param  pData ����������
//  *@param  eachSize ÿ���¼����ݵĴ�С
//  *@param  pEvent �����¼���ʾָ��
//  *@retval None
//  */
//static void EventDataResult(uint8_t *pData,uint8_t eachSize,struct EventDisplay *pEvent)
//{
//	uint32_t tempaddr,tempaddr2;
//	uint8_t i,j;
//	union{
//		float f32;
//		uint8_t u8[4];
//	}charToFloat;
//	tempaddr = (pData[2]<<8)+pData[3];
//	if(pData[4]>=0x20){//״̬λ����϶�
//		if((pData[4]&0x10) == 1){
//			tempaddr2 = 0x00060000|tempaddr;//ң��ȡ��
//		}
//		else{
//			switch(pData[4]&0x03){
//			case 0x01://��բ
//				switch(pData[4]&0x08){
//				case 0x00://ѡ��
//					tempaddr2 = 0x00000000|tempaddr;
//					break;
//				case 0x08://ִ��
//					tempaddr2 = 0x00010000|tempaddr;
//					break;
//				default: break;
//				}break;
//			case 0x02://��բ
//				switch(pData[4]&0x08){
//				case 0x00://ѡ��
//					tempaddr2 = 0x00020000|tempaddr;
//					break;
//				case 0x08://ִ��
//					tempaddr2 = 0x00030000|tempaddr;
//					break;
//				default: break;
//				}break;
//			case 0x03://�ϻ�
//				switch(pData[4]&0x08){
//				case 0x00://ѡ��
//					tempaddr2 = 0x00040000|tempaddr;
//					break;
//				case 0x08://ִ��
//					tempaddr2 = 0x00050000|tempaddr;
//					break;
//				default: break;
//				}break;
//			}
//		}
//		for(i = 0;i<SOE_NUM;i++){
//			if(tempaddr2 == SOEItems[i].addr){
//				pEvent->SOEItem = i;
//				pEvent->itemValue = (pData[4]&0x04)?1:0;
//				break;
//			}
//		}
//		if(i == SOE_NUM){
//			pEvent->SOEItem = 255;//255��ʾ��Ч
//		}
//	}
//	else{
//		tempaddr2 = (uint32_t)(tempaddr&0x0000FFFF);
//		for(i = 0;i<SOE_NUM;i++){
//			if(tempaddr2 == SOEItems[i].addr){
//				pEvent->SOEItem = i;
//				pEvent->itemValue = pData[4] - 1;//�� 1��2 ����״̬ת���� 0 1 �����������
//				if(pEvent->itemValue > 2){
//					pEvent->itemValue = 0;
//				}
//				break;
//			}
//		}
//		if(i == SOE_NUM){
//			pEvent->SOEItem = 255;//255��ʾ��Ч
//		}
//	}
//	if(pEvent->SOEItem < 255){
//		pEvent->yaoceNum = pData[12];
//		if(pEvent->yaoceNum > 12){
//			pEvent->yaoceNum = 12;
//		}
//		for(i=0;i<pEvent->yaoceNum;i++){
//			tempaddr = (pData[14 + i*6]<<8) + pData[15 + i*6];
//			for(j=0;j<YAOCE2_NUM;j++){
//				if(tempaddr == yaoCe2Items[j].addr){
//					pEvent->yaoceItem[i] = j;
//					charToFloat.u8[3] = pData[i*6+16];
//					charToFloat.u8[2] = pData[i*6+17];
//					charToFloat.u8[1] = pData[i*6+18];
//					charToFloat.u8[0] = pData[i*6+19];
//					if(((uint8_t)isnan(charToFloat.f32)) != 0){
//						sprintf((char *)&(pEvent->yaoceValue[i*16]),"%s","NaN");
//					}
//					else{
//						sprintf((char *)&(pEvent->yaoceValue[i*16]),"%.3f",charToFloat.f32);
//					}
//					break;
//				}
//			}
//			if(j == YAOCE2_NUM){
//				pEvent->yaoceItem[i] = 255;
//				break;
//			}
//		}
//	}
//	sprintf((char *)pEvent->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
//		(pData[11]%100),(pData[10]&0x3F),pData[9]&0x1F,\
//		(pData[8]&0x3F),(pData[7]&0x3F),\
//		((pData[6]<<8) + pData[5])/1000,((pData[6]<<8) + pData[5])%1000);
//}

///**
//  *@brief �����¼�������
//  *@param  None
//  *@retval None
//  */
//static void FaultEventFun(void)
//{
//	static uint8_t flag = 0;
//	static struct EventDisplay *pEvent;
//	static SCROLL Scroll ={156,18,141,2,1};//������
//	static SCROLL yaoCeScroll ={153,47,112,2,1};//������
//	static uint8_t getDataFlag = 0;
//	static uint16_t offestAddr = 0;//SOEƫ�Ƶ�ַ
//	static int16_t currentNum;
//	static int8_t yaoCeNum;
//	uint32_t pData;//��Ž�������ָ��
//	union yaoXinTimeflag dataUpdateFlag;
//	uint8_t tempRetrun;
//	uint8_t i;
//	uint16_t y;
//	Scroll.max = GetEventAllNum();
//	if(Scroll.max == 0){
//		GuiFont12Align(FaultEventWin.x+2,FaultEventWin.y + 50,\
//			FaultEventWin.x+FaultEventWin.wide-4,FONT_MID,"��ǰû�й����¼�");
//		GuiUpdateDisplayAll();
//	}
//	else{
//		if(flag == 0){//��ʼ���������ڴ�
//			pEvent = (struct EventDisplay *)userGUIBuff;
//			flag = 1;
//			getDataFlag = 0;
//		}
//		if(flag == 1){//��������
//			offestAddr = currentNum*(ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16/2);
//			tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//				ModbuscmdFIFO[FIFO_EVENT_NUM].addr.addr16+offestAddr,\
//				ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16,&pData);
//			if(tempRetrun == ISTURE){
//				EventDataResult((uint8_t *)pData,ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16,pEvent);
//				sprintf((char *)pEvent->itemNum,"%d",\
//					offestAddr/(ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16/2) + 1);
//				if(pEvent->yaoceNum <= 8){
//					yaoCeScroll.max = 1;
//				}
//				else{
//					yaoCeScroll.max = pEvent->yaoceNum - 8 + 1;
//				}
//				yaoCeNum = 0;
//				flag = 2;
//				if(pEvent->SOEItem == 255){
//					flag = 4;
//				}
//			}
//			else if(tempRetrun == ISFAILTURE){
//				getDataFlag = 0;
//			}
//			else{//�ȴ�����
//				GUIWiatMessage(30,65);
//			}
//		}
//		if(flag == 2){//��ͼ
//			y = FaultEventWin.y + 18;
//			GuiFillRect(FaultEventWin.x+1,y,155,158, backcolor);
//			GuiHLine(FaultEventWin.x,y,155,forecolor);//ˮƽ��	
//			GuiFillRect(FaultEventWin.x+1,y,FaultEventWin.x+20,y+13, forecolor);
//			GuiExchangeColor();
//			GuiFont12Align(FaultEventWin.x+1,y+2,20,FONT_MID,pEvent->itemNum);//���
//			GuiExchangeColor();	
//			GUI_RPointLine(FaultEventWin.x+20,y+1,y+13,2,forecolor);//��ֱ��
//			GuiFont12Align(FaultEventWin.x + 21,y+2,133,FONT_RIGHT,pEvent->time);
//			GuiHPointLine(FaultEventWin.x,y+13,155,2,forecolor);
//			GuiFont12Align(FaultEventWin.x+2,y+15,72,FONT_LEFT,SOEItems[pEvent->SOEItem].name);
//			GUI_RPointLine(FaultEventWin.x+10+72,y+15,y+27,2,forecolor);
//			GuiFont12Align(FaultEventWin.x+10+73,y+15,70,FONT_RIGHT,\
//				SOEItems[pEvent->SOEItem].status[pEvent->itemValue]);	
//			Scroll.lump = currentNum + 1;
//			DrawVScroll(&Scroll);
//			GuiHLine(FaultEventWin.x,y+28,155,forecolor);//ˮƽ��	
//			flag = 3;
//		}
//		if(flag == 3){
//			y = FaultEventWin.y + 18 + 29;
//			yaoCeScroll.lump = yaoCeNum + 1;
//			DrawVScroll(&yaoCeScroll);
//			GuiFillRect(FaultEventWin.x+1,y,151,158, backcolor);
//			for(i=0;i<8;i++){
//				GuiFont12Align(FaultEventWin.x+2,y+1+i*14,40,FONT_LEFT,\
//					yaoCe2Items[pEvent->yaoceItem[yaoCeNum + i]].name);			
//				GUI_RPointLine(FaultEventWin.x+43,y+i*14,y+13+i*14,2,forecolor);
//				GuiFont12Align(FaultEventWin.x+44,y+1+i*14,70,FONT_MID,&pEvent->yaoceValue[(i+yaoCeNum)*16]);
//				GUI_RPointLine(FaultEventWin.x+115,y+i*14,y+13+i*14,2,forecolor);
//				GuiFont12Align(FaultEventWin.x+116,y+1+i*14,35,FONT_RIGHT,\
//					yaoCe2Items[pEvent->yaoceItem[yaoCeNum+i]].unit[0]);		
//				GuiHPointLine(FaultEventWin.x,y+13+i*14,151,2,forecolor);
//			}
//			GuiUpdateDisplayAll();
//			flag = 4;
//		}
//		if(flag == 4){
//			 dataUpdateFlag = GetDtatUpdateStatus();
//			if(dataUpdateFlag.bit.newFaultEvent == 1){//SOE�и���
//				ClearnewFaultEvent();//������±�־
//				currentNum = 0;
//				flag = 1;
//			}
//		}
//	}
//	
//	switch(keyStatus){
//	case UpKey:
//		if(flag > 3){
//			yaoCeNum --;
//			if(yaoCeNum < 0){
//				yaoCeNum = yaoCeScroll.max - 1;
//			}
//		}
//		flag = 3;
//		break;
//	case LeftKey:
//		if(flag > 1){
//			currentNum --;
//			if(currentNum < 0){
//				currentNum = Scroll.max - 1;
//			}
//		}
//		flag = 1;
//		break;
//	case DownKey:
//		if(flag > 3){
//			yaoCeNum ++;
//			if(yaoCeNum > yaoCeScroll.max - 1){
//				yaoCeNum = 0;
//			}
//		}
//		flag = 3;
//		break;
//	case RightKey:
//		if(flag > 1){
//			currentNum ++;
//			if(currentNum > Scroll.max - 1){
//				currentNum = 0;
//			}
//		}
//		flag = 1;
//		break;
//	case OkKey:break;
//	case CancelKey:
//		currentNum = 0;
//		offestAddr = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

/**
  *@brief �������ô�����
  *@param  None
  *@retval None
  */
static void BasicSetFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(BasicConfigureItems,BasicConfigure_NUM,\
		&ModbuscmdDingZhi[BasicConfigureNum],&flag);
}

/**
  *@brief ��Ư���ô�����
  *@param  None
  *@retval None
  */
static void ZeroDriftFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(ZeroDriftItems,ZeroDrift_NUM,\
		&ModbuscmdDingZhi[ZeroDriftNum],&flag);
}

/**
  *@brief �������ô�����
  *@param  None
  *@retval None
  */
static void DeadZoneFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(DeadZoneItems,DeadZone_NUM,\
		&ModbuscmdDingZhi[DeadZoneNum],&flag);
}

/* END */
