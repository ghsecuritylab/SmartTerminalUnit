/**
  * @file   hmiInout.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/18
  * @brief  hmi�������صȴ���
  */

#ifndef __HMIINOUT_H
#define	__HMIINOUT_H

#include "stdint.h"

/* ����״̬ */
#define USERKEY_DOWN	1
#define USERKEY_UP 		0

/* ����ӳ�� */
enum UserKeyNomberMap{
	/* �˵�����ӳ�� */
	MENUKEY_UP = 0,
	MENUKEY_LEFT = 1,
	MENUKEY_DOWN = 2,
	MENUKEY_RIGHT = 3,
	MENUKEY_ESC = 4,
	MENUKEY_OK = 5,
	/* ң�ذ���ӳ�� */
	YK_RESET = 6,
	YK_CLOSESWITCH = 7,
	YK_SHFITSWITCH = 8,
	YK_OPENSWITCH = 9,
	/* ����ӳ�� */
	SW_LOCAL = 50,
	SW_REMORE = 51,
	SW_RECLOSE = 52,
	SW_PROTECT = 53,
	SW_CONTACT = 54,
	SW_SECTION = 55,
};
/* ����ö�� */
enum KeyStatus{
	NoKey,
	UpKey,
	DownKey,
	LeftKey,
	RightKey,
	OkKey,
	CancelKey
};
/* �˵�����״̬ */
struct MenuKeyValue{
	uint8_t up:1;
	uint8_t down:1;
	uint8_t left:1;
	uint8_t right:1;
	uint8_t ok:1;
	uint8_t esc:1;
	enum KeyStatus keyIs;
};
/* ң�ذ���״̬ */
struct YKKeyValue{
	uint8_t open:1;
	uint8_t close:1;
	uint8_t shift:1;
	uint8_t reset:1;
	enum KeyStatus keyIs;
};

/* ����ӳ�� */
enum UserOutputNoMap{
	/* LED ӳ�� */
	LED_BEGIN_NO = 2,
	LED_END_NO = 50,
};

#define ULED_ALLNUM 9
enum UserLedDefine{
	ULED_0,
	ULED_1,
	ULED_2,
	ULED_3,
	ULED_4,
};
enum UserLedState{
	ULED_OFF,
	ULED_ON,
};

enum KeyStatus GetKeyStatus(void);
void SetKeyIsNoKey(void);
void KeyCmdResult(uint8_t keyNo, uint8_t state);
void LedChangeCheck(void);

extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
#endif /* __HMIINOUT_H */

/* END */
