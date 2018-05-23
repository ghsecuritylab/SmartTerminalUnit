#include "gui_common.h"
#include ".\MultiThread\multi_thread.h"

/* 1ms���ڶ�ʱ������������������ʱʹ�� */
static struct rt_timer Timer1;
/* 1ms���ڶ�ʱ������ */
static volatile uint32_t timer1Count = 0;

/**
  *@brief ��ʱ��1�жϴ�����
  *@param  parameter 
  *@retval None
  */
static void Timer1Timeout(void	*parameter)
{
	timer1Count ++;
}

/**
  *@brief ��ʱ��1��ʼ������
  *@param  None 
  *@retval None
  */
void time_static_init(void)
{
	/* 1ms�ж�һ�� */
	rt_timer_init(&Timer1,"gui_time",Timer1Timeout,\
				   RT_NULL,1,RT_TIMER_FLAG_PERIODIC);	
	rt_timer_start(&Timer1);
}

/**
  *@brief ��ȡ��ʼ��ʱ��
  *@param  None
  *@retval beginTick  ��ʱ��ʼ����
  */
inline uint32_t GetTimer1Tick(void)
{
	return timer1Count;
}

/**
  *@brief ��ȡ���������
  *@param  beginTick  ��ʱ��ʼ������GetTimer1Tick()�������
  *@retval ���������
  */
inline uint32_t GetTimer1IntervalTick(uint32_t beginTick)
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
