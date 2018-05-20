
#include "userVariable.h"
#include <rtthread.h>

/* ң����ʾ��Ϣ */
YaoxinDisplayInfo yxInfo;
/* ң����ʾ��Ϣ */
YaoceDisplayInfo yceInfo[3];

/**
  *@brief  ң����ʾ��ʼ��
  *@param  None
  *@retval None
  */
static void YaoxinDisplayInit(void)
{
	/* ң����ʾ���� */
	#define YAOXIN_MAXITEMS 12
	
	uint8_t YaoxinItem = 0;
	uint8_t i = 0;
	
	yxInfo.pRoot = TelesignalCfg;
	for(i = 0; i < YAOXIN_MAXITEMS; i++){//���ҿ���ң��
		if(yxInfo.pRoot[i].enable == 0){
			continue;
		}
		YaoxinItem ++;
	}
	yxInfo.Num = YaoxinItem;
	yxInfo.pBuff = (uint8_t *)rt_malloc(yxInfo.Num);
	if(yxInfo.pBuff == NULL){
		rt_kprintf("ң���ڴ��ȡʧ��");
		return;
	}
	YaoxinItem = 0;
	for(i = 0; i < YAOXIN_MAXITEMS; i++){//ң����ʾ����
		if(yxInfo.pRoot[i].enable == 0){
			continue;
		}
		yxInfo.pBuff[YaoxinItem ++] = i;
	}
}

/**
  *@brief  ң����ʾ��ʼ��
  *@param  None
  *@retval None
  */
static void YaoceDisplayInit(void)
{
	uint8_t yaoceItemsAll,typeIs;
	uint8_t YaoceItem = 0;
	uint8_t i,j;
	
	yaoceItemsAll = sizeof(TelemetryCfg)/sizeof(tagTelemetryCfg);
	for(i = 0; i < 3; i++){
		if(i == 0){
			typeIs = SECONDRY;
		}
		else if(i == 1){
			typeIs = ONCE;
		}
		else{
			typeIs = HARMONIC;
		}
		/* ��ȡң��ʵ��ָ�� */
		yceInfo[i].pRoot = TelemetryCfg;	
		for(j = 0; j < yaoceItemsAll; j++){//���ҿ���ң��
			if(yceInfo[i].pRoot[j].enable == 0 && yceInfo[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			YaoceItem ++;
		}
		yceInfo[i].num = YaoceItem;
		yceInfo[i].pBuff = (uint8_t *)rt_malloc(yceInfo[i].num);
		if(yceInfo[i].pBuff == NULL){
			rt_kprintf("ң���ڴ��ȡʧ��");
			return;
		}
		YaoceItem = 0;
		for(j = 0; j < yaoceItemsAll; j++){//ң����ʾ����
			if(yceInfo[i].pRoot[j].enable == 0 && yceInfo[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			yceInfo[i].pBuff[YaoceItem ++] = j;
		}
	}
}

/**
  *@brief  �û���ʾ���ݳ�ʼ��
  *@param  None
  *@retval None
  */
void userVariableDisplayInit(void)
{
	YaoxinDisplayInit();
	YaoceDisplayInit();
}

const struct YaoCeItem yaoCe1Items[YAOCE1_NUM] = {
	{ YAOCE_START_ADDR,	   8,3,"f",  {"Hz", ""}},
	{(YAOCE_START_ADDR+20),8,3,"Ia", {"A","KA"}},
	{(YAOCE_START_ADDR+21),8,3,"Ib", {"A","KA"}},
	{(YAOCE_START_ADDR+22),8,3,"Ic", {"A","KA"}},
	{(YAOCE_START_ADDR+23),8,3,"I0", {"A","KA"}},
	{(YAOCE_START_ADDR+24),8,3,"Uab",{"V","KV"}},
	{(YAOCE_START_ADDR+25),8,3,"Ubc",{"V","KV"}},
	{(YAOCE_START_ADDR+26),8,3,"Uac",{"V","KV"}},
	{(YAOCE_START_ADDR+27),8,3,"U0", {"V","KV"}},
	{(YAOCE_START_ADDR+28),8,3,"UAB",{"V","KV"}},
	{(YAOCE_START_ADDR+29),8,3,"UBC",{"V","KV"}},
	{(YAOCE_START_ADDR+30),8,3,"P",  {"KV","" }},
	{(YAOCE_START_ADDR+31),8,3,"Q",  {"Kvar",""}},
	{(YAOCE_START_ADDR+32),8,3,"S",  {"KVA",""}},
	{(YAOCE_START_ADDR+14),8,3,"pf", {"",   ""}},
	{(YAOCE_START_ADDR+15),8,3,"��0", {"��","--"}},
	{(YAOCE_START_ADDR+16),8,3,"��ab", {"��","--"}},	
	{(YAOCE_START_ADDR+17),8,3,"DC1",{"V","KV"}},
	{(YAOCE_START_ADDR+18),8,3,"DC2",{"V","KV"}},
	{(YAOCE_START_ADDR+19),8,3,"T",  {"��", ""}}
};
const struct YaoCeItem yaoCe2Items[YAOCE2_NUM] = {
	{ YAOCE_START_ADDR,	  8,3,"f",  {"Hz", ""}},
	{(YAOCE_START_ADDR+1),8,3,"Ia", {"A","KA"}},
	{(YAOCE_START_ADDR+2),8,3,"Ib", {"A","KA"}},
	{(YAOCE_START_ADDR+3),8,3,"Ic", {"A","KA"}},
	{(YAOCE_START_ADDR+4),8,3,"I0", {"A","KA"}},
	{(YAOCE_START_ADDR+5),8,3,"Uab",{"V","KV"}},
	{(YAOCE_START_ADDR+6),8,3,"Ubc",{"V","KV"}},
	{(YAOCE_START_ADDR+7),8,3,"Uac",{"V","KV"}},
	{(YAOCE_START_ADDR+8),8,3,"U0", {"V","KV"}},
	{(YAOCE_START_ADDR+9),8,3,"UAB",{"V","KV"}},
	{(YAOCE_START_ADDR+10),8,3,"UBC",{"V","KV"}},
	{(YAOCE_START_ADDR+11),8,3,"P",  {"KV","" }},
	{(YAOCE_START_ADDR+12),8,3,"Q",  {"Kvar",""}},
	{(YAOCE_START_ADDR+13),8,3,"S",  {"KVA",""}},
	{(YAOCE_START_ADDR+14),8,3,"pf", {"",   ""}},
	{(YAOCE_START_ADDR+15),8,3,"��0", {"��","--"}},
	{(YAOCE_START_ADDR+16),8,3,"��ab", {"��","--"}},	
	{(YAOCE_START_ADDR+17),8,3,"DC1",{"V","KV"}},
	{(YAOCE_START_ADDR+18),8,3,"DC2",{"V","KV"}},
	{(YAOCE_START_ADDR+19),8,3,"T",  {"��", ""}}
};

const struct YaoCeItem yaoCe3Items[HARMONIC_NUM] = {
	{HARMONIC_START_ADDR,	 8,3,"����Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+1),8,3,"����Ub",{"V","KV"}},
	{(HARMONIC_START_ADDR+2),8,3,"����Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+3),8,3,"����U0",{"V","KV"}},
	{(HARMONIC_START_ADDR+4),8,3,"����Ia",{"A","KA"}},
	{(HARMONIC_START_ADDR+5),8,3,"����Ib",{"A","KA"}},
	{(HARMONIC_START_ADDR+6),8,3,"����Ic",{"A","KA"}},
	{(HARMONIC_START_ADDR+7),8,3,"����I0",{"A","KA"}},
	
	{(HARMONIC_START_ADDR+8),8,3, "���Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+9),8,3, "���Ub",{"V","KV"}},
	{(HARMONIC_START_ADDR+10),8,3,"���Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+11),8,3,"���U0",{"V","KV"}},
	{(HARMONIC_START_ADDR+12),8,3,"���Ia",{"A","KA"}},
	{(HARMONIC_START_ADDR+13),8,3,"���Ib",{"A","KA"}},
	{(HARMONIC_START_ADDR+14),8,3,"���Ic",{"A","KA"}},
	{(HARMONIC_START_ADDR+15),8,3,"���I0",{"A","KA"}},
};
const struct DingZhi1Item InterphaseOverItems[InterphaseOver_NUM - 3]={/* ������ */
	{InterphaseOver_START_ADDR,    "1�ι���Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(InterphaseOver_START_ADDR+2),"1�ι�������",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(InterphaseOver_START_ADDR+3),"1�ι���ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},	
	{(InterphaseOver_START_ADDR+4),"2�ι���Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},	
	{(InterphaseOver_START_ADDR+6),"2�ι�������",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(InterphaseOver_START_ADDR+7),"2�ι���ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},	
	{(InterphaseOver_START_ADDR+8),"3�ι���Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(InterphaseOver_START_ADDR+10),"3�ι�������",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(InterphaseOver_START_ADDR+11),"3�ι���ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item ZeroSequenceOverItems[ZeroSequenceOver_NUM]={/* ������� */
	{ZeroSequenceOver_START_ADDR,    "1�ι���Ͷ��",3,"0","0",0,0,    {"-","-"},{"��","Ͷ","�澯"}},
	{(ZeroSequenceOver_START_ADDR+1),"1�ι�������",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(ZeroSequenceOver_START_ADDR+2),"1�ι���ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(ZeroSequenceOver_START_ADDR+3),"2�ι���Ͷ��",3,"0","0",0,0,    {"-","-"},{"��","Ͷ","�澯"}},
	{(ZeroSequenceOver_START_ADDR+4),"2�ι�������",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(ZeroSequenceOver_START_ADDR+5),"2�ι���ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item ThresholdAlarmItems[ThresholdAlarm_NUM]={/* Խ�߱��� */
	//name ��־ max min float �� С�� ��λ ����
	{ThresholdAlarm_START_ADDR,    "Խ�ޱ���Ͷ��",2,"0","0",0,0,{"-","-"},{"��","Ͷ"}},
	{(ThresholdAlarm_START_ADDR+1),"Խ������ϵ��",0,"1","0",5,2,{"-","-"},{"-","-"}},
	{(ThresholdAlarm_START_ADDR+2),"Խ������ϵ��",0,"2","1",5,2,{"-","-"},{"-","-"},},
	{(ThresholdAlarm_START_ADDR+3),"Խ����ʱ",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	
	{(ThresholdAlarm_START_ADDR+4),"UabԽ��Ͷ��",2,"0","0",0,0,  {"-","-"},{"��","Ͷ"}},//Uab
	{(ThresholdAlarm_START_ADDR+5),"UbcԽ��Ͷ��",2,"0","0",0,0,  {"-","-"},{"��","Ͷ"}},//Ubc
	{(ThresholdAlarm_START_ADDR+6),"UacԽ��Ͷ��",2,"0","0",0,0,  {"-","-"},{"��","Ͷ"}},//Uac
    {(ThresholdAlarm_START_ADDR+7),"U���޶�ֵ",0,"999.9","0",6,2,{"V","KV"},{"-","-"}},
	{(ThresholdAlarm_START_ADDR+8),"U���޶�ֵ",0,"999.9","0",6,2,{"V","KV"},{"-","-"}},
	
    {(ThresholdAlarm_START_ADDR+9), "IaԽ��Ͷ��",2,"0","0",0,0,   {"-","-"},{"��","Ͷ"}},//Ia
	{(ThresholdAlarm_START_ADDR+10),"IbԽ��Ͷ��",2,"0","0",0,0,   {"-","-"},{"��","Ͷ"}},//Ib
	{(ThresholdAlarm_START_ADDR+11),"IcԽ��Ͷ��",2,"0","0",0,0,   {"-","-"},{"��","Ͷ"}},//Ic
	{(ThresholdAlarm_START_ADDR+12),"I���޶�ֵ",0,"99.99","0",6,2,{"A","KA"},{"-","-"}},
	{(ThresholdAlarm_START_ADDR+13),"I���޶�ֵ",0,"99.99","0",6,2,{"A","KA"},{"-","-"}},
	
	{Threshold_ZERO_START_ADDR,    "U0Խ��Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},//U0
	{(Threshold_ZERO_START_ADDR+1),"U0���޶�ֵ",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_ZERO_START_ADDR+2),"U0���޶�ֵ",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_ZERO_START_ADDR+3),"I0Խ��Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},//Ia
	{(Threshold_ZERO_START_ADDR+4),"I0���޶�ֵ",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(Threshold_ZERO_START_ADDR+5),"I0���޶�ֵ",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	
	{Threshold_DC_START_ADDR,	 "U_DCԽ��Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},//U_DC
	{(Threshold_DC_START_ADDR+1),"U_DC���޶�ֵ",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_DC_START_ADDR+2),"U_DC���޶�ֵ",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_DC_START_ADDR+3),"I_DCԽ��Ͷ��",2,"0","0",0,0,	  {"-","-"},{"��","Ͷ"}},//I_DC
	{(Threshold_DC_START_ADDR+4),"I_DC���޶�ֵ",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(Threshold_DC_START_ADDR+5),"I_DC���޶�ֵ",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
};

const struct DingZhi1Item OverloadMuchItems[OverloadMuch_NUM]={/* �ع��� */
	{OverloadMuch_START_ADDR,    "�ع���Ͷ��",2,"0","0",0,0,   {"-","-"},{"��","Ͷ"}},
	{(OverloadMuch_START_ADDR+1),"�ع��ض�ֵ",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(OverloadMuch_START_ADDR+2),"�ع���ϵ��",0,"1","0",5,2,	{"-","-"},{"-","-"}},
	{(OverloadMuch_START_ADDR+3),"�ع�����ʱ",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item OverloadItems[Overload_NUM]={/* ������ */
	{Overload_START_ADDR,    "������Ͷ��",2,"0","0",0,0,   {"-","-"},{"��","Ͷ"}},
	{(Overload_START_ADDR+1),"�����ɶ�ֵ",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(Overload_START_ADDR+2),"������ϵ��",0,"1","0",5,2,	{"-","-"},{"-","-"}},
	{(Overload_START_ADDR+3),"��������ʱ",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item OverVoltageItems[OverVoltage_NUM]={/* ����ѹ */
	{OverVoltage_START_ADDR,    "����ѹͶ��",3,"0","0",0,0,    {"-","-"},{"��","Ͷ","�澯"}},
	{(OverVoltage_START_ADDR+1),"����ѹ��ֵ",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(OverVoltage_START_ADDR+2),"����ѹʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+3),"����ѹϵ��",0,"1","0",5,2,	   {"-","-"},{"-","-"}},
	{(OverVoltage_START_ADDR+4),"��ƵͶ��",3,"0","0",0,0,    {"-","-"},{"��","Ͷ","�澯"}},
	{(OverVoltage_START_ADDR+5),"��Ƶ��ֵ",0,"99.99","0",5,2,{"HZ","HZ"},{"-","-"}},
	{(OverVoltage_START_ADDR+6),"��Ƶʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+7),"��Ƶϵ��",0,"1","0",5,2,	   {"-","-"},{"-","-"}},
	{(OverVoltage_START_ADDR+8),"��ѹͶ��",3,"0","0",0,0,    {"-","-"},{"��","Ͷ","�澯"}},
	{(OverVoltage_START_ADDR+9),"��ѹ��ֵ",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(OverVoltage_START_ADDR+10),"��ѹʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+11),"��ѹϵ��",0,"2","1",5,2,	   {"-","-"},{"-","-"}},
	{(OverVoltage_START_ADDR+12),"��ƵͶ��",3,"0","0",0,0,    {"-","-"},{"��","Ͷ","�澯"}},
	{(OverVoltage_START_ADDR+13),"��Ƶ��ֵ",0,"99.99","0",5,2,{"HZ","HZ"},{"-","-"}},
	{(OverVoltage_START_ADDR+14),"��Ƶʱ��",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+15),"��Ƶϵ��",0,"2","1",5,2,	   {"-","-"},{"-","-"}},
};

const struct DingZhi1Item BatterySetItems[BatterySet_NUM]={/* ������� */
	{BatterySet_START_ADDR,    "��ѹ����Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(BatterySet_START_ADDR+1),"��ص�ѹ��ֵ",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(BatterySet_START_ADDR+2),"��ص�ѹϵ��",0,"1","0",5,2,	   {"-","-"},{"-","-"}},
	{(BatterySet_START_ADDR+3),"��ص�ѹ��ʱ",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	
	{(BatterySet_START_ADDR+4),"��ػͶ��",2,"0","0",0,0,    {"-", "-"},{"��","Ͷ"}},
	{(BatterySet_START_ADDR+5),"��ػ����",0,"99999","0",5,2,{"��","h"},{"-","-"}},
	{(BatterySet_START_ADDR+6),"��ػʱ��",0,"23","0",5,2,	 {"h", "-"},{"-","-"}},
	{(BatterySet_START_ADDR+7),"����ϵ�ѹ",0,"9999", "0",5,2,{"V","KV"},{"-","-"}},
	{(BatterySet_START_ADDR+8),"�������ʱ",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
};
const struct DingZhi1Item AutoResetItems[AutoReset_NUM]={/* �Զ����� */
	{AutoReset_START_ADDR,    "�Զ�����Ͷ��",2,"0","0",0,0,      {"-","-"},{"��","Ͷ"}},
	{(AutoReset_START_ADDR+1),"�Զ�����ʱ��",0,"9999999","0",7,0,{"s","ms"},{"-","-"}},
};
const struct DingZhi1Item RingUniteItems[RingUnite_NUM]={/* �ϻ� */
	{RingUnite_START_ADDR,    "�ϻ�Ͷ��",2,"0","0",0,0,      {"-","-"},{"��","Ͷ"}},
	{(RingUnite_START_ADDR+1),"����ѹ��",0,"30","0",5,2,     {"V","KV"},{"-","-"}},
	{(RingUnite_START_ADDR+2),"��ǲ�",  0,"10","0",5,2,     {"��","-"},{"-","-"}},
};
const struct DingZhi1Item BreakDownItems[BreakDown_NUM]={/* ����Ͷ�� */
	{BreakDown_START_ADDR,    "���ƻ�·����",2,"0","0",0,0,  {"-","-"},{"��","Ͷ"}},
};
const struct DingZhi1Item LoadSwitchItems[LoadSwitch_NUM]={/* �߼����� */
	{LoadSwitch_START_ADDR,     "������ֵ",    0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(LoadSwitch_START_ADDR+1), "��ѹ��ֵ",    0,"9999","0",5,2,{"V","KV"},{"-","-"}},
	{(LoadSwitch_START_ADDR+2), "Y��ʱ",       0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+3), "��բ����ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+4), "ʧ���բͶ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(LoadSwitch_START_ADDR+5), "ʧ���բʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+6), "�õ��բͶ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(LoadSwitch_START_ADDR+7), "�õ��բʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+8),"����ʧѹ��բ",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(LoadSwitch_START_ADDR+9),"����ʧѹ��ʱ",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+10),"˫����ѹͶ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(LoadSwitch_START_ADDR+11),"��ѹ����Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(LoadSwitch_START_ADDR+12),"��ѹ������ѹ",0,"99.99","0",5,2,{"V","KV"},{"-","-"}},
	{(LoadSwitch_START_ADDR+13),"U0����Ͷ��",  2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(LoadSwitch_START_ADDR+14),"U0������ѹ",  0,"99.99","0",5,2,{"V","KV"},{"-","-"}},
	{(LoadSwitch_START_ADDR+15),"U0��ѹ��ʱ",  0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+16),"���ڶ�Ͷ��",  2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(LoadSwitch_START_ADDR+17),"���ڶϵ���",  0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
};
const struct DingZhi1Item CutoutSwitchItems[CutoutSwitch_NUM]={/* ��·�� */
	{CutoutSwitch_START_ADDR,    "��ʱ��Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(CutoutSwitch_START_ADDR+1),"��ʱ�޶�ֵ",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+2),"��ʱ����ʱ",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+3),"�����Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(CutoutSwitch_START_ADDR+4),"����ٵ���",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+5),"�����ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+6),"ӿ������Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(CutoutSwitch_START_ADDR+7),"ӿ������ʱ��",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+8),"�غϱ���Ͷ��",2,"0","0",0,0,    {"-","-"},{"��","Ͷ"}},
	{(CutoutSwitch_START_ADDR+9),"�غϱ���ʱ��",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+10),"����غϴ���",0,"5","0",5,0,    {"��","-"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+11),"�����غϴ���",0,"2","0",5,2,    {"��","-"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+12),"�غ�բ1ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+13),"�غ�բ2ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+14),"�غ�բ3ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+15),"�غ�բ4ʱ��",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+16),"�غϸ���ʱ��",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+17),"�غϳ��ʱ��",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item DeadZoneItems[DeadZone_NUM]={/* �������� */
	{DeadZone_START_ADDR,    "Ƶ��",    0,"99.99","0",5,2,{"%","Hz"},{"-","-"}},
	{(DeadZone_START_ADDR+1),"����A��", 0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+2),"����B��", 0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+3),"����C��", 0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+4),"�������",0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+5),"�ߵ�ѹab",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+6),"�ߵ�ѹbc",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+7),"�ߵ�ѹac",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+8),"�����ѹ",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+9), "�ߵ�ѹAB", 0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+10),"�ߵ�ѹBC", 0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+11),"�й�����", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+12),"�޹�����", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+13),"���ڹ���", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+14),"��������", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+15),"ֱ����ѹ1",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+16),"ֱ����ѹ2",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+17),"�¶�",     0,"99.99","0",5,2,{"%","-"},{"-","-"}},
};

const struct DingZhi1Item ZeroDriftItems[ZeroDrift_NUM]={/* ��Ư���� */
	{ZeroDrift_START_ADDR,     "Ƶ��",     0,"99.99","0",5,2,{"Hz","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+1), "����A��",  0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+2), "����B��",  0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+3), "����C��",  0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+4), "�������", 0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+5), "�ߵ�ѹab", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+6), "�ߵ�ѹbc", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+7), "�ߵ�ѹac", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+8), "�����ѹ", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},	
	{(ZeroDrift_START_ADDR+9), "�ߵ�ѹAB", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+10),"�ߵ�ѹBC", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+11),"�й�����", 0,"99.99","0",5,2,{"KW","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+12),"�޹�����", 0,"99.99","0",5,2,{"Kvr","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+13),"���ڹ���", 0,"99.99","0",5,2,{"KVA","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+14),"��������", 0,"99.99","0",5,2,{"-","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+15),"ֱ����ѹ1",0,"99.99","0",5,2,{"V","V"},{"-","-"}},
	{(ZeroDrift_START_ADDR+16),"ֱ����ѹ2",0,"99.99","0",5,2,{"V","V"},{"-","-"}},
	{(ZeroDrift_START_ADDR+17),"�¶�",     0,"99.99","0",5,2,{"��","-"},{"-","-"}},
};
const struct DingZhi1Item BasicConfigureItems[BasicConfigure_NUM]={/* �������� */
	{BasicConfigure_START_ADDR,     "��������",    2,"0","0",0,0,{"-","-"},{"����","����"}},
	{(BasicConfigure_START_ADDR+1), "��������",    2,"0","0",0,0,{"-","-"},{"��·��","���ɿ���"}},
	{(BasicConfigure_START_ADDR+2), "��·��ģʽ",  5,"0","0",0,0,{"-","-"},{"��","���汣��","��ѹʱ����","��ѹ������","����������"}},
	{(BasicConfigure_START_ADDR+3), "���ɿ���ģʽ",5,"0","0",0,0,{"-","-"},{"��","��ѹʱ����","��ѹ������","����������","�ֽ�"}},
	{(BasicConfigure_START_ADDR+4), "���翪��ʶ��",2,"0","0",0,0,{"-","-"},{"�ֶ�","�Զ�"}},
	{(BasicConfigure_START_ADDR+5), "���������ת",2,"0","0",5,2,{"-","-"},{"����ת","��ת"}},
	{(BasicConfigure_START_ADDR+6), "��բǷѹ��ֵ",0,"250","0",6,0,{"V","KV"},{"-","-"}},	
	{(BasicConfigure_START_ADDR+7), "��բǷѹ��ֵ",0,"250","0",6,0,{"V","KV"},{"-","-"}},
	{(BasicConfigure_START_ADDR+8), "����Ƿѹϵ��",0,"2","1",5,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+9), "һ�α��_U", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+10), "���α��_U", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+11),"һ�α��_U0",0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+12),"���α��_U0",0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+13),"һ�α��_I", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+14),"���α��_I", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+15),"һ�α��_I0",0,"1000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+16),"���α��_I0",0,"1000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+17),"�������",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+18),"��բ����",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+19),"��բ����",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+20),"��Уʱ��",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
};
const struct SOEItem SOEItems[SOE_NUM] = {//SOE
	{SOE_START_ADDR,    "���ط�λ",      {"��->��","��->��"}},
    {(SOE_START_ADDR+1),"���غ�λ",      {"��->��","��->��"}},
    {(SOE_START_ADDR+2),"����/��ѹ",     {"��->��","��->��"}},	
	{(SOE_START_ADDR+3),"����ѹ",        {"�澯->��λ","��->�澯"}},	
	{(SOE_START_ADDR+4),"��Դ���ϸ澯",  {"�澯->��λ","��->�澯"}},
	{(SOE_START_ADDR+5),"���Ƿѹ�澯",  {"�澯->��λ","��->�澯"}},
	{(SOE_START_ADDR+6),"��ػ״̬",  {"�->����","��ʼ�"}},
	{(SOE_START_ADDR+7),"����ʧ��澯",  {"��->�澯","�澯->����"}},
	{(SOE_START_ADDR+8),"����ģʽ",      {"����->�ֶ�","�ֶ�->����"}},
	{(SOE_START_ADDR+9),"�غ�ѹ��/FA",   {"Ͷ->��","��->Ͷ"}},
    {(SOE_START_ADDR+10),"����ѹ��",     {"Ͷ->��","��->Ͷ"}},
	{(SOE_START_ADDR+11),"Զ��/�͵�",    {"Զ��->�͵�","�͵�->Զ��"}},
	
	{(START_TO_SHORTBD_ADDR),  "��������",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+1),"���򱣻�",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+2),"��������",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+3),"��������",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+4),"��բ����",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+5),"��բ����",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+6),"�غ϶���",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+7),"�غϱ���",{"��λ","����"}},
	{(START_TO_SHORTBD_ADDR+8),"װ�ù���",{"��","����"}},
	{(START_TO_SHORTBD_ADDR+9),"�Լ��쳣",{"��","�쳣"}},
	{(START_TO_SHORTBD_ADDR+10),"ͨ��",   {"��","ͨ"}},
	{(START_TO_SHORTBD_ADDR+11),"��������",{"��·��","���ɿ���"}},	
	{(START_TO_SHORTBD_ADDR+12),"��ص�ѹ�澯",{"�澯->����","��->�澯"}},
	{(START_TO_SHORTBD_ADDR+13),"˫��ң��",    {"��->��","��->��"}},
	{(START_TO_SHORTBD_ADDR+14),"���ƻ�·����",{"��λ","����"}},
	
	{(START_TO_AOVERP_ADDR),  "A�����һ��",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+1),"B�����һ��",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+2),"C�����һ��",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+3),"A���������",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+4),"B���������",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+5),"C���������",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+6),"A���������",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+7),"B���������",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+8),"C���������",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+9), "�������һ��",{"��λ","����"}},
	{(START_TO_AOVERP_ADDR+10),"�����������",{"��λ","����"}},
	
	{(START_TO_POSTACCELE_ADDR),  "A������",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+1),"B������",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+2),"C������",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+3),"A�෴ʱ��",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+4),"B�෴ʱ��",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+5),"C�෴ʱ��",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+6),"�غ�բһ��",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+7),"�غ�բ����",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+8),"�غ�բ����",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+9),"�غ�բ�Ĵ�",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+10),"�غ�բ����",  {"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+11),"�����غ�һ��",{"��λ","����"}},
	{(START_TO_POSTACCELE_ADDR+12),"�����غ϶���",{"��λ","����"}},
	
	{(START_TO_TWOCLOSE_ADDR),  "�����غϱ���",{"��λ","����"}},	
	{(START_TO_TWOCLOSE_ADDR+1),"ʧ����բ",    {"��λ","��բ"}},
	{(START_TO_TWOCLOSE_ADDR+2),"�õ��բ",    {"��λ","��բ"}},
	{(START_TO_TWOCLOSE_ADDR+3),"��բ������",  {"��","����"}},
	{(START_TO_TWOCLOSE_ADDR+4),"��������",    {"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+5),"˫ѹ��ֹ��բ",{"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+6),"����ʧѹ��բ",{"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+7),"��ѹ������բ",{"��λ","����"}},	
	{(START_TO_TWOCLOSE_ADDR+8),"�����ѹ�ӵ�",{"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+9),"С�źŽӵ�",  {"��λ","��բ"}},
	{(START_TO_TWOCLOSE_ADDR+10),"�ڶϵ���",   {"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+11),"������",     {"��λ","�澯"}},
	{(START_TO_TWOCLOSE_ADDR+12),"�ع���",     {"��λ","�澯"}},
	{(START_TO_TWOCLOSE_ADDR+13),"��ѹ����",   {"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+14),"��Ƶ����",   {"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+15),"��ѹ����",   {"��λ","����"}},
	{(START_TO_TWOCLOSE_ADDR+16),"��Ƶ����",   {"��λ","����"}},
	
	{(START_TO_VLIMIT_A_ADDR),  "UabԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+1),"UabԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+2),"UcaԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+3),"UcaԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+4),"UBCԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+5),"UBCԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+6),"�����ѹԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+7),"�����ѹԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+8),"A�����Խ��", {"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+9),"A�����Խ��", {"��λ","Խ����"}},
	
	{(START_TO_VLIMIT_A_ADDR+10),"B�����Խ��", {"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+11),"B�����Խ��", {"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+12),"C�����Խ��", {"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+13),"C�����Խ��", {"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+14),"�������Խ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+15),"�������Խ��",{"��λ","Խ����"}},	
	{(START_TO_VLIMIT_A_ADDR+16),"ֱ����ѹԽ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+17),"ֱ����ѹԽ��",{"��λ","Խ����"}},	
	{(START_TO_VLIMIT_A_ADDR+18),"ֱ������Խ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+19),"ֱ������Խ��",{"��λ","Խ����"}},
	{(START_TO_VLIMIT_A_ADDR+20),"װ�õ���",    {"��λ","����"}},

	{0x00006001,"Զ����բ",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00016001,"Զ����բ",{"ִ��ʧ��","ִ�гɹ�"}},
	{0x00026001,"Զ����բ",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00036001,"Զ����բ",{"ִ��ʧ��","ִ�гɹ�"}},
	{0x00046001,"Զ���ϻ�",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00056001,"Զ���ϻ�",{"ִ��ʧ��","ִ�гɹ�"}},	
	{0x00066001,"ң��ȡ��",                {"",""}},
	{0x00006002,"Զ���",            {"��","ͨ"}},
	{0x00006003,"Զ����λ",        {"ʧ��","�ɹ�"}},
	{0x00006004,"Զ�������¼",    {"ʧ��","�ɹ�"}},
	{0x00006005,"���ط�բ",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00016005,"���ط�բ",{"ִ��ʧ��","ִ�гɹ�"}},
	{0x00026005,"���غ�բ",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00036005,"���غ�բ",{"ִ��ʧ��","ִ�гɹ�"}},
	{0x00046005,"���غϻ�",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00056005,"���غϻ�",{"ִ��ʧ��","ִ�гɹ�"}},
	{0x00006006,"���ظ�λ",        {"ʧ��","�ɹ�"}},
	{0x00006007,"���������¼",    {"ʧ��","�ɹ�"}},
	{0x00006008,"�ֳַ�բ",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00016008,"�ֳַ�բ",{"ִ��ʧ��","ִ�гɹ�"}},
	{0x00026008,"�ֳֺ�բ",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00036008,"�ֳֺ�բ",{"ִ��ʧ��","ִ�гɹ�"}},
	{0x00046008,"�ֳֺϻ�",{"ѡ��ʧ��","ѡ��ɹ�"}},
	{0x00056008,"�ֳֺϻ�",{"ִ��ʧ��","ִ�гɹ�"}},
};

const struct ConstParameter ConstParItems[ConstParameter_NUM] = {//���в���
	{ConstParameter_START_ADDR,    "�ն�����:"},	     									                                      
	{(ConstParameter_START_ADDR+1),"�ն˲���ϵͳ:"},
	{(ConstParameter_START_ADDR+2),"�ն�������:"},
	{(ConstParameter_START_ADDR+3),"�ն�Ӳ���汾:"},
	{(ConstParameter_START_ADDR+4),"�ն�����汾"},
	{(ConstParameter_START_ADDR+5),"�汾У����:"},
	{(ConstParameter_START_ADDR+6),"ͨ�Ź�Լ����:"},
	{(ConstParameter_START_ADDR+7),"�ն˳����ͺ�:"},
	{(ConstParameter_START_ADDR+8),"�ն�ID��:"},
	{(ConstParameter_START_ADDR+9),"�ն�����MAC��ַ:"},
	{(ConstParameter_START_ADDR+10),"�汾���к�:"},
};

const struct CmdSend CmdSendItems[CmdSend_NUM] = {//�����·�
	{CS_SOECLEAR_MENUCMD_NUM,"�����¼"},
	{CS_OPENSWITCH_NUM,		"��բ"},
	{CS_CLOSESWITCH_NUM,	"��բ"},
	{CS_RESETKEY_NUM,		"����"},
};

const struct PassWordMessage PassWordItems[PassWord_NUM] = {//�������
	{"100000","����������:"},
};

/* 0.1.2   0 Ϊ��汾�ţ�0ĿǰΪ���� 1 Ϊ ���� */
const uint8_t *HMiVersion = "180412";
