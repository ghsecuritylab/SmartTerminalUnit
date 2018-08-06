/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      goose_publisher_beat.h
* @brief:     goose�������Ļ���
* @version:   V0.0.1
* @author:    Zhang Yufei
* @date:      2018/8/4
* @update:
*/

#include <stdint.h>

#include "rtdef.h"

#define  MAX_BEAT  8
/** 
*googse ���Ŀ���
*/
typedef struct TagGooseBeat
{
	uint32_t beat[MAX_BEAT]; //��������
	uint8_t next; //��һ��Beat
	uint8_t count; //��������
	struct rt_timer timer;
	uint8_t (*Execute)(struct TagGooseBeat* handle);
}GooseBeat;


extern void TestGooseBeat(void);
