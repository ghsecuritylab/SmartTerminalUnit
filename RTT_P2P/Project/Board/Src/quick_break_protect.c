/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      quick_break_protest.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    tianxiaoliang
  * @date:      2018.08.03
  * @update:    [YYYY-MM-DD][����������][�������]
  */

#include "quick_break_protect.h"
#include "extern_interface.h"
#include "drv_timer.h"
#include "output.h"
#include "input.h"


extern volatile rt_uint8_t rt_interrupt_nest;
//��board.c�ļ���rt_hw_board_init()���潫����Ϊ1
uint8_t OSRunning=0;

#define delay_osrunning		  OSRunning			       //OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	RT_TICK_PER_SECOND	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	rt_interrupt_nest		//�ж�Ƕ�׼���,���ж�Ƕ�״���

static ComProSts g_ComProSts[BRE_DEVMAXNUM];
static CurQuickBreakSts g_Overcur[BRE_DEVMAXNUM];
static float  g_num;
/**
  * @Description: �����ٶ�
  * @param:  Ki-�������������
  * @return: Ikmax-��·ĩ������·����
  * @updata: [YYYY-MM-DD] [����������][�������]
  */
//void CurrentValue(CurQuickBreakSts *curquickbreakSts, float Ki, float Ikmax)
//{
//	float Krel = 1.3;             //����װ�õĿɿ�ϵ��
//	float Kw = 1;		      //����ϵ��		
//						  
//	*(curquickbreakSts->parastr.pValue) = ((Krel * Kw * Ikmax) / (2 * Ki));		//����1�ζ�������
//}
/**
  * @Description: �����ٶ�
  * @param:  
  * @return: 
  * @updata: [YYYY-MM-DD] [����������][�������]
  */
void CurQuickBreak_ctrl(ComProSts *comProSts,CurQuickBreakSts *curQuickBreakSts)
{										
	if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//��λ
	{
		if(curQuickBreakSts->valstr.flag&OVERCURSTA2)
		{
			curQuickBreakSts->valstr.flag = 0;
			*(curQuickBreakSts->valstr.gTime) = 0;					//1�ζ�ʱ������
		}
		*(curQuickBreakSts->parastr.pValue) = 0.3;
		
//		rt_kprintf("value             =              %d\r\n",    (int32_t)*(curQuickBreakSts->parastr.pValue));

		if((*(comProSts->yc.Ia)>*(curQuickBreakSts->parastr.pValue))||(*(comProSts->yc.Ib)>*(curQuickBreakSts->parastr.pValue)))	    //������
		{
//			if(!(*(curQuickBreakSts->valstr.gTime)&MAINPRO_ENTIMERS))
//            {
//                *(curQuickBreakSts->valstr.gTime) = MAINPRO_ENTIMERS;
//            }
//			if((*(curQuickBreakSts->valstr.gTime)&MAINPRO_ENTIMERS)>=(uint32_t)(*(curQuickBreakSts->parastr.pTime)*1000))
//			{
				curQuickBreakSts->valstr.flag |= OVERCUR1|OVERCURSTA1;
//				rt_hw_output_operate(ADDR_LOGIC_ACT,DO_OPEN);					//��բ
				OpeningclosingOperate(TMR_50MS_OPEN);
                curQuickBreakSts->valstr.flag |= RESETFLAG;
//			}
		}
		else
		{
			*(curQuickBreakSts->valstr.gTime) = 0;
		}
	}
	else if(curQuickBreakSts->valstr.flag&OVERCURSTA1)
	{
		curQuickBreakSts->valstr.flag &= (~OVERCURSTA1);
		curQuickBreakSts->valstr.flag |= OVERCURSTA2;
	}
}

/**
  * @Description: �������
  * @param:  
  * @return: 
  * @updata: [YYYY-MM-DD] [����������][�������]
  */

void CurQuickBreakI0_ctrl(ComProSts *comProSts,CurQuickBreakI0Sts *curquickbreakSts)
{
	
}



void CurProtectCtrlClock(void)
{
	uint8_t pdrv = 0;
	
	g_ComProSts[pdrv].yc.Ia = &g_TelemetryDB[g_TelemetryAddr.Ia];
	g_ComProSts[pdrv].yc.Ib = &g_TelemetryDB[g_TelemetryAddr.Ib];
	g_ComProSts[pdrv].yc.Ic = &g_TelemetryDB[g_TelemetryAddr.Ic];
	
	g_Overcur[pdrv].parastr.pValue = &g_num;
//	g_Overcur[pdrv].parastr.pTime = &g_FixedValueP[OVER_CURRENT_TIME1];
	g_ComProSts[pdrv].yx.switchOpen.value = &g_TelesignalDB[g_TelesignalAddr.switchOpen];
    g_ComProSts[pdrv].yx.switchClose.value = &g_TelesignalDB[g_TelesignalAddr.switchClose];
	
	CurQuickBreak_ctrl(&g_ComProSts[pdrv], &g_Overcur[pdrv]);	
}
static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;



void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
	fac_us=SYSCLK;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
						//�����Ҫ֧��OS.
	reload=SYSCLK;						//ÿ���ӵļ������� ��λΪM	   
	reload*=1000000/delay_ostickspersec;	//����delay_ostickspersec�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����	
	fac_ms=1000/delay_ostickspersec;		//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//����SYSTICK      
}	

void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt = 0;
	uint32_t reload = SysTick->LOAD;

	ticks = nus * fac_us;
	rt_enter_critical();
	told = SysTick->VAL;
	while (1)
	{
		tnow = SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
			{
				tcnt+=told-tnow;
			}
			else
			{
				tcnt+=reload-tnow+told;
			}
			told = tnow;
			if(tcnt>=ticks)
			{
				break;
			}
		}
	}
	rt_exit_critical();
}

void delay_ostimedly(uint32_t ticks)
{
	rt_thread_delay(ticks);
}

void delay_ms(uint16_t nms)
{	
	if(delay_osrunning && delay_osintnesting==0)//���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)	    
	{		 
		if(nms>=fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			delay_ostimedly(nms/fac_ms);	//OS��ʱ
		}
		nms%=fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((uint32_t)(nms*1000));				//��ͨ��ʽ��ʱ
}
