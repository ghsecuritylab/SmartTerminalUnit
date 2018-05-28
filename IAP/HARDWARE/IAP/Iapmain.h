
#ifndef __IAPMAIN_H
#define	__IAPMAIN_H

#include "stm32f4xx.h"

#define REVIAPSLAVE_MAX		1024
#define SENDIAPSLAVE_MAX	1024

#define REGTABMUN 5

#define IAPREG_BEGINADDR 0x0001
enum IapRegAddrTab{
	IAPREG_CONNECT = 0x0001,/* �����������״̬ */
	IAPREG_FIRSTPOWER,/* ��һ���ϵ� */
	IAPREG_RUNSTATUS,/* ����״̬ */
	IAPREG_UPDATA,/* ��ʼ���� */
	IAPREG_GETPROGRAM,/* ��ȡ���� */
};
struct RegisterTab{
	const uint16_t regAddr;
	const uint8_t type;
	uint8_t *value;
};

typedef struct Type1Reg_{
	const uint16_t addr;
	uint8_t value;
}Type1Reg;

struct IapRegInfo{
	Type1Reg connect;/* �����������״̬ */
	Type1Reg firstPower;/* ��һ���ϵ� */
	Type1Reg runStatus;	/* ����״̬ */
	Type1Reg updata;/* ��ʼ���� */
	Type1Reg GetProgram;/* ��ȡ���� */
	Type1Reg iapJump;	/* ��ת���� */
};

struct IapTimeOutCheck{
	uint8_t flag;//�Ƿ���г�ʱ��� 0 ������ 1 ����
	uint8_t source;//��ʱԴ
	uint32_t count;//����
	uint32_t outTime;//��ʱʱ��
	void (*overFun)(uint8_t source);
};

enum IapStepEnum{
	IAP_INIT,
	IAP_WAITCONNECT,
	IAP_CONNECTFINISH,
	IAP_WAITCONTROL,
};

struct FileOperate{
	uint8_t number;
	uint8_t flag;
	uint8_t *pbuff;
	uint16_t buffSize;
	uint16_t offset;
};

void IapmainInit(void);
void IapMianScan(void);

#endif /* __IAPMAIN_H */

/* END */
