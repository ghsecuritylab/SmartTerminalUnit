
#ifndef __GWPROTOCAL_H
#define	__GWPROTOCAL_H

#include "stm32f4xx.h"

/* ���һ�ν��������� */
#define RECEIVE_MAXBYTE	1024
/* ���һ�η��������� */
#define SEND_MAXBYTE	1024
/* ����λ */
#define ISMASTERBIT	0x01
/* �û���λ */
#define USERNUMBER	0xFE
/* ����û��� */
#define MAXUSER_NUM 4
/* ���Ͷ��д�С */
#define USERSEND_MAXNUM 8


typedef enum FrameOffset_{
	FRAME_BEGIN,		
	USERID,
	FRAME_NUMBER,
	DATALEN_L,
	DATALEN_H,
}FrameOffset;


typedef enum ReturnStatus_{
	ISTURE,		/* ʧ�� */
	ISFAILURE,	/* �ɹ� */
}ReturnStatus;

enum ReceiveFlag{
	CANRECEIVE,
	REVISBUSYING,
};
struct ReceiveControl{
	enum ReceiveFlag  flag;			/* �Ƿ���Խ��ձ�־ */
	uint16_t outTim;				/* ������ɼ�� */
	uint16_t pInCount;				/* ���ռ��� */
	uint8_t buff[RECEIVE_MAXBYTE];	/* ���ջ����� */
};

typedef enum UserCurrentStatus{
	IDLE,
	PRIVATE,
	SENDING,
	SENDFALSE,
	WAITACK,
	OUTTIME,
	ISACK,
	ACKFALSE,
	ISMASTERCALL,
}UserStatus;
typedef struct UserInfo_{
	uint8_t ID;			/* �û�ID */
	uint8_t number;		/* ֡��ˮ�� */
	UserStatus status;	/* ��ǰ�û�״̬ */
	uint8_t outTimeFlag;
	uint32_t outTime;	/* ��ʱʱ�� */
	uint32_t timConut;	/* ��ʱʱ���ʱ */
	uint16_t revBuffMax;
	uint8_t *pRevBuff;
	void (*slaveFun)(void);
}UserInfo;

typedef struct UserTab_{
	UserInfo *user[MAXUSER_NUM];
	uint8_t num;
}UserTab;

typedef struct UserSendInfo_{
	UserInfo *user;
	uint8_t *pSendBuff;
}UserSendInfo;
typedef struct UserSendQueue_{
	UserSendInfo user[USERSEND_MAXNUM];
	uint8_t pIn;
	uint8_t pOut;
}UserSendQueue;

typedef union Uint8To16_{
	uint16_t Uint16;
	uint8_t Uint8[2];
}Uint8To16;

typedef union Uint8To32_{
	uint32_t Uint32;
	uint8_t Uint8[4];
}Uint8To32;

extern void Protocalmain(void);
extern void ProtocalInit(void);
/* �ײ�������ݽӿ� */
void ReceiveDealWith(uint8_t data);
/* �ײ��������һ֡��� */
void ReceiveFinish(void);
/* ����û� */
ReturnStatus AddUserInfo(UserInfo *info);
/* ������� */
ReturnStatus UserDataSendFill(UserInfo *sendInfo,uint8_t *pSendBuff);

extern void SendData(uint8_t *pBuff, uint16_t length);
#endif /* __GWPROTOCAL_H */

/* END */
