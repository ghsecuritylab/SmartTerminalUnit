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
** File name:               DLT634_HMI_master_disk.c
** Descriptions:            The application of DLT634.HMI_2002
**
** -----------------------------------------------------------------------------
** Created by:              Mr.J
** Created date:            2017.5.16
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

#include "dlt634_HMImaster_disk.h"
#include "dlt634_HMImaster_app.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
/* PUBLIC VARIABLES ----------------------------------------------------------*/


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
//link
/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_ReadData
** ��������: ������
** �䡡  ��: pdrv���豸���
**           pbuf����������ָ��
**           count��������
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: Readx  ��ײ������
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
uint16_t DLT634_HMI_MASTER_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 
	uint16_t len = 0;  
	
	switch(pdrv)
	{
		case 0:
			len = Readx(pbuf, count);

			break;
		default:
			break;
	}
	return(len);
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_WriteData
** ��������: д����
** �䡡  ��: pdrv���豸���
**           pbuf����������ָ��
**           count��д����
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: WriteX  ��ײ�д����
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
uint16_t DLT634_HMI_MASTER_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 
	uint16_t len = 0; 
	
	switch(pdrv)
	{
		case 0:
			len = WriteX(pbuf,count);
			break;
		default:
			break;
	}
	return(len);
}

//app
/**
  * @brief : TO HMI.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_MASTER_TO_HMI(uint8_t pdrv, uint8_t *pbuf)//TO HMI
{
	return hmi101Scan(pbuf);
}

/**
  * @brief : EMPTY.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_SLAVE_EMPTY(uint8_t pdrv)
{   
	Cmd101SendFinish();
}

/**
  * @brief : judg soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_MASTER_H_HMIYX(uint8_t pdrv)//
{
	Cmd101SendFinish();
    return(0);    
}

/**
  * @brief : read soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_MASTER_R_HMIYX(uint8_t pdrv, uint8_t *pbuf)//?HMIYX
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array(InfoAddr_L/InfoAddr_H/Value/CP56Time2a) 
    uint16_t length;
 
    switch(pdrv)
    {
        case DLT634_HMIMASTER_DISK0:
				length = 6;
        pbuf[0] = length&0xff;
        pbuf[1] = (length>>8)&0xff;
        pbuf[2] = 0;
        pbuf[3] = 0;
        pbuf[4] = 0;
        pbuf[5] = 0;
        break;
    }
}
/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_SLAVE_C_REPLY
** ��������: �����豸�ظ�
** �䡡  ��: drvid��Ӧ���豸���
**         : pbuf���ظ���������ָ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: DBSend  ���͸������豸����
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/
uint8_t DLT634_HMI_MASTER_C_REPLY(uint8_t drvid,uint8_t *pbuf)//�����豸�ظ�
{
    DLT634_HMI_MASTER_REPLY(0, pbuf); 

	return(TRUE);
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_INIT
** ��������: ���ò�����ʼ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: 
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
void DLT634_HMI_MASTER_INIT(uint8_t pdrv)//��ʼ��
{
    switch(pdrv)
	{
        case 0:
            DLT634_HMIMaster_Pad[pdrv].IEC_DIR = 0;
            DLT634_HMIMaster_Pad[pdrv].SourceAddr = 1;
            DLT634_HMIMaster_Pad[pdrv].LinkAddrSize = 2;//1:97,2:02
            DLT634_HMIMaster_Pad[pdrv].ASDUCotSize = 2;//1:97,2:02
            DLT634_HMIMaster_Pad[pdrv].ASDUAddr = 1;
            DLT634_HMIMaster_Pad[pdrv].ASDUAddrSize = 2;//1:97,2:02
            DLT634_HMIMaster_Pad[pdrv].FixFrmLength = 6;//5:97,6:02
            DLT634_HMIMaster_Pad[pdrv].ClockTimers = 1;//�������м������
            DLT634_HMIMaster_Pad[pdrv].TimeOutValue = 10;//�ط��������
            DLT634_HMIMaster_Pad[pdrv].AskStaOutValue = 100;//��·����������
						DLT634_HMIMaster_Pad[pdrv].AllDataOutValue = 500;
            break;
        default:
            break;
	}
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MasterTask
** ��������: 101��վ������
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ����ģ��: DLT634_HMI_MASTER_Clock  ��ʱִ�к���
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
void DLT634_HMI_MasterTask(void* arg)
{
	DLT634_HMI_MASTER_Clock(DLT634_HMIMASTER_DISK0);
}

/* END OF FILE ---------------------------------------------------------------*/

