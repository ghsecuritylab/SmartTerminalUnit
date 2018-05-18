/**
  * @file   Gui101Cmd.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/11
  * @brief  Gui101CmdMaster
  */

#include "gui_101_cmd.h"
#include "gui_hmi_101.h"
#include "string.h"
#include "stdio.h"
#include "lkdGuiMenu.h"
#include "lkdGuiWindow.h"
#include <rtthread.h>


/* cmd101�����¼� */
struct rt_event Cmd101SendEvent;


Gui101CmdControl cmd101;

/* ����ǰ��ɫ/����ɫ */
lkdColour forecolor = 1,backcolor;
/* ���尴��״̬ */
enum KeyStatus guikeyIs;

/**
  *@brief  ����101ѹջ
  *@param  None
  *@retval None
  */
void EndCmd101Down(void)
{
	union{
		uint16_t len16;
		uint8_t len8[2];
	}frameLen;	
	frameLen.len16 = cmd101.pIn;
	cmd101.state = 2;
	cmd101.packBuff[0] = frameLen.len8[0];
	cmd101.packBuff[1] = frameLen.len8[1];
	cmd101.packBuff[2] = cmd101.cmdNum;
	DLT634_HMI_SLAVE_C_REPLY(0,cmd101.packBuff);
}

/**
  *@brief cmd101ѹջ���ƺ���
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
static void Cmd101DownControl(uint16_t len)
{
	if(cmd101.state == 0){//ջ��,����ѹջ
		cmd101.cmdNum = 0;
		cmd101.pIn = 3;
		cmd101.state = 1;
	}
	if(cmd101.state == 2){//���ڷ���,�����ȴ�
		rt_event_recv(&Cmd101SendEvent, (1 << 0), RT_EVENT_FLAG_OR |\
			RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
		cmd101.cmdNum = 0;
		cmd101.pIn = 3;
		cmd101.state = 1;
	}
	if(cmd101.state == 1){//ѹջ��
		if(cmd101.pIn + len >= PACKBUFFMAX - 20){
			EndCmd101Down();
			rt_event_recv(&Cmd101SendEvent, (1 << 0), RT_EVENT_FLAG_OR |\
				RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
			cmd101.cmdNum = 0;
			cmd101.pIn = 3;
			cmd101.state = 1;
		}
	}
}

/**
  *@brief cmd101������ɺ���
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
void Cmd101SendFinish(void)
{
	//��������¼�
	rt_event_send(&Cmd101SendEvent, (1 << 0));
}


void BeginCmd101Down(void)
{
	if(cmd101.state != 0){
	}
	else{
		cmd101.pIn = 3;
	}
	cmd101.cmdNum = 0;
	cmd101.pIn = 3;
}

/**
  *@brief ��б��
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
void GuiBiasLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_BLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}
/**
  *@brief ��ˮƽ����
  *@param  x0,y0 ��ʼ����
  *@param  x1 ��������
  *@param  interval ÿ��N�㻭һ����
  *@param  color ��ɫ
  *@retval None
  */
void GuiHPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, uint8_t interval, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_HLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = interval;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ֱ����
  *@param  x0,y0 ��ʼ����
  *@param  y1 ��������
  *@param  interval ÿ��N�㻭һ����
  *@param  color ��ɫ
  *@retval None
  */
void  GuiRPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord y1,uint8_t interval, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_RLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = interval;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}
/**
  *@brief ��ˮƽ��
  *@param  x0,y0 ��ʼ����
  *@param  x1��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_HLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ֱ��
  *@param  x0,y0 ��ʼ����
  *@param  y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void  GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_RLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief ������
  *@param  x0,y0 ��ʼ����
  *@param  x1,y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD101_CMD] = 101;
	cmd101.packBuff[cmd101.pIn + CMD101_TYPE] = C101TYPE_RECT;
	cmd101.packBuff[cmd101.pIn + CMD101_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD101_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD101_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD101_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD101_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD101_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief ��������
  *@param  x0,y0 ��ʼ����
  *@param  x1,y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD101_CMD] = 101;
	cmd101.packBuff[cmd101.pIn + CMD101_TYPE] = C101TYPE_FILLRECT;
	cmd101.packBuff[cmd101.pIn + CMD101_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD101_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD101_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD101_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD101_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD101_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief 12x12���庯��
  *@param  x,y ��ʼ����
  *@param  str �ַ���
  *@retval None
  */
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str)
{
	uint8_t tstrlen;
	
	tstrlen = strlen((char *)str) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = C108FLAG_FEEDLINE;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD108_CONTENT],str,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD108_LEN] = tstrlen + 7;
	cmd101.pIn += tstrlen + 7;
	cmd101.cmdNum += 1;
}

/**
  *@brief 12x12���������ʾ����
  *@param  x,y ��ʼ����
  *@param  wide ���
  *@param  flag �����־
  *@param  str �ַ���
  *@retval None
  */
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide, FontFlag flag,uint8_t *str)
{
	uint8_t tstrlen;
	tstrlen = strlen((char *)str) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = wide;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 14;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = flag;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD108_CONTENT],str,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD108_LEN] = tstrlen + CMD108_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD108_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ��ֱ������
  *@param  pScroll Scroll handle
  *@retval None
  */
void GuiVScroll(SCROLL *pScroll)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD105_CMD] = 105;
	cmd101.packBuff[cmd101.pIn + CMD105_TYPE] = C105TYPE_RSCROLL;
	cmd101.packBuff[cmd101.pIn + CMD105_BX] = pScroll->x;
	cmd101.packBuff[cmd101.pIn + CMD105_BY] = pScroll->y;
	cmd101.packBuff[cmd101.pIn + CMD105_SLEN] = pScroll->hight;
	cmd101.packBuff[cmd101.pIn + CMD105_SMAX] = pScroll->max;
	cmd101.packBuff[cmd101.pIn + CMD105_SLUMP] = pScroll->lump;
	cmd101.packBuff[cmd101.pIn + CMD105_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief ����
  *@param  x��y ��ʼ����
  *@param  keyName ��������
  *@param  flag 0��ѡ�� 1ѡ��
  *@retval None
  */
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag)
{
	uint8_t tstrlen;
	tstrlen = strlen((char *)keyName) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD106_CMD] = 106;
	cmd101.packBuff[cmd101.pIn + CMD106_TYPE] = C106TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD106_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD106_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD106_FlAG] = flag;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD106_CONTENT],keyName,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD106_LEN] = tstrlen + CMD106_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD106_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void CloseLcdDisplay(void)
{
	Cmd101DownControl(3);
	cmd101.packBuff[cmd101.pIn + CMD104_CMD] = 104;
	cmd101.packBuff[cmd101.pIn + CMD104_TYPE] = C104TYPE_OFFSCREEN;
	cmd101.packBuff[cmd101.pIn + CMD104_LEN] = 3;
	cmd101.pIn += 3;
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void OpenLcdDisplay(void)
{
	Cmd101DownControl(3);
	cmd101.packBuff[cmd101.pIn + CMD104_CMD] = 104;
	cmd101.packBuff[cmd101.pIn + CMD104_TYPE] = C104TYPE_ONSCREEN;
	cmd101.packBuff[cmd101.pIn + CMD104_LEN] = 3;
	cmd101.pIn += 3;
	cmd101.cmdNum += 1;
}

/**
  *@brief ��ָ����Χ��λͼ
  *@param  bitMap λͼ����
  *@param  x0,y0 ��ʼ����
  *@param  endx,endy �յ�����
  *@param  flag 0 ������ʾ 1����
  *@retval None
  */
void GuiAreaBitMap(lkdBitmap *bitMap,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag)
{
	uint16_t tstrlen;
	
	if(bitMap->number < 1){
		if((bitMap->wide % 8) == 0){
			tstrlen = bitMap->high * (bitMap->wide / 8);
		}
		else{
			tstrlen = bitMap->high * (bitMap->wide / 8 + 1);
		}
		Cmd101DownControl(tstrlen + CMD107_CONTENT);
		memcpy(&cmd101.packBuff[cmd101.pIn + CMD107_CONTENT],bitMap->bitmap,tstrlen);
	}
	else{
		tstrlen = 1;
		Cmd101DownControl(tstrlen + CMD107_CONTENT);
		cmd101.packBuff[cmd101.pIn + CMD107_CONTENT] = '\0';
	}
	cmd101.packBuff[cmd101.pIn + CMD107_CMD] = 107;
	cmd101.packBuff[cmd101.pIn + CMD107_TYPE] = bitMap->number;
	cmd101.packBuff[cmd101.pIn + CMD107_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD107_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD107_EX] = endx;
	cmd101.packBuff[cmd101.pIn + CMD107_EY] = endy;
	cmd101.packBuff[cmd101.pIn + CMD107_MXLEN] = bitMap->wide;
	cmd101.packBuff[cmd101.pIn + CMD107_MYLEN] = bitMap->high;
	cmd101.packBuff[cmd101.pIn + CMD107_FlAG] = flag;
	
	cmd101.packBuff[cmd101.pIn + CMD107_LEN] = tstrlen + CMD107_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD107_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ������ʾ
  *@param  None
  *@retval None
  */
void GuiUpdateDisplayAll(void)
{
	Cmd101DownControl(2);
	cmd101.packBuff[cmd101.pIn + CMD103_CMD] = 103;
	cmd101.packBuff[cmd101.pIn + CMD103_LEN] = 2;
	cmd101.pIn += 2;
	cmd101.cmdNum += 1;
	EndCmd101Down();
}

/**
  *@brief ��ȡǰ��ɫ
  *@param  None
  *@retval ����ǰ��ɫ
  */
lkdColour GuiGetForecolor(void)
{
	return forecolor;
}

/**
  *@brief ����ǰ��ɫ
  *@param  colour ǰ��ɫ
  *@retval None
  */
void GuiSetForecolor(lkdColour colour)
{
	forecolor = colour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_FORE;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = forecolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ��ȡ����ɫ
  *@param  None
  *@retval ���ر���ɫ
  */
lkdColour GuiGetbackcolor(void)
{
	return backcolor;
}
/**
  *@brief ���ñ���ɫ
  *@param  colour ����ɫ
  *@retval None
  */
void GuiSetbackcolor(lkdColour colour)
{
	backcolor = colour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_BACK;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = backcolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief ����ǰ��ɫ�ͱ���ɫ
  *@param  None
  *@retval None
  */
void GuiExchangeColor(void)
{
	lkdColour tColour;
	tColour = backcolor;
	backcolor = forecolor;
	forecolor = tColour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_EXCHANGE;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = forecolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

enum KeyStatus GetKeyStatus(void)
{
	return guikeyIs;
}
void SetKeyStatus(enum KeyStatus keyIs)
{
	guikeyIs = keyIs;
}
void SetKeyIsNoKey(void)
{
	guikeyIs = NoKey;
}


uint8_t *testStr[] = {
	"���ط�λ0","���غ�λ1","����һ��2","��������3","�غ�բ����4",
	"���ط�λ5","���غ�λ6","����һ��7","��������8","�غ�բ����9",
	"���ط�λ10","���غ�λ11","����һ��12","��������13","�غ�բ����14",
	"��15","SOE16","����Ͷ17","������18","����19",
	"��20","SOE21","����Ͷ22","������23","����24",
};


void TestHmiMain(uint8_t *flag)
{
	uint8_t i;
	static  uint32_t TestTick;
	uint8_t floatStr[16];
	if(*flag == 1){
		BeginCmd101Down();
		GuiFillRect(0, 0, 159, 159, 0);
		GuiFillRect(10, 20, 149, 36, 1);
		GuiFont12Align(12, 22, 130, FONT_MID,"sojo�¼ܹ���������");
		for(i = 0; i < 6; i++){
			GuiHLine(0, 38 + i * 14, 159, 1);
		}
		GuiRLine(0, 38, 102, 1);
		GuiRLine(52, 38, 102, 1);
		GuiRLine(104, 38, 102, 1);
		GuiRLine(159, 38, 102, 1);
		GuiRect(20, 110, 140,130, 1);
		GuiUpdateDisplayAll();
	}
	if(*flag == 2){
		BeginCmd101Down();
		GuiFillRect(0, 0, 159, 159, 0);
		GuiRect(0, 0, 159, 159, 1);
		GuiHLine(0, 16, 159, 1);
		GuiHLine(0, 18, 159, 1);
		GuiFont12Align(0, 2, 159, FONT_MID,"��ˢ�´���");
		for(i = 1; i < 9; i++){
			GuiHPointLine(0, 18 + i * 15, 159, 2, 1);
		}
		for(i = 0; i < 9; i++){
			GuiFont12Align(2, 20 + i * 15, 78, FONT_LEFT,testStr[i]);
		}
		for(i = 0; i < 9; i++){
			GuiFont12Align(82, 20 + i * 15, 77, FONT_LEFT,testStr[i + 9]);
		}
		GuiUpdateDisplayAll();
	}
	if(*flag == 3){
		if(GetTimer1IntervalTick(TestTick) > 1000){
			TestTick = GetTimer1Tick();
			BeginCmd101Down();
			GuiFillRect(0, 0, 159, 159, 0);
			GuiRect(0, 0, 159, 159, 1);
			GuiHLine(0, 16, 159, 1);
			GuiHLine(0, 18, 159, 1);
			GuiFont12Align(0, 2, 159, FONT_MID,"HMI����");
			for(i = 1; i < 9; i++){
				GuiHPointLine(0, 18 + i * 15, 159, 2, 1);
			}
			sprintf((char *)floatStr,"%d",(uint8_t)TestTick);
			for(i = 1; i < 9; i++){
				GuiFont12Align(2, 20 + i * 15, 78, FONT_LEFT,floatStr);
			}
			for(i = 1; i < 9; i++){
				GuiFont12Align(82, 20 + i * 15, 77, FONT_LEFT,floatStr);
			}
			GuiUpdateDisplayAll();
		}
	}
	if(*flag == 4){
		BeginCmd101Down();
		//GUIMenuDraw(&mianMenu);
		GuiUpdateDisplayAll();
	}
}

//MENU mianMenu = {
//	0,0,NULL,0,5,{1,1,1,0,0},//x,y,fatherMenu,currentItem,mun,ItemFlag
//	{"��Ϣ��ѯ","��ֵ����","��������","�����·�","�汾��Ϣ",NULL,NULL,NULL,NULL,NULL},
//	NULL/* MenuFunction */
//};
//MENU MenuM1 = {
//	0,0,&mianMenu,0,9,{1,0,0,0,0,0,0,0,1},//x,y,fatherMenu,currentItem,mun
//	{"��������","�߼�����","Խ�ޱ���","�� �� ��","�� �� ��","ԽѹԽƵ","�������","�Զ�����","��������",NULL},
//	NULL/* itemFunction */
//};

//void MenuTest(void)
//{
//	static uint8_t menustep,ItemIs,itemflag;
//	static uint32_t menuTick;
//	BeginCmd101Down();
//	if(menustep == 0){
//		BeginCmd101Down();
//		userGUIMenuAdd(&mianMenu);
//		userGUIMenuAdd(&MenuM1);
//		GuiUpdateDisplayAll();
//		menuTick = GetTimer1Tick();
//		menustep = 1;
//	}
//	if(menustep == 1 && GetTimer1IntervalTick(menuTick) > 1000){
//		menuTick = GetTimer1Tick();
//		if(ItemIs < 9 && itemflag == 0){
//			userGUIMenuIremModfiy(1);
//			ItemIs ++;
//			if(ItemIs >= 9){
//				itemflag = 1;
//			}
//		}
//		else{
//			if(ItemIs < 1){
//				itemflag = 0;
//			}
//			ItemIs --;
//			userGUIMenuIremModfiy(0);	
//		}
//		GuiUpdateDisplayAll();
//	}
//}
//WINDOW VersionWin = {0,0,160,160,NULL,"�汾��Ϣ����",NULL};
//void WindowTest(void)
//{
//	static uint8_t winstep;
//	static uint32_t winTick;
//	if(winstep == 0){
//		BeginCmd101Down();
//		userGUIWindowAdd(&VersionWin);
//		GuiUpdateDisplayAll();
//		winTick = GetTimer1Tick();
//		winstep = 1;
//	}
//	if(winstep == 1 && GetTimer1IntervalTick(winTick) > 1000){
//		winstep = 0;
//		BeginCmd101Down();
//		userGUITopWindowHide();
//		GuiUpdateDisplayAll();
//	}
//}
void TestMain(void)
{
	static uint8_t flag;
	static uint32_t runTick,HmiTestTick;
	if(flag == 0){
		runTick = GetTimer1Tick();
		flag = 1;
	}
	else if(flag == 1 && GetTimer1IntervalTick(runTick) > 5000){
		runTick = GetTimer1Tick();
		flag = 2;
	}
	else if(flag == 2 && GetTimer1IntervalTick(runTick) > 5000){
		runTick = GetTimer1Tick();
		flag = 3;
	}
	else if(flag == 3 && GetTimer1IntervalTick(runTick) > 10000){
		runTick = GetTimer1Tick();
		flag = 4;
	}
	else if(flag == 4 && GetTimer1IntervalTick(runTick) > 10000){
		runTick = GetTimer1Tick();
		flag = 1;
	}
	if(GetTimer1IntervalTick(HmiTestTick) > 50){
		HmiTestTick = GetTimer1Tick();
		//TestHmiMain(&flag);
		//MenuTest();
		//WindowTest();
	}
	
}

