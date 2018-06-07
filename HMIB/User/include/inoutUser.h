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
#define USERLED_NUMALL 20	/* �û�ʹ��LED���� */
#define USERLED_RUN 1
#define USERLED_COMMUN 12

/* �û�����״̬ */
enum UserLedStatus{
	USERLED_OFF,
	USERLED_ON
};

#define USERSWITCH_NUMALL 8	/* �û�ʹ�ÿ������� */
struct SwitchQueue{
	#define SWITCHQUEUE_MAX 64
	uint8_t pIn;
	uint8_t pOut;
	uint8_t isfull;
	uint8_t number[SWITCHQUEUE_MAX];
	uint8_t state[SWITCHQUEUE_MAX];
};

#define USERKEY_NUMALL 13	/* �û�ʹ�ð������� */
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
uint8_t SwitchAllStateFill(void);
void SwitchQueueInit(void);
void KeyQueueInit(void);
void InOutUserMain(void);
/* ���ⲿ���� */
extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);

#endif /* END _INOUTUSER_H_ */

/* END */
