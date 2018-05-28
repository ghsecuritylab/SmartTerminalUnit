#ifndef __IAPCONTROL_H
#define	__IAPCONTROL_H

#include "stm32f4xx.h"
#include "stdio.h"

#define NOFIRSTPOWER 		0x55
#define HMIPRO_RUNTURE	0x08
#define HMIPRO_UPDATA		0x01
#define HMIPRO_SAVE			0x02

#define MAINPRO_RUNTURE	HMIPRO_RUNTURE
#define MAINPRO_UPDATA	HMIPRO_UPDATA
#define MAINPRO_SAVE		HMIPRO_SAVE


#define IS_FIRSTRUN_FLAG					0x55
#define IS_WILLUPDATA_FLAG				0x01
#define IS_UPDATAFINISH_FLAG			0x02
#define IS_UPDATAFAULT_FLAG				0x04
#define IS_RUNOK_FLAG							0x08
enum IapControlStep{
	CON_INIT,
	CON_HMIPRO_RUNOK,
	CON_HMIPRO_UPDATA,
	CON_HMIPRO_SAVE,
	CON_HMIPRO_BACK,
	CON_HMIPRO_FINISH,
	CON_HMIPRO_UPDATAPROERROE,
	CON_MAINPRO_RUNOK,
	CON_MAINPRO_UPDATA,
	CON_MAINPRO_SAVE,
	CON_MAINPRO_BACK,
	CON_MAINPRO_FINISH,
	CON_MAINPRO_UPDATAPROERROE,
	CON_TOJUMP,
};
struct ProgramRunState{
	uint8_t firstPower;	/* ��һ���ϵ� */
	uint8_t mBoradState;/* ����״̬ */
	uint8_t hBoradState;/* ���״̬ */
	uint8_t bootCount;	/* �������� */
};

void IapControlInit(void);
void IapControlMain(void);
void SetControlStep(enum IapControlStep step);
void FatfsInit(void);
void FatfsUninstall(void);
#endif /* __IAPCONTROL_H */

/* END */
