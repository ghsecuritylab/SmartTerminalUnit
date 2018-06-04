
#include "userVariable.h"
#include <rtthread.h>
#include "common_data.h"
#include "drv_do.h"

/* ң����ʾ��Ϣ */
YaoxinDisplayInfo yxInfo;
/* ң����ʾ��Ϣ 0һ�� 1���� 2 г�� */
YaoceDisplayInfo yceInfo[3];
/* ���ö�ֵ��ʾ��Ϣ */
DzhiDisplayInfo dzhi0Info[DZ0_ALLNUM];
/* ���ж�ֵ��ʾ��Ϣ */
DzhiDisplayInfo dzhi1Info[DZ1_ALLNUM];
/* Soe��ʾ��Ϣ */
SoeDisplayInfo soeInfo;
/* Co��ʾ��Ϣ */
SoeDisplayInfo coInfo;
/* �����¼���ʾ��Ϣ */
FeventDisplayInfo fEventInfo;
/* �����·� */
struct HmiCmdSend hmiCmdItems[] = {
	{"�����¼",0},
	{"��բ",1},
	{"��բ",2},
	{"����",3},
};
HmiCmdSendInfo hcmdInfo;
/* �汾��Ϣ */
VersionInfo versInfo;
/* ������� */
const struct PassWordMessage PassWordItems[PassWord_NUM] = {
	{"100000","����������:"},
};

/**
  *@brief  ��ȡϵͳʱ��
  *@param  None
  *@retval None
  */
void GetDisplayTime(SystemTimeDisplay *tim)
{
	uint16_t tms;
	struct CP56Time2a_t cpTime;
	DBReadSystemTime(&cpTime);
	tim->year = cpTime.year;
	tim->month = cpTime.month;
	tim->day = cpTime.dayofWeek & 0x1F;
	tim->hour = cpTime.hour;
	tim->min = cpTime.minute;
	tms = (cpTime.msecondH << 8) + cpTime.msecondL;
	tim->s = tms / 1000;
	tim->ms = tms % 1000;
}
/**
  *@brief  �޸�ϵͳʱ��
  *@param  None
  *@retval None
  */
void SetDisplayTime(SystemTimeDisplay *tim)
{
	uint16_t tms;
	struct CP56Time2a_t cpTime;
	DBReadSystemTime(&cpTime);
	cpTime.year = tim->year;
	cpTime.month = tim->month;
	cpTime.dayofWeek = ((tim->day & 0x1F) | (cpTime.dayofWeek &0xE0));
	cpTime.hour = tim->hour;
	cpTime.minute = tim->min;
	tms = tim->s * 1000 + tim->ms; 
	cpTime.msecondH = (uint8_t)(tms >> 8);
	cpTime.msecondL = (uint8_t)tms;
	DBWriteSystemTime(&cpTime);
}
/**
  *@brief  ң����ʾ��ʼ��
  *@param  None
  *@retval None
  */
static void YaoxinDisplayInit(void)
{
	/* ң����ʾ���� */
	uint8_t YaoxinItem = 0;
	uint8_t i = 0;
	
	yxInfo.pRoot = TelesignalCfg;
	for(i = 0; i < g_TelesignalCfg_Len; i++){//���ҿ���ң��
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
	for(i = 0; i < g_TelesignalCfg_Len; i++){//ң����ʾ����
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
	
	yaoceItemsAll = g_TelemetryCfg_Len;
	for(i = 0; i < 3; i++){
		if(i == 0){
			typeIs = ONCE;
		}
		else if(i == 1){
			typeIs = SECONDRY;
		}
		else{
			typeIs = HARMONIC;
		}
		/* ��ȡң��ʵ��ָ�� */
		yceInfo[i].pRoot = TelemetryCfg;
		YaoceItem = 0;
		for(j = 0; j < yaoceItemsAll; j++){//���ҿ���ң��
			if(yceInfo[i].pRoot[j].enable == 0 || yceInfo[i].pRoot[j].menuNum != typeIs){
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
			if(yceInfo[i].pRoot[j].enable == 0 || yceInfo[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			yceInfo[i].pBuff[YaoceItem ++] = j;
		}
	}
}

/**
  *@brief  ���ö�ֵ�޸ı���
  *@param  addr ��ַ��
  *@retval None
  */
static void Dzhi0ModfiySave(uint16_t addr)
{
	addr = addr;
	rt_multi_common_data_save_value_to_fram(0);
}

/**
  *@brief  ���ж�ֵ�޸ı���
  *@param  addr ��ַ��
  *@retval None
  */
static void Dzhi1ModfiySave(uint16_t addr)
{
	addr = addr;
	rt_multi_common_data_save_value_to_fram(g_ValueParaOperateInfo.currentSN);
}

/**
  *@brief  ���ж�ֵ������ӳ��
  *@param  i ��ǰ���к� type ԭʼ����
  *@retval None
  */
static void Dzhi0TypeRemap(uint16_t i,uint16_t *type)
{
	switch(i)
	{
		case DZ0_CONFIG:*type = ME_BASIC_SET;break;
		case DZ0_ZERODRIFT:*type = ME_ZERODRIFT;break;
		case DZ0_DEADEZONE:*type = ME_DEADEZONE;break;
		case DZ0_SERIAL:*type = ME_UART_COM;break;
		case DZ0_INTERNET:*type = ME_NET_COM;break;
		default:*type = ME_BASIC_SET;break;
	}
}
/**
  *@brief  ������ʾ��ʼ��
  *@param  None
  *@retval None
  */
static void Dzhi0DisplayInit(void)
{
	uint16_t dzhiItemsAll,typeIs;
	uint8_t dzhiItem = 0;
	uint16_t i,j;
	dzhiItemsAll = g_ParameterCfg_Len;
	for(i = 0 ; i < DZ0_ALLNUM; i++){
		Dzhi0TypeRemap(i,&typeIs);
		dzhiItem = 0;
		dzhi0Info[i].SaveModify = Dzhi0ModfiySave;
		dzhi0Info[i].pRoot = ParameterCfg;
		for(j = 0; j < dzhiItemsAll; j++){//���ҿ��ö�ֵ
			if(dzhi0Info[i].pRoot[j].enable == 0 || dzhi0Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhiItem ++;
		}
		dzhi0Info[i].num = dzhiItem;
		dzhi0Info[i].pBuff = (uint8_t *)rt_malloc(dzhi0Info[i].num);
		if(dzhi0Info[i].pBuff == NULL){
			rt_kprintf("��ֵ�ڴ��ȡʧ��");
			return;
		}
		dzhiItem = 0;
		for(j = 0; j < dzhiItemsAll; j++){//��ֵ��ʾ����
			if(dzhi0Info[i].pRoot[j].enable == 0 || dzhi0Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhi0Info[i].pBuff[dzhiItem ++] = j;
		}
	}
}

/**
  *@brief  ���ж�ֵ������ӳ��
  *@param  i ��ǰ���к� type ԭʼ����
  *@retval None
  */
static void Dzhi1TypeRemap(uint16_t i,uint16_t *type)
{
	switch(i)
	{
		case DZ1_INTERGHASE:*type = INTERGHASE;break;
		case DZ1_ZERO_SEQUE:*type = ZERO_SEQUE;break;
		case DZ1_LIMITATION:*type = LIMITATION;break;
		case DZ1_HEAVY_LOAD:*type = HEAVY_LOAD;break;
		case DZ1_OVERLOAD:*type = OVERLOAD;break;
		case DZ1_IBATTERY_SET:*type = BATTERY_SET;break;
		case DZ1_AUTO_RESET:*type = AUTO_RESET;break;
		case DZ1_LIMIT_V_F:*type = LIMIT_V_F;break;
		case DZ1_LOOP_CLOSE:*type = LOOP_CLOSE;break;
		case DZ1_FAULT_SWITCH:*type = FAULT_SWITCH;break;
		case DZ1_OTHER_PROTEC:*type = OTHER_PROTEC;break;
		case DZ1_LOGICAL_FUN:*type = LOGICAL_FUN;break;
		default:*type = INTERGHASE;break;
	}
}
/**
  *@brief  ������ʾ��ʼ��
  *@param  None
  *@retval None
  */
static void Dzhi1DisplayInit(void)
{
	uint16_t dzhiItemsAll,typeIs;
	uint8_t dzhiItem = 0;
	uint16_t i,j;
	dzhiItemsAll = g_FixedValueCfg1_Len;
	for(i = 0 ; i < DZ1_ALLNUM; i++){
		Dzhi1TypeRemap(i,&typeIs);
		dzhiItem = 0;
		dzhi1Info[i].SaveModify = Dzhi1ModfiySave;
		dzhi1Info[i].pRoot = g_pFixedValueCfg;
		for(j = 0; j < dzhiItemsAll; j++){//���ҿ��ö�ֵ
			if(dzhi1Info[i].pRoot[j].enable == 0 || dzhi1Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhiItem ++;
		}
		dzhi1Info[i].num = dzhiItem;
		dzhi1Info[i].pBuff = (uint8_t *)rt_malloc(dzhi1Info[i].num);
		if(dzhi1Info[i].pBuff == NULL){
			rt_kprintf("��ֵ�ڴ��ȡʧ��");
			return;
		}
		dzhiItem = 0;
		for(j = 0; j < dzhiItemsAll; j++){//��ֵ��ʾ����
			if(dzhi1Info[i].pRoot[j].enable == 0 || dzhi1Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhi1Info[i].pBuff[dzhiItem ++] = j;
		}
	}
}

/**
  *@brief  ���SOE����
  *@param  None
  *@retval 0 �޸��� 1�и���
  */
uint8_t CheckSoeUpdata(void)
{
	if(soeInfo.in != g_FlagDB.queue_soe.in){
		soeInfo.in = g_FlagDB.queue_soe.in;
		return 1;
	}
	return 0;
}

/**
  *@brief  ���CO����
  *@param  None
  *@retval 0 �޸��� 1�и���
  */
uint8_t CheckCoUpdata(void)
{
	if(coInfo.in != g_FlagDB.queue_co.in){
		coInfo.in = g_FlagDB.queue_co.in;
		return 1;
	}
	return 0;
}

/**
  *@brief  ��ȡSOE
  *@param  soeNo soe��
  *@retval 0 �ɹ� 1 ʧ��
  */
uint8_t GetSoeNoContent(uint16_t soeNo,SoeContent *pSoe)
{
	uint16_t pNo;
	uint16_t tms;
	GetSoeNum();
	if(soeNo >= soeInfo.num){
		return 1;
	}
	if(g_FlagDB.queue_soe.full == FULL){
		if(soeNo < g_FlagDB.queue_soe.in){
			pNo = g_FlagDB.queue_soe.in - soeNo - 1;
		}
		else{
			pNo = soeInfo.num - soeNo + g_FlagDB.queue_soe.in - 1;
		}
	}
	else{
		pNo = g_FlagDB.queue_soe.in - 1 - soeNo;
	}
	
	pSoe->time.year = g_SOEDB[pNo].time.year;
	pSoe->time.month = g_SOEDB[pNo].time.month;
	pSoe->time.day = g_SOEDB[pNo].time.dayofWeek & 0x1F;
	pSoe->time.hour = g_SOEDB[pNo].time.hour;
	pSoe->time.min = g_SOEDB[pNo].time.minute;
	tms = (g_SOEDB[pNo].time.msecondH << 8) + g_SOEDB[pNo].time.msecondL;
	pSoe->time.s = tms / 1000;
	pSoe->time.ms = tms % 1000;
	
	if(g_SOEDB[pNo].addr < 0x6000){
		if(g_SOEDB[pNo].addr <= g_TelesignalCfg_Len && g_SOEDB[pNo].addr > 0){	
			pSoe->pName = (uint8_t *)TelesignalCfg[g_SOEDB[pNo].addr - 1].pName;
			pSoe->pContent = (uint8_t *)TelesignalCfg[g_SOEDB[pNo].addr - 1].pContentSoe[g_SOEDB[pNo].value - 1];
		}
		if(g_SOEDB[pNo].value - 1){
			pSoe->highlight = 1;
		}
		else{
			pSoe->highlight = 0;
		}
	}
	else{
		return 1;
	}
	return 0;
}

/**
  *@brief  ��ȡCO
  *@param  soeNo co��
  *@retval 0 �ɹ� 1 ʧ��
  */
uint8_t GetCoNoContent(uint16_t coNo,SoeContent *pSoe)
{
	uint16_t pNo,tms;
	uint32_t tAddr;
	GetCoNum();
	if(coNo >= coInfo.num){
		return 1;
	}
	if(g_FlagDB.queue_co.full == FULL){//����ӳ���ַ
		if(coNo < g_FlagDB.queue_co.in){
			pNo = g_FlagDB.queue_co.in - coNo - 1;
		}
		else{
			pNo = coInfo.num - coNo + g_FlagDB.queue_co.in - 1;
		}
	}
	else{
		pNo = g_FlagDB.queue_co.in - 1 - coNo;
	}
	pSoe->highlight = 0;
	pSoe->time.year = g_CoDB[pNo].time.year;
	pSoe->time.month = g_CoDB[pNo].time.month;
	pSoe->time.day = g_CoDB[pNo].time.dayofWeek & 0x1F;
	pSoe->time.hour = g_CoDB[pNo].time.hour;
	pSoe->time.min = g_CoDB[pNo].time.minute;
	tms = (g_CoDB[pNo].time.msecondH << 8) + g_CoDB[pNo].time.msecondL;
	pSoe->time.s = tms / 1000;
	pSoe->time.ms = tms % 1000;
	tAddr = g_CoDB[pNo].addr - REMOTE_START_ADDR;
	if(tAddr <= g_TelecontrolCfg_Len){	
		pSoe->pName = (uint8_t *)TelecontrolCfg[tAddr].pName;
		uint8_t label = 255;
		if(g_CoDB[pNo].value >= 0x20){
			switch(g_CoDB[pNo].value){
				case OPEN_SELECT_FAIL:label = 0;break;
				case OPEN_SELECT_SUCCESS:label = 1;break;
				case OPEN_EXECUTE_FAIL:label = 2;break;
				case OPEN_EXECUTE_SUCCESS:label = 3;break;
				case CLOSE_SELECT_FAIL:label = 4;break;
				case CLOSE_SELECT_SUCCESS:label = 5;break;
				case CLOSE_EXECUTE_FAIL:label = 6;break;
				case CLOSE_EXECUTE_SUCCESS:label = 7;break;
				case DISTANT_REMOTE_CANCEL:label = 8;break;		
				case CLOSE_LOOP_SELECT_FAIL:label = 9;break;
				case CLOSE_LOOP_SELECT_SUCCESS:label = 10;break;
				case CLOSE_LOOP_EXECUTE_FAIL:label = 11;break;
				case CLOSE_LOOP_EXECUTE_SUCCESS:label = 12;break;
			}
		}
		else{
			label = g_CoDB[pNo].value - 1;
		}
		
		if(label > TelecontrolCfg[tAddr].dataType){
			pSoe->pContent = "��������";
		}
		else{
			pSoe->pContent = (uint8_t *)TelecontrolCfg[tAddr].pContentSoe[label];
		}
	}
	else{
		return 1;
	}
	return 0;
}

/**
  *@brief  ��ȡSOE����
  *@param  soeNo soe��
  *@retval soe����
  */
uint16_t GetSoeNum(void)
{
	if(g_FlagDB.queue_soe.full == FULL){
		soeInfo.num = SOE_MAX_NUM;
	}
	else{
		soeInfo.num = g_FlagDB.queue_soe.in;
	}
	return soeInfo.num;
}

/**
  *@brief  ��ȡCo����
  *@param  None
  *@retval ����
  */
uint16_t GetCoNum(void)
{
	if(g_FlagDB.queue_co.full == FULL){
		coInfo.num = CO_MAX_NUM;
	}
	else{
		coInfo.num = g_FlagDB.queue_co.in;
	}
	return coInfo.num;
}

/**
  *@brief  SOE/CO ��Ϣ��ʼ��
  *@param  None
  *@retval None
  */
void SoeCoInfoInit(void)
{
	soeInfo.in = 0;
	soeInfo.num = 0;
	soeInfo.CheckUpdata = CheckSoeUpdata;
	soeInfo.GetNoContent = GetSoeNoContent;
	soeInfo.GetNum = GetSoeNum;
	coInfo.in = 0;
	coInfo.num = 0;
	coInfo.CheckUpdata = CheckCoUpdata;
	coInfo.GetNoContent = GetCoNoContent;
	coInfo.GetNum = GetCoNum;
}
/**
  *@brief  �������¼�����
  *@param  None
  *@retval 0 �޸��� 1�и���
  */
uint8_t CheckFeventUpdata(void)
{
	if(fEventInfo.in != g_FlagDB.queue_fevent.in){
		fEventInfo.in = g_FlagDB.queue_fevent.in;
		return 1;
	}
	return 0;
}
/**
  *@brief  ��ȡ�����¼�
  *@param  feventNo �����¼���
  *@retval 0 �ɹ� 1 ʧ��
  */
uint8_t GetFeventNoContent(uint16_t feventNo,FeventContent *pEvent)
{
	struct SOE_Str *tSoe;
	uint16_t pNo,tAddr;
	uint16_t tms;
	GetFeventNum();
	if(feventNo >= fEventInfo.num){
		return 1;
	}
	if(g_FlagDB.queue_fevent.full == FULL){
		if(feventNo < g_FlagDB.queue_fevent.in){
			pNo = g_FlagDB.queue_fevent.in - feventNo - 1;
		}
		else{
			pNo = fEventInfo.num - feventNo + g_FlagDB.queue_fevent.in - 1;
		}
	}
	else{
		pNo = g_FlagDB.queue_fevent.in - 1 - feventNo;
	}
	tSoe = &g_FeventDB[pNo].yx[0];
	pEvent->time.year = tSoe->time.year;
	pEvent->time.month =  tSoe->time.month;
	pEvent->time.day =  tSoe->time.dayofWeek & 0x1F;
	pEvent->time.hour =  tSoe->time.hour;
	pEvent->time.min =  tSoe->time.minute;
	tms = ( tSoe->time.msecondH << 8) + tSoe->time.msecondL;
	pEvent->time.s = tms / 1000;
	pEvent->time.ms = tms % 1000;
	if(tSoe->addr < 0x6000){
		if(tSoe->addr <= g_TelesignalCfg_Len){
			pEvent->pName = (uint8_t *)TelesignalCfg[tSoe->addr - 1].pName;
			pEvent->pContent = (uint8_t *)TelesignalCfg[tSoe->addr - 1].pContentSoe[tSoe->value - 1];
		}
	}
	pEvent->yaoceNum = g_FeventDB[pNo].yc_num;
	if(pEvent->yaoceNum > 16){
		pEvent->yaoceNum = 16;
	} 
	for(uint8_t i = 0; i < pEvent->yaoceNum; i ++){
		tAddr = g_FeventDB[pNo].yc[i].addr - TELEMETRY_START_ADDR;
		if(tAddr < g_TelemetryCfg_Len){
			pEvent->pYaoceName[i] = (uint8_t *)TelemetryCfg[tAddr].pName;
			pEvent->pYaoceUnit[i] = (uint8_t *)TelemetryCfg[tAddr].pUnit;
			pEvent->pYaoceValue[i] = g_FeventDB[pNo].yc[i].value;
		}
	}
	return 0;
}

/**
  *@brief  ��ȡ�����¼�����
  *@param  None
  *@retval �����¼�����
  */
uint16_t GetFeventNum(void)
{
	if(g_FlagDB.queue_fevent.full == FULL){
		fEventInfo.num = FEVENT_MAX_NUM;
	}
	else{
		fEventInfo.num = g_FlagDB.queue_fevent.in;
	}
	return fEventInfo.num;
}

/**
  *@brief  �����·��ص�����
  *@param  cmdIs �����
  *@retval �����¼�����
  */
static void HmiCmdSendFun(uint8_t cmdIs)
{
	switch(cmdIs)
	{
		case 0:/* �����¼ */
            rt_multi_telecontrl_operate(ADDR_LOCAL_CLEAR, 0);		
			break;
		case 1:/* ��բ */
			if(g_TelesignalDB[g_TelesignalAddr.earth] == ON){
				rt_multi_telecontrl_operate(ADDR_LOCAL_OPERATE, DO_OPEN);
			}break;
		case 2:/* ��բ */
			if(g_TelesignalDB[g_TelesignalAddr.earth] == ON){
				rt_multi_telecontrl_operate(ADDR_LOCAL_OPERATE, DO_CLOSE);
			}break;
		case 3:/* ���� */
			rt_multi_telecontrl_operate(ADDR_LOCAL_RESET, 0);
		    break;
		default:break;
	}
}

/**
  *@brief  �����·���ʼ��
  *@param  None
  *@retval None
  */
static void HmiCmdSendInit(void)
{
	hcmdInfo.itemsNum = sizeof(hmiCmdItems)/sizeof(struct HmiCmdSend);
	hcmdInfo.pHmiCmd = hmiCmdItems;
	hcmdInfo.cmdfun = HmiCmdSendFun;
}

/**
  *@brief  �汾��Ϣ��ʼ��
  *@param  None
  *@retval None
  */
static void VersionInfoInit(void)
{
	versInfo.num = g_InherentParaCfg_Len;
	versInfo.pItems = InherentParaCfg;
}

/**
  *@brief  �Ƿ���³�����
  *@param  None
  *@retval 0 �Ǹ��� 1����
  */
uint8_t CheckUpdataProgram(void)
{
	if(rt_multi_common_read_update_state()){
		return 1;
	}
	else{
		return 0;
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
	Dzhi1DisplayInit();
	Dzhi0DisplayInit();
	HmiCmdSendInit();
	VersionInfoInit();
	SoeCoInfoInit();
}
