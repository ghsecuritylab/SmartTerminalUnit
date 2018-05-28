#ifndef __HMIBOOT_H
#define	__HMIBOOT_H

#include "stm32f4xx.h"

#define REVIAPMASTER_MAX		1024
#define SENDIAPMASTER_MAX		1024
#define FILEBUFFSIZE 				4096
/* ������SDRAM */
#define HMIPROGRAMSIZEADDR 	0xC00C0000
#define HMIPROGRAMSTARTADDR (HMIPROGRAMSIZEADDR + 4)

/* �Ĵ������� */
#define REGTABMUN 6
#define IAPREG_BEGINADDR 0x0001

enum IapRegAddrTab{
	IAPREG_CONNECT = 0x0001,/* �����������״̬ */
	IAPREG_FIRSTPOWER,/* ��һ���ϵ� */
	IAPREG_RUNSTATUS,/* ����״̬ */
	IAPREG_UPDATA,/* ��ʼ���� */
	IAPREG_GETPROGRAM,/* ��ȡ���� */
	IAPREG_IAPJUMP,
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
	uint8_t *name;
	uint32_t count;//����
	uint32_t outTime;//��ʱʱ��
	void (*overFun)(uint8_t source,uint8_t *name);
};

enum TimeCheckStatus{
	TC_DISENABLE,
	TC_ENABLE,
};
enum TimeOutNumber{
	TO_NONE,
	TO_SAVEFILEFALSE,
	TO_ERASEFALSE,
	TO_WRITEPROFLASE,
	TOHMIFINISH,
	TOJUMP,
	TOHMIBACK,
};

struct FileOperate{
	uint8_t number;//�ļ�����
	uint8_t flag;//����ʹ��
	uint8_t fileFlag;//�ļ�Ӧ��ʹ��
	uint32_t fileSize;//�ļ����ݴ�С
	uint8_t *const pbuff;//�ļ�����
	const uint16_t buffSize;//�����С
	uint16_t pBuffIn;
	uint16_t len;//���ͳ���
	uint32_t offset;//�ļ�ƫ��
};

void IapHmiInit(void);
void IapHmiScan(void);
uint8_t SaveHmiProgram(uint8_t *flag);
uint8_t HmiProgramErase(uint8_t *flag);
uint8_t WriteHmiProgram(uint8_t *flag);
uint8_t HmiProgramUpadta(uint8_t *flag);
uint8_t HmiProgramBack(uint8_t *flag);
void IapOutTimCheck(void);
uint8_t GetprogramTofile(void);
void HmiProgramJump(void);
void testCreatFile(void);
#endif /* __HMIBOOT_H */

/* END */
