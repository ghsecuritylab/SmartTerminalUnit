/**
  * @file    inOutUser.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/17
  * @brief   
  */
  
#ifndef _INOUTUSER_H_
#define _INOUTUSER_H_

#include "stdint.h"

/* -- ����ӳ�䶨�� -- */

/* -- ����ӳ�䶨�� -- */

/* -- LEDӳ�䶨�� -- */
#define USERLED_NUMALL 10	/* �û�ʹ��LED���� */
#define USERLED_NO0 0			/* LEDӳ�� 0~n Ϊʵ��led���� */
#define USERLED_NO1 1
#define USERLED_NO2 2
#define USERLED_NO3 3
#define USERLED_NO4 4
#define USERLED_NO5 5
#define USERLED_NO6 6
#define USERLED_NO7 7
#define USERLED_NO8 8
#define USERLED_NO9 9
#define USERLED_NO10 10
#define USERLED_NO11 11
#define USERLED_NO12 12
#define USERLED_NO13 13
#define USERLED_NO14 14
#define USERLED_NO15 15
/* �û�����״̬ */
enum UserLedStatus{
	USERLED_OFF,
	USERLED_ON
};

#define USERSWITCH_NUMALL 6	/* �û�ʹ�ÿ������� */
struct SwitchQueue{
	#define SWITCHQUEUE_MAX 64
	uint8_t pIn;
	uint8_t pOut;
	uint8_t isfull;
	uint8_t number[SWITCHQUEUE_MAX];
	uint8_t state[SWITCHQUEUE_MAX];
};

#define USERKEY_NUMALL 10	/* �û�ʹ�ð������� */
struct KeyQueue{
	#define KEYQUEUE_MAX 64
	uint8_t pIn;
	uint8_t pOut;
	uint8_t isfull;
	uint8_t number[KEYQUEUE_MAX];
	uint8_t state[KEYQUEUE_MAX];
};

void SetUserLedStatus(uint8_t ledNo, uint8_t state);
void ContinuousOutResult(uint8_t beginNo,uint8_t num,uint8_t *pBuff);
struct KeyQueue *GetkeyQueueP(void);
void UserKeyChangeDeal(uint8_t keyNum, uint8_t state);
struct SwitchQueue *GetswitchQueueP(void);
void UserSwitchChangeDeal(uint8_t switchNum, uint8_t state);

#endif /* END _INOUTUSER_H_ */

/* END */
