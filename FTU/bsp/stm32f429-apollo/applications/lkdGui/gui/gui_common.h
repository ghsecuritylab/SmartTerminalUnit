#ifndef __COMMON_H__
#define __COMMON_H__

#include <rtthread.h>


uint32_t GetTimer1Tick(void);/* ��ȡTimer1��ǰ����*/
/* ��ȡTimer1�������������ļ������*/
uint32_t GetTimer1IntervalTick(uint32_t beginTick);

void time_static_init(void); /* Timer1��ʼ��*/

#endif
