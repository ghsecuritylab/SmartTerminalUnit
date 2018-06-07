/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- File Info ----------------------------------
** File name:               dlt634_HMImaster_app.h
** Descriptions:            The application layer of DL/T634.HMI_2002
**
** -----------------------------------------------------------------------------
** Created by:              Mr.Lee
** Created date:            2015.10.25
** System:                  Protocol Software System
** Version:                 V3.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef	_DLT634_HMIMASTER_APP_H_
#define _DLT634_HMIMASTER_APP_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "dlt634_HMImaster_config.h"
#include "stdint.h"

#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif

/* PUBLIC VARIABLE -----------------------------------------------------------*/

typedef struct 
{
	uint8_t Port;            //�˿�
	
	uint8_t IEC_DIR;         // �ն�0x80,��վ0x00
	uint8_t SourceAddr;	    // Դ��ַ����������ַ
	
	uint8_t LinkAddrSize;   //��·��ַ����
	uint8_t ASDUCotSize;    //����ԭ�򳤶�
	
	uint8_t ASDUAddr;       //ASDU��ַ
	uint8_t ASDUAddrSize;    //ASDU��ַ����
	uint8_t FixFrmLength;		//�̶�֡����
	
	uint32_t ClockTimers;		//���ó�������������ִ��һ�Σ�������������
	uint32_t TimeOutValue;		//��ʱ�ط����
	
	uint32_t AskStaOutValue;		//��ʱ�ط����
	uint32_t AllDataOutValue;		//��ʱ�ط����
	uint32_t LinkTestValue;		//��ʱ�ط����
	uint32_t LinkIdleValue;		//��·���м�ʱ��
	
	uint32_t DevidValue;		   //�豸ռ�ü�ʱ��
	
}DLT634_HMIMASTER_PAD;

extern DLT634_HMIMASTER_PAD      DLT634_HMIMaster_Pad[_DLT634_HMIMASTER_VOLUMES];

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern uint8_t DLT634_HMI_MASTER_Clock(uint8_t pdrv);
extern uint8_t DLT634_HMI_MASTER_REPLY(uint8_t pdrv, uint8_t *pbuf);//�ظ�

#endif /* END _DLT634_HMI_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/


