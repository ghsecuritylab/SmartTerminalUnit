/**
  ******************************************************************************
  * @file    systemconfig.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   M3��Systick��ʱ��������Ϊ�����ļ� ����
  ******************************************************************************
  */
#include "systemconfig.h"

/* 1usʱ��Ľ����� */
static uint32_t Fac_us;	
/* LSIƵ�� */
__IO uint32_t LsiFreq = 40000; 

static uint32_t timeCount;
/**
  *@brief ϵͳ���ȼ�����
  *@param  None
  *@retval None
  */
void NVIC_Configuration(void)
{  
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the USARTy Interrupt */		
}

void SysTick_Init(int16_t clock_div)
{
	/* SystemCoreClock / 1000   1ms */
   if (SysTick_Config(SystemCoreClock / clock_div)){ 
		/* Capture error */ 
		while (1);
	}
    Fac_us = SystemCoreClock/1000000;/* 1usʱ��Ľ�����,������ʱ���� */
	OPen_SysTick();
}

/**
  *@brief ��ȷ΢����ʱ
  *@param  nus  Ҫ��ʱ��΢����
  *@retval None
  */
void Delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				/* LOAD��ֵ	 */    	 
	ticks=nus*Fac_us; 						/* ��Ҫ�Ľ�����	  */ 		 
	tcnt=0;
	told=SysTick->VAL;        				/* �ս���ʱ�ļ�����ֵ */
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)
				tcnt+=told-tnow;			/* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����. */
			else 
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;						/* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�. */
		}  
	}							    
}

/**
  *@brief ��ȷms��ʱ
  *@param  nms Ҫ��ʱ�ĺ�����
  *@retval None
  */
void Delay_ms(uint16_t nms)
{	    		  
	Delay_us((uint32_t)(nms*1000));
}

void SystemconfigInit(void)
{
	
	NVIC_Configuration();	
	SysTick_Init((int16_t)1000);/* 1ms */
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	timeCount ++;
}

/**
  *@brief ��ȡ��ʼ��ʱ��
  *@param  None
  *@retval beginTick  ��ʱ��ʼ����
  */
uint32_t GetTimer1Tick(void)
{
	return timeCount;
}

/**
  *@brief ��ȡ���������
  *@param  beginTick  ��ʱ��ʼ������GetTimer1Tick()�������
  *@retval ���������
  */
uint32_t GetTimer1IntervalTick(uint32_t beginTick)
{
	uint32_t temptick;
	temptick = GetTimer1Tick();
	if(beginTick <= temptick){
		return (temptick - beginTick);
	}
	else{
		return ((0xFFFFFFFF - beginTick)+temptick);
	}
}
