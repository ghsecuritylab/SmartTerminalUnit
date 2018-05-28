/**
  * @file   hmiInout.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/18
  * @brief  hmi�������صȴ���
  */
#include "hmiInOut.h"
#include "common_data.h"
#include "drv_do.h"
#include "gui_101_cmd.h"

const uint8_t LedBitLook[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
/* ��״̬ */
uint8_t Ledstate[ULED_ALLNUM / 8 + 1] = {0};
/* ң�����ӳ�� */
uint8_t *RealYxLed[ULED_ALLNUM],LastYxLed[ULED_ALLNUM];
/* �˵����� */
struct MenuKeyValue menuKey;
/* ң�ذ���ֵ */
struct YKKeyValue ykKeyValue;
/**
  *@brief  �˵���������
  *@param  keyNo ������
  *@param  state ֵ
  *@retval None
  */
void MenuKeyResult(uint8_t keyNo, uint8_t state)
{
	if(state == USERKEY_DOWN){
		switch(keyNo){
			case MENUKEY_UP:
				menuKey.up = 1;menuKey.keyIs = UpKey;break;
			case MENUKEY_DOWN:
				menuKey.down = 1;menuKey.keyIs = DownKey;break;
			case MENUKEY_LEFT:
				menuKey.left = 1;menuKey.keyIs = LeftKey;break;
			case MENUKEY_RIGHT:
				menuKey.right = 1;menuKey.keyIs = RightKey;break;
			case MENUKEY_ESC:
				menuKey.esc = 1;menuKey.keyIs = CancelKey;break;
			case MENUKEY_OK:
				menuKey.ok = 1;menuKey.keyIs = OkKey;break;
			default:break;
		}
	}
	else{
		switch(keyNo){
			case MENUKEY_UP:
				menuKey.up = 0;break;
			case MENUKEY_DOWN:
				menuKey.down = 0;break;
			case MENUKEY_LEFT:
				menuKey.left = 0;;break;
			case MENUKEY_RIGHT:
				menuKey.right = 0;break;
			case MENUKEY_ESC:
				menuKey.esc = 0;break;
			case MENUKEY_OK:
				menuKey.ok = 0;break;
			default:break;
		}
	}
}

/**
  *@brief  ��ȡ�˵���ǰ����ֵ
  *@param  None
  *@retval None
  */
enum KeyStatus GetKeyStatus(void)
{
	return menuKey.keyIs;
}

/**
  *@brief  ����˵�����
  *@param  None
  *@retval None
  */
void SetKeyIsNoKey(void)
{
	menuKey.keyIs = NoKey;
}
/**
  *@brief  ң�ذ��������
  *@param  keyNo ������
  *@retval None
  */
void YaoKongKeyCmdResult(enum UserKeyNomberMap keyNo)
{
	if(keyNo == YK_OPENSWITCH){
		if(ykKeyValue.shift == 1){
			ykKeyValue.shift = 0;
			if(g_TelesignalDB[ADDR_REMOTE_EARTH] != ON){//��բ
				rt_hw_do_operate(ADDR_LOCAL_OPERATE, DO_OPEN);
			}
		}
	}
	else if(keyNo == YK_CLOSESWITCH){//��բ
		if(ykKeyValue.shift == 1){
			ykKeyValue.shift = 0;
			if(g_TelesignalDB[ADDR_REMOTE_EARTH] != ON){
				rt_hw_do_operate(ADDR_LOCAL_OPERATE, DO_CLOSE);
			}
		}
	}
	else if(keyNo == YK_RESET){//����
		rt_multi_telecontrl_operate(ADDR_LOCAL_RESET, 0);
	}
}
/**
  *@brief  ң�ذ�������
  *@param  keyNo ������
  *@param  state ״̬
  *@retval None
  */
void YaoKongKeyResult(uint8_t keyNo, uint8_t state)
{
	if(state == USERKEY_DOWN){
		switch(keyNo){
			case YK_OPENSWITCH:
			case YK_CLOSESWITCH:
			case YK_RESET:
				YaoKongKeyCmdResult((enum UserKeyNomberMap)keyNo);break;
			case YK_SHFITSWITCH:
				ykKeyValue.shift = 1;break;
			default:break;
		}
	}
	else{
		switch(keyNo){
			case YK_OPENSWITCH:
				ykKeyValue.open = 0;break;
			case YK_CLOSESWITCH:
				ykKeyValue.close = 0;break;
			case YK_SHFITSWITCH:
				ykKeyValue.shift = 0;break;
			case YK_RESET:
				ykKeyValue.reset = 0;break;
			default:break;
		}
	}
}
/**
  *@brief  ���������
  *@param  None
  *@retval None
  */
void SwitchResult(uint8_t switchNo, uint8_t state)
{
	if(state == 1){//0Ϊ��Ч״̬
		switch(switchNo){
			/* ����Ϊһ���� ͳһ������Ч״̬���� */
//			case SW_LOCAL:DBWriteSOE(ADDR_REMOTE_EARTH, OFF);break;
//			case SW_REMORE:DBWriteSOE(ADDR_REMOTE_EARTH, ON);break;
			case SW_RECLOSE:DBWriteSOE(ADDR_RECLOSE_FA_STRAP, OFF);break;
			case SW_PROTECT:DBWriteSOE(ADDR_FUNCTION_HARDSTRAP, OFF);break;
			/* ����Ϊһ���� ͳһ������Ч״̬���� */
//			case SW_CONTACT:DBWriteSOE(ADDR_BREAK_CONTACT, ON);break;
//			case SW_SECTION:DBWriteSOE(ADDR_BREAK_CONTACT, OFF);break;
		}
	}
	else{
		switch(switchNo){
			case SW_LOCAL:DBWriteSOE(ADDR_REMOTE_EARTH, ON);break;
			case SW_REMORE:DBWriteSOE(ADDR_REMOTE_EARTH, OFF);break;
			case SW_RECLOSE:DBWriteSOE(ADDR_RECLOSE_FA_STRAP, ON);break;
			case SW_PROTECT:DBWriteSOE(ADDR_FUNCTION_HARDSTRAP, ON);break;
			case SW_CONTACT:DBWriteSOE(ADDR_BREAK_CONTACT, ON);break;
			case SW_SECTION:DBWriteSOE(ADDR_BREAK_CONTACT, OFF);break;
		}
	}
}
/**
  *@brief  ���������
  *@param  None
  *@retval None
  */
void KeyCmdResult(uint8_t keyNo, uint8_t state)
{
	switch(keyNo){
		case MENUKEY_UP:
		case MENUKEY_DOWN:
		case MENUKEY_LEFT:
		case MENUKEY_RIGHT:
		case MENUKEY_ESC:
		case MENUKEY_OK:MenuKeyResult(keyNo,state);break;
		case YK_OPENSWITCH:
		case YK_CLOSESWITCH:
		case YK_SHFITSWITCH:
		case YK_RESET:YaoKongKeyResult(keyNo,state);break;
		case SW_LOCAL:
		case SW_REMORE:
		case SW_RECLOSE:
		case SW_PROTECT:
		case SW_CONTACT:
		case SW_SECTION:SwitchResult(keyNo,state);break;
	}
}

/**
  *@brief  ����Led״̬
  *@param  ledNo led��
  *@param  state ״̬
  *@retval 0 �ɹ� 1ʧ��
  */
uint8_t ULedStateSet(uint8_t ledNo,enum UserLedState state)
{
	if(ledNo >= ULED_ALLNUM){
		return 1;
	}
	if(state == ULED_ON){
		Ledstate[ledNo / 8] |= LedBitLook[ledNo % 8]; 
	}
	else{
		Ledstate[ledNo / 8] &= ~LedBitLook[ledNo % 8]; 
	}
	return 0;
}
/**
  *@brief  Led״̬����
  *@param  None
  *@retval None
  */
void ULedStateSend(void)
{
	uint8_t i,tNum;
	uint8_t tBuff[ULED_ALLNUM / 8 + 2];
	tBuff[0] = LED_BEGIN_NO;
	tNum = ULED_ALLNUM / 8 + (ULED_ALLNUM % 8 ? 1:0);
	for(i = 0; i < tNum; i ++){
		tBuff[i + 1] = Ledstate[i];
	}
	HmiCmd001Fill(1, ULED_ALLNUM,tBuff);
}

/**
  *@brief  Led״̬�仯���
  *@param  None
  *@retval None
  */
void LedChangeCheck(void)
{
	uint8_t sendFlag = 0;
	for(uint8_t i = 0; i < ULED_ALLNUM; i++){
		if(LastYxLed[i] != *(RealYxLed[i])){
			LastYxLed[i] = *RealYxLed[i];
			if(LastYxLed[i] == OFF){
				if(i == ULED_NOENERGY){
					ULedStateSet(i,ULED_ON);
				}
				else{
					ULedStateSet(i,ULED_OFF);
				}
			}
			else{
				if(i == ULED_NOENERGY){
					ULedStateSet(i,ULED_OFF);
				}
				else{
					ULedStateSet(i,ULED_ON);
				}
			}
			sendFlag = 1;
		}
	}
	if(sendFlag == 1){
		if(LastYxLed[ULED_NOENERGY] == OFF){
			ULedStateSet(ULED_NOENERGY,ULED_ON);
		}
		else{
			ULedStateSet(ULED_NOENERGY,ULED_OFF);
		}
		ULedStateSend();
	}
}

/**
  *@brief  led�͵�ӳ��
  *@param  None
  *@retval None
  */
void YaoxinMapToLed(void)
{
	RealYxLed[ULED_COMMUN] = &g_TelesignalDB[ADDR_COMMUNICATION];
	RealYxLed[ULED_NOENERGY] = &g_TelesignalDB[ADDR_OPERATING_MECHANISM];
	RealYxLed[ULED_SWITCHOPEN] = &g_TelesignalDB[ADDR_OPEN];
	RealYxLed[ULED_LowPa] = &g_TelesignalDB[ADDR_LOW_PRESSURE];
	RealYxLed[ULED_SWITCHCLOSE] = &g_TelesignalDB[ADDR_CLOSE];
	RealYxLed[ULED_CLOSELOCK] = &g_TelesignalDB[ADDR_CLOSING_LOCK];
	RealYxLed[ULED_OVERALARM] = &g_TelesignalDB[ADDR_SHORT_CIRCUIT_FAULT];
	RealYxLed[ULED_OPENLOCK] = &g_TelesignalDB[ADDR_OPENING_LOCK];
	RealYxLed[ULED_ZEROALARM] = &g_TelesignalDB[ADDR_EARTHING_FAULT];
	RealYxLed[ULED_DEVICEFAULT] = &g_TelesignalDB[ADDR_DEVICE_FAULT];
	RealYxLed[ULED_SELFCHECK] = &g_TelesignalDB[ADDR_SELF_CHECK_ABNOMAL];
	RealYxLed[ULED_RECLOSELOCK] = &g_TelesignalDB[ADDR_RECLOSE_LOCK];
	
	for(uint8_t i = 0; i < ULED_ALLNUM; i++){
		LastYxLed[i] = *(RealYxLed[i]);
		if(LastYxLed[i] == OFF){
			if(i == ULED_NOENERGY){//ȡ��
				ULedStateSet(i,ULED_ON);
			}
			else{
				ULedStateSet(i,ULED_OFF);
			}
		}
		else{
			if(i == ULED_NOENERGY){//ȡ��
				ULedStateSet(i,ULED_OFF);
			}
			else{
				ULedStateSet(i,ULED_ON);
			}
		}
	}
	ULedStateSend();
}

/**
  *@brief  HmiOut��ʼ��
  *@param  None
  *@retval None
  */
void HmiInOutInit(void)
{
	YaoxinMapToLed();
}
/* END */
