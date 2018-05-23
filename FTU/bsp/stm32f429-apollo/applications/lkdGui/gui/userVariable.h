#ifndef  __USERVARIABLE_H
#define  __USERVARIABLE_H

#include "stdint.h"
#include "point_table_config.h"

typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t s;
	uint16_t ms;
}SystemTimeDisplay;

/* ң����ʾ�ṹ�� */
typedef struct{
	uint8_t *pBuff;	/* ����ң�������� */
	uint8_t Num;		/* ���������� */
	struct tagTelesignalCfg *pRoot; /* ң��ʵ��ָ�� */
}YaoxinDisplayInfo;

/* ң����ʾ�ṹ�� */
typedef struct{
	uint8_t *pBuff;	/* ����ң�������� */
	uint8_t num;		/* ���������� */
	struct tagTelemetryCfg *pRoot; /* ң��ʵ��ָ�� */
}YaoceDisplayInfo;

/* ��ֵ��ʾ�ṹ�� */
typedef struct{
	uint8_t *pBuff;	/* ���涨ֵ������ */
	uint8_t num;		/* ���������� */
	void(* SaveModify)(uint16_t addr);/* ��ֵ�޸ı���ָ�뺯�� */
	struct tagValueParaCfg *pRoot; /* ��ֵʵ��ָ�� */
}DzhiDisplayInfo;

/* Soe��ʾ�ṹ�� */
typedef struct{
	uint8_t highlight;
	uint8_t *pName;
	uint8_t *pContent;
	SystemTimeDisplay time;
}SoeContent;

/* Soe��ʾ�ṹ�� */
typedef struct{
	uint16_t num;		/* soe���� */
	uint16_t in;
}SoeDisplayInfo;

/* �����¼���ʾ�ṹ�� */
typedef struct{
	uint8_t *pName;
	uint8_t *pContent;
	uint8_t yaoceNum;
	uint8_t *pYaoceName[16];
	uint8_t *pYaoceUnit[16];
	float pYaoceValue[16];
	SystemTimeDisplay time;
}FeventContent;

/* �����¼���ʾ�ṹ�� */
typedef struct{
	uint16_t num;		/* �����¼����� */
	uint16_t in;
}FeventDisplayInfo;

enum Dzhi0OffsetNumber{
	DZ0_CONFIG,/* ���� */
	DZ0_ZERODRIFT,/* ��Ư */
	DZ0_DEADEZONE,/* ���� */
	DZ0_ALLNUM,/* ��ֵ���� */
};
enum Dzhi1OffsetNumber{
	DZ1_INTERGHASE,		/* ������ */
	DZ1_ZERO_SEQUE,		/* ������� */
	DZ1_LIMITATION,		/* Խ�߸澯 */
	DZ1_HEAVY_LOAD,		/* �ع��� */
	DZ1_OVERLOAD,			/* ������ */
	DZ1_IBATTERY_SET,	/* ������� */
	DZ1_AUTO_RESET,		/* �Զ����� */
	DZ1_LIMIT_V_F,		/* ԽѹԽƵ */
	DZ1_LOOP_CLOSE,		/* �ϻ� */
	DZ1_FAULT_SWITCH,	/* ����Ͷ�� */
	DZ1_OTHER_PROTEC,	/* �������� */
	DZ1_LOGICAL_FUN,	/* �߼����� */
	DZ1_ALLNUM,				/* ��ֵ���� */
};

/* �����·��ṹ */
struct HmiCmdSend{
	uint8_t *name;
	uint8_t cmd;
};
/* �����·���Ϣ */
typedef struct{
	uint8_t itemsNum;
	struct HmiCmdSend *pHmiCmd;
	void (* cmdfun)(uint8_t cmdIs);
}HmiCmdSendInfo;

/* ң����Ϣ�ṹ */
extern YaoxinDisplayInfo yxInfo;
/* ң����Ϣ�ṹ */
extern YaoceDisplayInfo yceInfo[3];
/* ��ֵ��Ϣ�ṹ */
extern DzhiDisplayInfo dzhi0Info[DZ0_ALLNUM];
extern DzhiDisplayInfo dzhi1Info[DZ1_ALLNUM];
/* �����·��ṹ�� */
extern HmiCmdSendInfo hcmdInfo;
/* ��ʾ��Ϣӳ���ʼ�� */
void userVariableDisplayInit(void);

/* ��ȡSOE���� */
uint8_t GetSoeNoContent(uint16_t soeNo,SoeContent *pSoe);
/* ���SOE���޸��� */
uint8_t CheckSoeUpdata(void);
/* ��ȡSOE���� */
uint16_t GetSoeNum(void);
/* ��ȡ�����¼����� */
uint16_t GetFeventNum(void);
/* ��ȡ�����¼����� */
uint8_t GetFeventNoContent(uint16_t feventNo,FeventContent *pEvent);
/* �������¼����޸��� */
uint8_t CheckFeventUpdata(void);

/* ң�� */
struct YaoXinItem{
	uint16_t addr;
	uint8_t *name;
	uint8_t *status[2];
};
/* SOE */
struct SOEItem{
	uint32_t addr;
	uint8_t *name;
	uint8_t *status[2];
};
struct YaoCeItem{
	uint16_t addr;
	uint8_t	valueWide;
    uint8_t valueDecimal;
	uint8_t *name;
	uint8_t *unit[2];
};

struct DingZhi1Item{
	uint16_t addr;
	uint8_t *name;
	uint8_t	flag;//0 ������ 1����
	uint8_t *max;
	uint8_t *min;
	uint8_t	valueWide;
    uint8_t valueDecimal;
	uint8_t *unit[2];
	uint8_t *pContent[8];//����
};

struct ConstParameter{
	uint16_t addr;
	uint8_t *name;
};

struct CmdSend{
	/* ң������λ��modbuscmd.c�У�M_mapNumΪ��Ӧ�ṹ���item�� */
	uint8_t M_mapNum;
	uint8_t *name;
};

struct PassWordMessage{
	uint8_t passWord[8];
	uint8_t *name;
};

#if (USEING_TAB == USE_TAB_ONE)

/* ң�� */
//#define YAOXIN_NUM 24
#define YAOXIN_START_ADDR 0x0001

/* ң�� */
#define YAOCE1_NUM 20
#define YAOCE2_NUM 20
#define YAOCE_NUM  34//ң������
#define YAOCE_START_ADDR 0x4001
#define HARMONIC_NUM 16//г������
#define HARMONIC_START_ADDR (YAOCE_START_ADDR+YAOCE_NUM)

/* ��ֵ */

/* ���в�����ַ */
#define ConstParameter_NUM 11
#define ConstParameter_START_ADDR	(0x8001)
/* �������� */
#define BasicConfigure_NUM 21
#define BasicConfigure_START_ADDR	(0x8101)
/* ��Ư���� */
#define ZeroDrift_NUM 18
#define ZeroDrift_START_ADDR	(BasicConfigure_START_ADDR+BasicConfigure_NUM)
/* �������� */
#define DeadZone_NUM 18
#define DeadZone_START_ADDR	(ZeroDrift_START_ADDR+ZeroDrift_NUM)
/* ������ */
#define InterphaseOver_NUM 12
#define InterphaseOver_START_ADDR	0x8301
/* ������� */
#define ZeroSequenceOver_NUM 6
#define ZeroSequenceOver_START_ADDR	(InterphaseOver_START_ADDR+InterphaseOver_NUM)
/* Խ�ޱ��� */
#define ThresholdAlarm_NUM 26
#define ThresholdAlarm_START_ADDR	(ZeroSequenceOver_START_ADDR+ZeroSequenceOver_NUM)
#define START_TO_ZERO_NUM 14//��ʼ��ַ������ֵ����Ŀ
#define Threshold_ZERO_START_ADDR (ThresholdAlarm_START_ADDR+START_TO_ZERO_NUM)
#define ZERO_TO_DC_NUM 6
#define Threshold_DC_START_ADDR (Threshold_ZERO_START_ADDR+ZERO_TO_DC_NUM)
/* �ع��� */
#define OverloadMuch_NUM 4
#define OverloadMuch_START_ADDR	(ThresholdAlarm_START_ADDR + ThresholdAlarm_NUM)
/* ������ */
#define Overload_NUM 4
#define Overload_START_ADDR	(OverloadMuch_START_ADDR + OverloadMuch_NUM)
/* ������� */
#define BatterySet_NUM 9
#define BatterySet_START_ADDR	(Overload_START_ADDR+Overload_NUM)
/* �Զ����� */
#define AutoReset_NUM 2
#define AutoReset_START_ADDR	(BatterySet_START_ADDR+BatterySet_NUM)
/* ����ѹ */
#define OverVoltage_NUM 16
#define OverVoltage_START_ADDR	(AutoReset_START_ADDR+AutoReset_NUM)
/* �ϻ����� */
#define RingUnite_NUM 3
#define RingUnite_START_ADDR	(OverVoltage_START_ADDR+OverVoltage_NUM)
/* ������ */
#define BreakDown_NUM 1
#define BreakDown_START_ADDR	(RingUnite_START_ADDR+RingUnite_NUM)
/* ���汣�� */
#define CutoutSwitch_NUM 18
#define CutoutSwitch_START_ADDR	0x8401
/* �߼����� */
#define LoadSwitch_NUM 18
#define LoadSwitch_START_ADDR	0x8501
/*----------------SOE---------------------*/
#define SOE_NUM 113//SOE����
#define SOE_START_ADDR YAOXIN_START_ADDR//SOE��ʼ��ַ
#define START_TO_SHORTBD_NUM 12//��ʼ��ַ����·���ϵ���Ŀ
#define START_TO_SHORTBD_ADDR (SOE_START_ADDR+START_TO_SHORTBD_NUM)//SOE����������ַ
#define START_TO_AOVERP_NUM 15//��·���ϵ�A�����һ�α�������Ŀ
#define START_TO_AOVERP_ADDR (START_TO_SHORTBD_ADDR+START_TO_AOVERP_NUM)//A�����һ�α����ĵ�ַ
#define START_TO_POSTACCELE_NUM 11//�����һ�α�����A�����ٹ�������Ŀ
#define START_TO_POSTACCELE_ADDR (START_TO_AOVERP_ADDR+START_TO_POSTACCELE_NUM)//A�����ٹ���
#define START_TO_TWOCLOSE_NUM 13//A�����ٹ����������غ�բ����Ŀ
#define START_TO_TWOCLOSE_ADDR (START_TO_POSTACCELE_ADDR+START_TO_TWOCLOSE_NUM)//�����غ�բ��ַ
#define START_TO_VLIMIT_A_NUM 17//�����غ�բ��A���ѹԽ�ߵ���Ŀ
#define START_TO_VLIMIT_A_ADDR (START_TO_TWOCLOSE_ADDR+START_TO_VLIMIT_A_NUM)//�����غ�բ��ַ

/*----------------ң��---------------------*/
#define CmdSend_NUM	4

#define CS_OPENSWITCH_NUM	0
#define CS_CLOSESWITCH_NUM	0
#define CS_RESETKEY_NUM		0
#define CS_SOECLEAR_MENUCMD_NUM	0

/*-------------�������--------------------*/
#define PassWord_NUM	1

/*=========================================*/
#elif (USEING_TAB == USE_TAB_TWO)

/* ң�� */
#define YAOXIN_NUM 27
#define YAOXIN_START_ADDR 0x0001

/* ң�� */
#define YAOCE1_NUM 19
#define YAOCE2_NUM 19
#define YAOCE_NUM  33//ң������
#define YAOCE_START_ADDR 0x4001
#define HARMONIC_NUM 16//г������
#define HARMONIC_START_ADDR (YAOCE_START_ADDR+YAOCE_NUM)

/* ��ֵ*/

/* ���в�����ַ */
#define ConstParameter_NUM 10
#define ConstParameter_START_ADDR	(0x8001)
/* �������� */
#define BasicConfigure_NUM 15
#define BasicConfigure_START_ADDR	(0x8101)
/* ��Ư���� */
#define ZeroDrift_NUM 18
#define ZeroDrift_START_ADDR	(BasicConfigure_START_ADDR+BasicConfigure_NUM)
/* �������� */
#define DeadZone_NUM 18
#define DeadZone_START_ADDR	(ZeroDrift_START_ADDR+ZeroDrift_NUM)
/* ������ */
#define InterphaseOver_NUM 12
#define InterphaseOver_START_ADDR	0x8301
/* ������� */
#define ZeroSequenceOver_NUM 6
#define ZeroSequenceOver_START_ADDR	(InterphaseOver_START_ADDR+InterphaseOver_NUM)
/* Խ�ޱ��� */
#define ThresholdAlarm_NUM 26
#define ThresholdAlarm_START_ADDR	(ZeroSequenceOver_START_ADDR+ZeroSequenceOver_NUM)
#define START_TO_ZERO_NUM 14//��ʼ��ַ������ֵ����Ŀ
#define Threshold_ZERO_START_ADDR (ThresholdAlarm_START_ADDR+START_TO_ZERO_NUM)
#define ZERO_TO_DC_NUM 6
#define Threshold_DC_START_ADDR (Threshold_ZERO_START_ADDR+ZERO_TO_DC_NUM)
/* �ع��� */
#define OverloadMuch_NUM 4
#define OverloadMuch_START_ADDR	(ThresholdAlarm_START_ADDR + ThresholdAlarm_NUM)
/* ������ */
#define Overload_NUM 4
#define Overload_START_ADDR	(OverloadMuch_START_ADDR + OverloadMuch_NUM)
/* ������� */
#define BatterySet_NUM 9
#define BatterySet_START_ADDR	(Overload_START_ADDR+Overload_NUM)
/* �Զ����� */
#define AutoReset_NUM 2
#define AutoReset_START_ADDR	(BatterySet_START_ADDR+BatterySet_NUM)
/* ����ѹ */
#define OverVoltage_NUM 4
#define OverVoltage_START_ADDR	(AutoReset_START_ADDR+AutoReset_NUM)
/* �ϻ����� */
#define RingUnite_NUM 3
#define RingUnite_START_ADDR	(OverVoltage_START_ADDR+OverVoltage_NUM)
/* ������ */
#define BreakDown_NUM 1
#define BreakDown_START_ADDR	(RingUnite_START_ADDR+RingUnite_NUM)
/* ���汣�� */
#define CutoutSwitch_NUM 16
#define CutoutSwitch_START_ADDR	0x8401
/* �߼����� */
#define LoadSwitch_NUM 25
#define LoadSwitch_START_ADDR	0x8501
/*----------------SOE---------------------*/
#define SOE_NUM 110//SOE����
#define SOE_START_ADDR YAOXIN_START_ADDR//SOE��ʼ��ַ
#define START_TO_SHORTBD_NUM 12//��ʼ��ַ����·���ϵ���Ŀ
#define START_TO_SHORTBD_ADDR (SOE_START_ADDR+START_TO_SHORTBD_NUM)//SOE����������ַ
#define START_TO_AOVERP_NUM 15//��·���ϵ�A�����һ�α�������Ŀ
#define START_TO_AOVERP_ADDR (START_TO_SHORTBD_ADDR+START_TO_AOVERP_NUM)//A�����һ�α����ĵ�ַ
#define START_TO_POSTACCELE_NUM 11//�����һ�α�����A�����ٹ�������Ŀ
#define START_TO_POSTACCELE_ADDR (START_TO_AOVERP_ADDR+START_TO_POSTACCELE_NUM)//A�����ٹ���
#define START_TO_TWOCLOSE_NUM 13//A�����ٹ����������غ�բ����Ŀ
#define START_TO_TWOCLOSE_ADDR (START_TO_POSTACCELE_ADDR+START_TO_TWOCLOSE_NUM)//�����غ�բ��ַ
#define START_TO_VLIMIT_A_NUM 14//�����غ�բ��A���ѹԽ�ߵ���Ŀ
#define START_TO_VLIMIT_A_ADDR (START_TO_TWOCLOSE_ADDR+START_TO_VLIMIT_A_NUM)//�����غ�բ��ַ

/*----------------ң��---------------------*/
#define CmdSend_NUM	4

#define CS_OPENSWITCH_NUM	OPENSWITCH_NUM
#define CS_CLOSESWITCH_NUM	CLOSESWITCH_NUM
#define CS_RESETKEY_NUM		RESETKEY_NUM
#define CS_SOECLEAR_MENUCMD_NUM	SOECLEAR_MENUCMD_NUM

/*-------------�������--------------------*/
#define PassWord_NUM	1

#endif

extern const struct YaoXinItem yaoXinItems[12];
extern const struct YaoCeItem yaoCe1Items[YAOCE1_NUM];
extern const struct YaoCeItem yaoCe2Items[YAOCE2_NUM];
extern const struct YaoCeItem yaoCe3Items[HARMONIC_NUM];

/* ���ɿ���-�߼����� */
extern const struct DingZhi1Item LoadSwitchItems[LoadSwitch_NUM];
/* Խ�߱��� */
extern const struct DingZhi1Item ThresholdAlarmItems[ThresholdAlarm_NUM];
/* �ع��� */
extern const struct DingZhi1Item OverloadMuchItems[OverloadMuch_NUM];
/* ������ */
extern const struct DingZhi1Item OverloadItems[Overload_NUM];
/* ����ѹ */
extern const struct DingZhi1Item OverVoltageItems[OverVoltage_NUM];
/* ������� */
extern const struct DingZhi1Item BatterySetItems[BatterySet_NUM];
/* �Զ����� */
extern const struct DingZhi1Item AutoResetItems[AutoReset_NUM];
/* ������ */
extern const struct DingZhi1Item InterphaseOverItems[InterphaseOver_NUM - 3];
/* ������� */
extern const struct DingZhi1Item ZeroSequenceOverItems[ZeroSequenceOver_NUM];
/*��·������ */
extern const struct DingZhi1Item CutoutSwitchItems[CutoutSwitch_NUM];
/* �������� */
extern const struct DingZhi1Item DeadZoneItems[DeadZone_NUM];
/* ��Ư���� */
extern const struct DingZhi1Item ZeroDriftItems[ZeroDrift_NUM];
/* �������� */
extern const struct DingZhi1Item BasicConfigureItems[BasicConfigure_NUM];
/* SOE */
extern const struct SOEItem SOEItems[SOE_NUM];
/* ���в��� */
extern const struct ConstParameter ConstParItems[ConstParameter_NUM];
/* �����·� */
extern const struct CmdSend CmdSendItems[CmdSend_NUM];
/* ������� */
extern const struct PassWordMessage PassWordItems[PassWord_NUM];
/* �ϻ� */
extern const struct DingZhi1Item RingUniteItems[RingUnite_NUM];
/* ������ */
extern const struct DingZhi1Item BreakDownItems[BreakDown_NUM];

extern const uint8_t *HMiVersion;
#endif
