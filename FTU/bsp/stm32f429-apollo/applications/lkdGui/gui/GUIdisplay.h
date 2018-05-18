/**
  * @file   GUIdisplay.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI��ʾ
  */

#ifndef __GUIDISPLAY_H
#define __GUIDISPLAY_H

#include "lkdGuiMenu.h"
#include "gui_101_cmd.h"
#include "lkdGuiWindow.h"

#define DZDISPLAYCOL 3		/* ��ֵ��ʾ���� */
#define YaoCeDISPLAYCOL 3	/* ң����ʾ���� */
#define DISPLAYLISTROW 10	/* ����ʾ���� */
#define SOEDISPLAYROW 5		/* SOE��ʾ���� */
#define EVENTDISPLAYROW 1	/* �����¼���ʾ���� */
#define CONSTPARDISPLAYROW 5/* ���в�����ʾ���� */

#define YAOCE_UPDATATIME 1000 /* ң�������ʾʱ�� 1s */

/* Lcd������ʾ���� */
#define LCDDISPLAYON	1
#define LCDDISPLAYOFF	0
#define LCDDISPLAYOFFTIME	(1000*60)/* ms */

/* ����״̬ */
enum ReturnState{
	ISFAILTURE,	/* ʧ�� */
	ISTURE,		/* �ɹ� */
	ISWAITING,	/* �ȴ� */
	ISOVERTIME	/* ��ʱ */
};

/* ���ö�ֵ���� */
enum DingZhiSetOrSee{
	NULL_DINGZHI,	/* NULL */
	SET_DINGZHI,	/* �޸Ķ�ֵ */
	SEE_DINGZHI,	/* �����ֵ */
};

/* ��Ϣ������Ϣ */
struct Message{
	uint8_t *str;	/* ��ʾ�� */
	uint8_t *flag;	/* ���ر�־ */
};

/* ���봰����Ϣ */
struct PassWordPipe{
	uint8_t passWordType; 	/* �������� */
	uint8_t *returnFlag;	/* ���ر�־ */
};

/* SOE��ʾ */
struct SOEDisplay{
	uint8_t itemNum[4];	/* ��ǰ���� */
	uint8_t time[24];	/* ʱ�� */
	uint16_t SOEItem;	/* SOE item */
	uint8_t itemValue;	/* SOE ״̬ */
};

/* �����¼���ʾ */
struct EventDisplay{
	uint8_t itemNum[4];		/* ��ǰ���� */
	uint8_t time[24];		/* ʱ�� */
	uint16_t SOEItem;		/* SOE item */
	uint8_t itemValue;		/* SOE ״̬ */
	uint8_t yaoceNum;		/* ң���� */
	uint8_t yaoceItem[12];	/* ң���Ӧ��item */
	uint8_t yaoceValue[12*16];/* ң��ֵת��Ϊ�ַ��� */
};

/* ��ֵ�޸� */
struct DZModfiy{
	const struct DingZhi1Item *Items;	/* ��ֵ�ṹ�� */
	const struct ModbusCmd_ *cmd;		/* modbus���� */
	uint8_t flag;	/* ��־ */
	uint8_t itemIs;	/* ��ǰ�� */
	uint8_t *str;	/* Ҫ�޸ĵ��ַ��� */
};

/* ��ҳ���洰�� */
static void HomeWindowFun(void);
/* �˵����� */
static void MenuWindowFun(void);
/*M ���˵� */
static void mianMenuFun(void);
/*M0 ��Ϣ��ѯ */
static void MenuM0Fun(void);
//	static void YaoxinFun(void);		//ң��
void YaoxinFun(void);
	static void MenuM0S1Fun(void);		//ң��
		static void YaoCe1Fun(void);	//һ��
		static void YaoCe2Fun(void);	//����
		static void HarmonicFun(void);	//г��
	static void SOEFun(void);			//SOE
	static void FaultEventFun(void);	//�����¼�
	static void MenuM0S4Fun(void);		//��ֵ��ѯ 
		static void MenuM0S4S0Fun(void);
	static void MenuM0S5Fun(void);		//���ò�ѯ
/* M1 ��ֵ����  */
static void MenuM1Fun(void);
	static void MenuM1S0Fun(void);		//��������
		static void InterphaseOverFun(void);
		static void ZeroSequenceOverFun(void);
		static void CutoutSwitchFun(void);
	static void LogicalFunFun(void);	//�߼�����
	static void OverLineWarnFun(void);	//Խ�߱���
	static void OverLoadMuchFun(void);	//�ع���
	static void OverLoadFun(void);		//������
	static void OverVoltageFun(void);	//����ѹ
	static void BatterySetFun(void);	//�������
	static void AutoResetFun(void);		//�Զ�����
	static void MenuM1S8Fun(void);		//����
		static void RingUniteFun(void); //�ϻ�
		static void BreakDownFun(void); //����Ͷ��
/* M2 ��������  */	
static void MenuM2Fun(void);	
	static void BasicSetFun(void);		//��������
	static void ZeroDriftFun(void);		//��Ư����
	static void DeadZoneFun(void);		//��������
/* �����·�  */
static void CmdSendWinFun(void);
/* �汾��Ϣ */	
static void VersionWinFun(void);
/* ��ֵ�޸Ĵ��ں��� */
void DZModfiyFun(void);
/* ��Ϣ���� */
void MessageFun(void);
/* ���봰�� */
void PassWordFun(void);

/* �������滭ͼ */
void GUIStartInterface(void);
/* GUI������ */
void GUIDisplayMian(void);
/* GUI��ʼ�� */
void GUIDisplayInit(void);

#endif /* __GUIDISPLAY_H */
