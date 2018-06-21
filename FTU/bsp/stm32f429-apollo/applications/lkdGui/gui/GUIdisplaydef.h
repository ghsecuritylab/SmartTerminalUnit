#ifndef  __GUIDISPLAYDEF_H
#define  __GUIDISPLAYDEF_H

#include "lkdGuiMenu.h"
#include "gui_101_cmd.h"
#include "lkdGuiWindow.h"

MENU mianMenu = {
	0,0,NULL,0,6,{1,1,1,0,0},//x,y,fatherMenu,currentItem,mun,ItemFlag
	{"��Ϣ��ѯ","��ֵ����","��������","ʱ���޸�","�����·�","�汾��Ϣ",NULL,NULL,NULL,NULL},
	mianMenuFun/* MenuFunction */
};
MENU MenuM0 = {
	0,0,&mianMenu,0,7,{0,1,0,0,1,1},//x,y,fatherMenu,currentItem,mun
	{"ң�Ų�ѯ","ң���ѯ"," SOE��ѯ","������¼","�����¼�","��ֵ��ѯ","���ò�ѯ",NULL,NULL,NULL},
	MenuM0Fun/* itemFunction */	
};
MENU MenuM1 = {
	0,0,&mianMenu,0,9,{1,0,0,0,0,0,0,0,1,0},//x,y,fatherMenu,currentItem,mun
	{"��������","�߼�����","�Ե�ͨ��","Խ�ޱ���","�� �� ��","�� �� ��","ԽѹԽƵ","�������","��������",NULL},
	MenuM1Fun/* itemFunction */
};
MENU MenuM2 = {
	0,0,&mianMenu,0,5,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"��������","��Ư����","��������","��������","��������",NULL,NULL,NULL,NULL,NULL},
	MenuM2Fun/* itemFunction */
};
MENU MenuM0S1 = {
	0,0,&MenuM0,0,3,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"һ��ң��","����ң��","г������",NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM0S1Fun/* itemFunction */
};
MENU MenuM0S4 = {
	0,0,&MenuM0,0,9,{1,0,0,0,0,0,0,0,1,0},//x,y,fatherMenu,currentItem,mun
	{"��������","�߼�����","�Ե�ͨ��","Խ�ޱ���","�� �� ��","�� �� ��","ԽѹԽƵ","�������","������ѯ",NULL},
	MenuM0S4Fun/* itemFunction */
};
MENU MenuM0S5 = {
	0,0,&MenuM0,0,5,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"������ѯ","��Ư��ѯ","������ѯ","���ڲ�ѯ","���ڲ�ѯ",NULL,NULL,NULL,NULL,NULL},
	MenuM0S5Fun/* itemFunction */
};
MENU MenuM0S4S0 = {
	0,0,&MenuM0S4,0,3,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"������","�������","��������",NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM0S4S0Fun/* itemFunction */
};
MENU MenuM0S4S8 = {
	0,0,&MenuM0S4,0,2,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"��    ��","����Ͷ��",NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM0S4S8Fun/* itemFunction */
};
MENU MenuM1S0 = {
	0,0,&MenuM1,0,3,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"������","�������","��������",NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM1S0Fun/* itemFunction */
};

MENU MenuM1S8 = {
	0,0,&MenuM1,0,2,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"��    ��","����Ͷ��",NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM1S8Fun/* itemFunction */
};

WINDOW TimeModfiyWin  = {12,44,136,66,NULL,"ʱ���޸Ĵ���",TimeModfiyFun};
WINDOW HomeWindow = {0,0,160,160,NULL,NULL,HomeWindowFun};
WINDOW MenuWindow = {0,0,160,160,NULL,"�˵�������",MenuWindowFun};
WINDOW CmdSendWin = {0,0,160,160,NULL,"�����·�����",CmdSendWinFun};
WINDOW VersionWin = {0,0,160,160,NULL,"�汾��Ϣ����",VersionWinFun};

///* ��Ϣ��ѯ�˵��Ӵ��� */
WINDOW YaoxinWin     = {0,0,160,160,NULL,"ң��״̬����",YaoxinFun};
	WINDOW YaoCe1Win   = {0,0,160,160,NULL,"һ��ң�ⴰ��",YaoCe1Fun};
	WINDOW YaoCe2Win   = {0,0,160,160,NULL,"����ң�ⴰ��",YaoCe2Fun};
	WINDOW HarmonicWin = {0,0,160,160,NULL,"г����������",HarmonicFun};
WINDOW SOEWin   	 = {0,0,160,160,NULL,"SOE��¼����",SOEFun};
WINDOW COWin   	 = {0,0,160,160,NULL,"������¼����",COFun};
WINDOW FaultEventWin = {0,0,160,160,NULL,"�����¼�����",FaultEventFun};

/* ��Ϣ��ѯ->��ֵ��ѯ�˵��Ӵ��� */

/* ��ֵ���ò˵��Ӵ��� */
	WINDOW InterphaseOverWin   = {0,0,160,160,NULL,"����������",InterphaseOverFun};
	WINDOW ZeroSequenceOverWin = {0,0,160,160,NULL,"�����������",ZeroSequenceOverFun};
	WINDOW CutoutSwitchWin     = {0,0,160,160,NULL,"������������",  CutoutSwitchFun};
WINDOW LogicalFunWin   = {0,0,160,160,NULL,"�߼����ܴ���",LogicalFunFun};
WINDOW PToPComFunWin   = {0,0,160,160,NULL,"�Ե�ͨ�Ŵ���",PToPComFun};
WINDOW OverLineWarnWin = {0,0,160,160,NULL,"Խ�ޱ�������",OverLineWarnFun};
WINDOW OverLoadMuchWin = {0,0,160,160,NULL,"�ع��ش���",OverLoadMuchFun};
WINDOW OverLoadWin     = {0,0,160,160,NULL,"�����ɴ���",OverLoadFun};
WINDOW OverVoltageWin  = {0,0,160,160,NULL,"����ѹ����",OverVoltageFun};
WINDOW BatterySetWin   = {0,0,160,160,NULL,"��ع�����",BatterySetFun};
	WINDOW RingUniteWin        = {0,0,160,160,NULL,"�ϻ����ô���",RingUniteFun};
	WINDOW BreakDownWin        = {0,0,160,160,NULL,"����Ͷ�˴���",BreakDownFun};
/* �������� */
WINDOW BasicSetWin   = {0,0,160,160,NULL,"�������ô���",BasicSetFun};
WINDOW ZeroDriftWin  = {0,0,160,160,NULL,"��Ư���ô���",ZeroDriftFun};
WINDOW DeadZoneWin 	 = {0,0,160,160,NULL,"�������ô���",DeadZoneFun};
WINDOW SerialWin  = {0,0,160,160,NULL,"�������ô���",SerialFun};
WINDOW InternetWin 	 = {0,0,160,160,NULL,"�������ô���",InternetFun};

WINDOW DZModfiyWin = {14,19,130,134,NULL,"��ֵ�޸Ĵ���",DZModfiyFun};
/* ���ô��� */
WINDOW MessageWin = {23,40,110,75,NULL,"��Ϣ����",MessageFun};
WINDOW PassWordWin = {23,40,110,75,NULL,"���봰��",PassWordFun};

#endif
