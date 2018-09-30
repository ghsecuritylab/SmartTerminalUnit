#ifndef _QUICK_BREAK_PROTECT_H
#define _QUICK_BREAK_PROTECT_H

#include "drv_do.h"
#include "common_data.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "breaker_protect.h"
#include "breaker_interface.h"
#include "distribution.h"


#define RESETFLAG  0x80000000 
//�ٶϱ���
#define OVERCUR1 0x00000001			//�˱�־λ����������ʱ��λ�����������߼����ж���λ�������ܽ��뱣���߼�
#define OVERCURSTA1  0x00010000    	//��������
#define OVERCURSTA2  0x00020000    	//�������·�բ
typedef struct
{
	struct
    {
        uint32_t *overcurI0flag;
        uint32_t flag;
        uint32_t *gTime;                 //��ʱ�� 
    }valstr;							
    struct
    {
        float *pSwitch;        			//����һ��Ͷ�� ��1Ͷ�롱����0�˳���    
        float *pValue;                  //����һ�ε��� ����ֵ 0.00A~99.99A
        float *pTime;              		//����һ��ʱ�� ʱ�䶨ֵ��������Χ��0.00S~99.99S
    }parastr;							//�趨�����ṹ��
	
}CurQuickBreakSts;

//�����������
#define OVERCURI01 0x00000001
#define OVERCURI02 0x00000002
#define OVERCURI0STA1  0x00010000          //��բλ�÷�������
#define OVERCURI0STA2  0x00020000    	   //��բλ������λ
typedef struct
{
    struct
    {
        uint32_t *overcurflag;
        uint32_t flag;
        uint32_t *gTime[2];                //��ʱ�� 
    }valstr;
    struct
    {
        float *pSwitch[2];        			//����һ��Ͷ�� ��1Ͷ�롱����0�˳���    
        float *pValue[2];                   //����һ�ε��� ����ֵ 0.00A~99.99A
        float *pTime[2];              		//����һ��ʱ�� ʱ�䶨ֵ��������Χ��0.00S~99.99S
    }parastr;
}CurQuickBreakI0Sts;

void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void CurrentValue(CurQuickBreakSts *curquickbreakSts, float Ki, float Ikmax);
void CurQuickBreak_ctrl(ComProSts *comProSts,CurQuickBreakSts *curquickbreakSts);
void CurQuickBreakI0_ctrl(ComProSts *comProSts,CurQuickBreakI0Sts *curquickbreakSts);
void CurProtectCtrlClock(void);

#endif /*_QUICK_BREAK_PROTECT_H*/

