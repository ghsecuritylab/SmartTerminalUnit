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
** File name:               encrypt_app.h
** Descriptions:            The application layer of encryp
**
** -----------------------------------------------------------------------------
** Created by:              Mr.J
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
#ifndef	_ENCRYPT_APP_H_
#define _ENCRYPT_APP_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "encrypt_config.h"
#include "integer.h"

#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif

/* PUBLIC VARIABLE -----------------------------------------------------------*/
#pragma pack(push,1)
__packed typedef struct 
{
    __packed struct STATUS//ENCRYPT_PASDU-2
    {
        BYTE Length_H;
        BYTE Length_L;
    }status;
		
    __packed struct HEAD//��������
    {
        BYTE MesType_H;
        BYTE MesType_L;
    }Head;
		
    __packed struct//��װ������
    {
        BYTE Type;
        BYTE buff[_ENCRYPT_LPDUSIZE-sizeof(struct STATUS)-sizeof(struct HEAD)-1];
    }Data;
}ENCRYPT_PASDU;

typedef struct 
{
//link	

	enum
	{
        RXDSTART = 0,	          // Ѱ����ʼ��־״̬
        RXDHEAD,	              // Ѱ�ұ���ͷ
        RXDCONTINUE             // ���պ�������
	}RxdStatus;           			// ����״̬
	
	WORD TxdHead;	      				// ���ͻ�����ͷָ��
	WORD TxdTail;	     					// ���ͻ�����βָ��
	BYTE TxdBuf[_ENCRYPT_LPDUSIZE];     	// ���ͻ�����
	
	WORD RxdHead;	      				// ����������ͷָ��
	WORD RxdTail;       				// ����������βָ��
	WORD RxdLength;     				// ����һ֡���ݳ���
	BYTE RxdBuf[_ENCRYPT_LPDUSIZE*2];			// ���ջ�����
	
//app
	ENCRYPT_PASDU RxTemp;     	// �������ݴ���
    ENCRYPT_PASDU TxTemp;     	// �������ݴ���
	
    ENCRYPT_PASDU StoreDATA1Temp;             //���
	ENCRYPT_PASDU *StoreDATA1IN;              //�����ͻ�����1��������ָ��
	ENCRYPT_PASDU *StoreDATA1OUT;             //�����ͻ�����1�����ݳ�ָ��
	ENCRYPT_PASDU StoreDATA1Buf[_ENCRYPT_STOREDATA1NUM];     	// 1�����ݴ����ͻ�����
	
	DWORD LinkFlag;              //�ظ���·���ݱ�־
	
	WORD IECLength;     				// IEC���ݳ���
	BYTE IECBuf[_ENCRYPT_LPDUSIZE];			// IEC���ջ�����

	BYTE IVData[16];   //ǰ8λR1,��8λR1��λȡ��
	
	BYTE Object;       //���Ӷ���
	BYTE M_ID[8];       //֤�������ID
    BYTE R_NUM[8];       //�����
	
	BYTE CerContent[_ENCRYPT_LPDUSIZE*2]; // ֤������
    
    BYTE Enc_txbuff[_ENCRYPT_LPDUSIZE*2];
    BYTE Enc_rxbuff[_ENCRYPT_LPDUSIZE*2];
	
}ENCRYPT_APPINFO;
#pragma pack(pop)

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern WORD Encrypt_ReadMISIData(BYTE *pbuf, BYTE pdrv);
extern BYTE Encrypt_IECSendData(BYTE *pbuf, WORD count, BYTE type, BYTE pdrv);
extern BYTE Encrypt_ENCSendData(BYTE pdrv);
extern BYTE Encrypt_AppInit(BYTE pdrv);

#endif /* END _ENCRYPT_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/


