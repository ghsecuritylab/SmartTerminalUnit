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
** File name:               DLT634_HMI_app.c
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


/* INCLUDE FILES -------------------------------------------------------------*/
#include "dlt634_HMImaster_app.h"
#include "dlt634_HMImaster_disk.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
#pragma pack(push,1)

typedef struct 
{
    struct STATUS
    {
        uint16_t Length;
    }status;

    uint8_t	buff[_DLT634_HMIMASTER_LPDUSIZE-sizeof(struct STATUS)];
}DLT634_HMIMASTER_PASDU;

typedef struct 
{
//link	
    enum
	{
        RXDSTART = 0,	          // Ѱ����ʼ��־״̬
        RXDHEAD,	              // Ѱ�ұ���ͷ
        RXDCONTINUE             // ���պ�������
	}RxdStatus;           			// ����״̬
	
	uint16_t TxdHead;	      				// ���ͻ�����ͷָ��
	uint16_t TxdTail;	     					// ���ͻ�����βָ��
	uint8_t TxdBuf[_DLT634_HMIMASTER_LPDUSIZE];     	// ���ͻ�����
	
	uint16_t RxdHead;	      				// ����������ͷָ��
    uint16_t RxdTail;       				// ����������βָ��
	uint16_t RxdLength;     				// ����һ֡���ݳ���
	uint8_t RxdBuf[_DLT634_HMIMASTER_LPDUSIZE];			// ���ջ�����
	
	uint16_t TxdTail_Pri;
	uint8_t TxdBuf_Pri[_DLT634_HMIMASTER_LPDUSIZE]; // ���ͻ��������ݴ淢�͵����ݣ��ط�ʱ��
	
//app
	
	uint8_t StoreDATABuf[_DLT634_HMIMASTER_LPDUSIZE];     	// ���ݴ����ͻ�����
	
	DLT634_HMIMASTER_PASDU TX_Temp;              //�������ݴ���
	DLT634_HMIMASTER_PASDU RX_Temp;              //�������ݴ���
	
	uint32_t LinkFlag;              //�ظ���·���ݱ�־
	uint32_t DataFlag;             //Ӧ�����ݱ�־
	
	uint32_t ClockCounter;  				//�����������ڿ���
	uint32_t TimeOutTick_Pri; 			//��ʱ���ļ�ʱ��
	uint32_t RetryCount;						//��ʱ������
	
	uint32_t TimeOutTick_AskSta; 			//�ٻ���·���ļ�ʱ��
    uint32_t TimeOutTick_AllData; 
	
	uint8_t SendConCode;	  // ���͵Ŀ�����FCBλ��¼

}DLT634_HMIMASTER_APPINFO;
#pragma pack(pop)
/* PUBLIC VARIABLES ----------------------------------------------------------*/
DLT634_HMIMASTER_PAD      DLT634_HMIMaster_Pad[_DLT634_HMIMASTER_VOLUMES];
DLT634_HMIMASTER_APPINFO  DLT634_HMIMaster_App[_DLT634_HMIMASTER_VOLUMES];

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/


/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_CKS
** ��������: ����CKS
** �䡡  ��: pdrv���豸���
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
static uint8_t DLT634_HMI_MASTER_CKS(uint8_t pdrv ,uint8_t *pBuf)
{
    uint16_t sum = 0;
    uint16_t j =0;
    uint8_t *p = 0;
    uint16_t i = 0;

    if (*(pBuf) == _DLT634_HMIMASTER_STARTCODE10)
    {
        for (j = 0; j < (DLT634_HMIMaster_Pad[pdrv].LinkAddrSize + 1); j++)
        {
            sum += *(pBuf + 1 + j);
        }
    }
    else if(*(pBuf) == _DLT634_HMIMASTER_STARTCODE68)
    {
        p = pBuf+4;
        i = *(uint16_t *)(pBuf+1);

        while (i--)
        {
            sum += (*p++);
        }
    }

    return (uint8_t)(sum&0xff);
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_CheckFram68Valid
** ��������: �ɱ�֡���ļ��
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**         : DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_CKS ����CKS
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static uint8_t DLT634_HMI_MASTER_CheckFram68Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;
    
    pBuf = (DLT634_HMIMaster_App[pdrv].RxdBuf + DLT634_HMIMaster_App[pdrv].RxdHead);

	  // ��ͷУ��
    if (pBuf[0] != pBuf[3])
    {
        return(0);
    }

	  // ������ַУ��
    {
        if (((pBuf[5]|(pBuf[6]<<8)) != DLT634_HMIMaster_Pad[pdrv].SourceAddr) && ((pBuf[5]|(pBuf[6]<<8)) != 0xffff))
        {
            return (0);
        }
    }
    
    return (1);
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_CheckFram10Valid
** ��������: �̶�֡���ļ��
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**         : DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_CKS ����CKS
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static uint8_t DLT634_HMI_MASTER_CheckFram10Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;
    uint16_t addr = 0;

    pBuf = &DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead];

	  // �̶�֡�����ݵ�У��ͼ��
    if (pBuf[2+2] != (DLT634_HMI_MASTER_CKS(pdrv, pBuf)&0xFF))	
    {
        return (0);
    }

	  // �̶�֡�����ݵĽ���֡���
    if (pBuf[3+2] != _DLT634_HMIMASTER_ENDCODE)
    {
        return (0);
    }

	  // �̶�֡���ݹ�����ַ���
    {
        addr = pBuf[2] | ((pBuf[3]<<8));
    }
		
    if (addr != DLT634_HMIMaster_Pad[pdrv].SourceAddr)
    {
        return (0);
    }
    
    return (1);
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_DecodeFrame10
** ��������: �̶�֡���Ľ���
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**         : DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: 
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_DecodeFrame10(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint8_t control = 0;

    control = DLT634_HMIMaster_App[pdrv].RxdBuf[1];
    if (control & _DLT634_HMIMASTER_PRM) // ��վ��Ϊ�Ӷ�վ������վ������
    {
        switch (control & _DLT634_HMIMASTER_FUNCODE)
        {
            case _DLT634_HMIMASTER_M_FUN9: // �ٻ���·״̬
                DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta = DLT634_HMIMaster_Pad[pdrv].AskStaOutValue;
                DLT634_HMIMaster_App[pdrv].LinkFlag = 0;
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_REQSTATUS;
                /* ���Ա��������ʵĹ��̣����콨����· */ 
                while (((DLT634_HMIMaster_App[pdrv].RxdTail - DLT634_HMIMaster_App[pdrv].RxdHead) >= DLT634_HMIMaster_Pad[pdrv].FixFrmLength) && (stop == 0)) // �յ��ٻ���·״̬����ʱ�����������ͬ���
                {
                    if (memcmp(DLT634_HMIMaster_App[pdrv].RxdBuf, DLT634_HMIMaster_App[pdrv].RxdBuf + DLT634_HMIMaster_App[pdrv].RxdHead, DLT634_HMIMaster_Pad[pdrv].FixFrmLength) == 0)
                    {
                        DLT634_HMIMaster_App[pdrv].RxdHead += DLT634_HMIMaster_Pad[pdrv].FixFrmLength;
                    }
                    else
                    {
                        stop = 1;
                    }
                }
                break;

            case _DLT634_HMIMASTER_M_FUN0: // ��λ��·
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_RECONFIRM;
								DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_INITEND;
								DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_SENDABLE;
								DLT634_HMIMaster_App[pdrv].DataFlag |= _DLT634_HMIMASTER_CALLALLDATA;
                DLT634_HMIMaster_App[pdrv].SendConCode = 0x20;
                memset(DLT634_HMIMaster_App[pdrv].StoreDATABuf,0,sizeof(DLT634_HMIMaster_App[pdrv].StoreDATABuf));
								DLT634_HMI_MASTER_H_HMIYX(pdrv);	
                break;
            default:
                break;
        }
        return;
    }
    else // Prm=0 ƽ��ģʽ��վ��Ϊ����վ���յ���վ�����
    {
        switch (control & _DLT634_HMIMASTER_FUNCODE)
        {
            case _DLT634_HMIMASTER_S_FUN0:	// ȷ��
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_SENDABLE;
                DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = 0; // ��·�����ݾͿ��Բ��ط�
                break;

            case _DLT634_HMIMASTER_S_FUN11: // Ӧ����·
                DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = 0; // ��·�����ݾͿ��Բ��ط�
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_ASKRELINK;	
                break;

            default:
                break;
        }
    }
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_AppProcess
** ��������: Ӧ�ô�����
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**         : DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_C_SC  �������豸����ң��
**         : DLT634_HMI_MASTER_C_CS  �������豸���Ͷ�ֵ
**         : DLT634_HMI_MASTER_W_YXDATA  дYXDATA
**         : DLT634_HMI_MASTER_W_SOE  дSOE
**         : DLT634_HMI_MASTER_W_YCDATA  дYCDATA
**         : DLT634_HMI_MASTER_W_FEvent  дFEvent
**         : DLT634_HMI_MASTER_StoreIN  ���ͻ�������ջ 
**         : DLT634_HMI_MASTER_F_FR  �������豸�����ļ�����
**         : DLT634_HMI_MASTER_F_SR  �������豸�����������
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_AppProcess(uint8_t pdrv,uint8_t Control)
{		
    //�����02�汨�ģ����ڿ��ƺʹ���
    memset((uint8_t *)&DLT634_HMIMaster_App[pdrv].RX_Temp,0,sizeof(DLT634_HMIMASTER_PASDU));
    DLT634_HMIMaster_App[pdrv].RX_Temp.status.Length = *(uint16_t *)&DLT634_HMIMaster_App[pdrv].RxdBuf[1] -1 -2;
    memcpy((uint8_t *)&DLT634_HMIMaster_App[pdrv].RX_Temp.buff,&DLT634_HMIMaster_App[pdrv].RxdBuf[5+2],DLT634_HMIMaster_App[pdrv].RX_Temp.status.Length);

    DLT634_HMIMaster_App[pdrv].RX_Temp.status.Length += sizeof(DLT634_HMIMaster_App[pdrv].RX_Temp.status);
    
    if (Control & _DLT634_HMIMASTER_PRM) // ��վ��Ϊ�Ӷ�վ������վ��·���ġ�
    {
			
			DLT634_HMI_MASTER_TO_HMI(pdrv,(uint8_t *)&DLT634_HMIMaster_App[pdrv].RX_Temp);

    }
    else  // Prm=0����վ��Ϊ����վ������վ��·����,��ظ�����
    {
    }
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_DecodeFrame68
** ��������: �����ɱ�֡�屨��
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_AppProcess  Ӧ�ô�����
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_DecodeFrame68(uint8_t pdrv)
{
    uint8_t control;
    
    control = DLT634_HMIMaster_App[pdrv].RxdBuf[4];

    DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_RECONFIRM;
    DLT634_HMI_MASTER_AppProcess(pdrv,control);
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_SearchValidFrame
** ��������: ���ݴ�����
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_CheckFram68Valid  ���ɱ�֡����
**         : DLT634_HMI_MASTER_CheckFram10Valid  ���̶�֡����
**         : DLT634_HMI_MASTER_DecodeFrame68  �����ɱ��屨��
**         : DLT634_HMI_MASTER_DecodeFrame10  �����̶��屨��
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_SearchValidFrame(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint16_t length = 0;
    
    while ((DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail) && (!stop))
    {
        if (DLT634_HMIMaster_App[pdrv].RxdStatus == RXDSTART)
        {
            while ((DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead] != _DLT634_HMIMASTER_STARTCODE10)
                    && (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead] != _DLT634_HMIMASTER_STARTCODE68)
                    && (DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail))
            {
                DLT634_HMIMaster_App[pdrv].RxdHead++;
            }

            if (DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail) // �ҵ������ַ������������뻺�������롣
            {
                DLT634_HMIMaster_App[pdrv].RxdStatus = RXDHEAD;
                if (DLT634_HMIMaster_App[pdrv].RxdHead != 0)
                {
                    memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf,DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead,DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);
                    DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
                    DLT634_HMIMaster_App[pdrv].RxdHead = 0;
                }
            }
        }
        
        DLT634_HMIMaster_App[pdrv].RxdLength = DLT634_HMIMaster_App[pdrv].RxdTail - DLT634_HMIMaster_App[pdrv].RxdHead;
        if (DLT634_HMIMaster_App[pdrv].RxdStatus == RXDHEAD) // ���Ĺ�һ֡����
        {
            switch (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead])
            {
                case _DLT634_HMIMASTER_STARTCODE68:
                    if (DLT634_HMIMaster_App[pdrv].RxdLength >= 5+2) // �����ַ�����·��ַ��6�ֽڡ�
                    {
                        if (!DLT634_HMI_MASTER_CheckFram68Valid(pdrv)) // �ж��Ƿ���ͷ
                        {
                            DLT634_HMIMaster_App[pdrv].RxdHead++;
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
                    
                case _DLT634_HMIMASTER_STARTCODE10:
                    if (DLT634_HMIMaster_App[pdrv].RxdLength >= 4 + 2)
                    {
                        if (!DLT634_HMI_MASTER_CheckFram10Valid(pdrv))
                        {
                            DLT634_HMIMaster_App[pdrv].RxdHead++;
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
            }
        }
        if (DLT634_HMIMaster_App[pdrv].RxdStatus == RXDCONTINUE)
        {
            switch (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead])
            {
            case _DLT634_HMIMASTER_STARTCODE68:
                length = *(uint16_t *)&DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead+1];
                if (DLT634_HMIMaster_App[pdrv].RxdLength >= length+6) // ������ȫ�����д���
                {
                    if ((DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead+length+4] == (DLT634_HMI_MASTER_CKS(pdrv, DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead)&0xFF))//У����ȷ
                            && (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead+length+4+1] == _DLT634_HMIMASTER_ENDCODE)) // ��������ȷ
 					          {
                        if ( DLT634_HMIMaster_App[pdrv].RxdHead > 0 )
                        {
                            memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf,DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead,DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);//�����뻺��������
                            DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
                            DLT634_HMIMaster_App[pdrv].RxdHead = 0;
                        }
												/* ͷָ����Ƶ����ĺ� */
                        DLT634_HMIMaster_App[pdrv].RxdHead += (length+6);
                        DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;

                        DLT634_HMI_MASTER_DecodeFrame68(pdrv);
                    }
                    else
                    {
                        DLT634_HMIMaster_App[pdrv].RxdHead += 6;
                        DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
                    }
                }
                else // ����δ��ȫ�������������ȴ����ݡ�
                {
                    stop = 1;
                }
                break;
                
            case _DLT634_HMIMASTER_STARTCODE10:
                if (DLT634_HMIMaster_App[pdrv].RxdHead > 0)
                {
                    memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf,DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead,DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);
                    DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
                    DLT634_HMIMaster_App[pdrv].RxdHead = 0;
                }
                DLT634_HMIMaster_App[pdrv].RxdHead += 4 + DLT634_HMIMaster_Pad[pdrv].LinkAddrSize;
                DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
			
                DLT634_HMI_MASTER_DecodeFrame10(pdrv);
                break;
            }
        }
    }
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_ReadMISIData
** ��������: ��ȡ����
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_ReadData  ��ȡ����
**         : DLT634_HMI_MASTER_SearchValidFrame  ���ݴ�����
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_ReadMISIData(uint8_t pdrv)
{
    uint16_t count = 0;
    
    if (DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail) // ���ջ�����������δ���������
    {
        if (DLT634_HMIMaster_App[pdrv].RxdHead != 0)
        {
            memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf, DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead, DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);//��δ����������Ƶ�������ͷ
            DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
            DLT634_HMIMaster_App[pdrv].RxdHead = 0;
        }
    }
    else
    {
        DLT634_HMIMaster_App[pdrv].RxdHead = 0;
		    DLT634_HMIMaster_App[pdrv].RxdTail = 0;
    }
    
    count = DLT634_HMI_MASTER_ReadData(pdrv,(uint8_t *)&DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdTail], _DLT634_HMIMASTER_LPDUSIZE);
    if (count > 0)
    {
        DLT634_HMIMaster_App[pdrv].RxdTail += count;
        DLT634_HMI_MASTER_SearchValidFrame(pdrv);
    }
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_WriteDataToMISI
** ��������: ���ͺ���
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_WriteData  ���ͺ���
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static uint8_t DLT634_HMI_MASTER_WriteDataToMISI(uint8_t pdrv)
{
    uint16_t SendLen = 0;
   
    SendLen = DLT634_HMI_MASTER_WriteData(pdrv ,DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdHead, DLT634_HMIMaster_App[pdrv].TxdTail - DLT634_HMIMaster_App[pdrv].TxdHead);
	
    DLT634_HMIMaster_App[pdrv].TxdHead += SendLen;
    if (DLT634_HMIMaster_App[pdrv].TxdHead >= DLT634_HMIMaster_App[pdrv].TxdTail) // �ô����������Ѿ�����
    {
        DLT634_HMIMaster_App[pdrv].TxdHead = 0;
        DLT634_HMIMaster_App[pdrv].TxdTail = 0;
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_SlaveFrame10Packet
** ��������: �̶�֡�Ӷ�վ���
** �䡡  ��: pdrv���豸���
**         : Function  ������
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_CKS  ����CKS
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_SlaveFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;
									
    pBuf = DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_HMIMASTER_STARTCODE10;
    pBuf[1] = Function&0x0f;
    pBuf[1] |= DLT634_HMIMaster_Pad[pdrv].IEC_DIR; // ��վ����վ����

    for (i=0; i<DLT634_HMIMaster_Pad[pdrv].LinkAddrSize; i++)
    {
        pBuf[2+i] = (DLT634_HMIMaster_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = DLT634_HMI_MASTER_CKS(pdrv, pBuf);
    pBuf[3+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = _DLT634_HMIMASTER_ENDCODE;
    DLT634_HMIMaster_App[pdrv].TxdTail += DLT634_HMIMaster_Pad[pdrv].FixFrmLength;//�ƶ�����βָ��
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_MasterFrame10Packet
** ��������: �̶�֡����վ���
** �䡡  ��: pdrv���豸���
**         : Function  ������
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_CKS  ����CKS
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_MasterFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;

    pBuf = DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_HMIMASTER_STARTCODE10;
    pBuf[1] = Function&0x0f;
    pBuf[1] |= _DLT634_HMIMASTER_PRM;
    pBuf[1] |= DLT634_HMIMaster_Pad[pdrv].IEC_DIR; // ��վ����վ����

    for (i=0; i<DLT634_HMIMaster_Pad[pdrv].LinkAddrSize; i++)
    {
        pBuf[2+i] = (DLT634_HMIMaster_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = DLT634_HMI_MASTER_CKS(pdrv, pBuf);
    pBuf[3+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = _DLT634_HMIMASTER_ENDCODE;
    DLT634_HMIMaster_App[pdrv].TxdTail += DLT634_HMIMaster_Pad[pdrv].FixFrmLength;//�ƶ�����βָ��

    memcpy(DLT634_HMIMaster_App[pdrv].TxdBuf_Pri, pBuf, DLT634_HMIMaster_Pad[pdrv].FixFrmLength); // ���������ݱ��浽����վ�ط�������
    DLT634_HMIMaster_App[pdrv].TxdTail_Pri = DLT634_HMIMaster_Pad[pdrv].FixFrmLength;
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_Link10Process
** ��������: �̶�֡���ʹ���
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_WriteDataToMISI  ���ͺ���
**           DLT634_HMI_MASTER_SlaveFrame10Packet  �̶�֡�Ӷ�վ���
**           DLT634_HMI_MASTER_MasterFrame10Packet  �̶�֡����վ���
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_Link10Process(uint8_t pdrv)
{ 
	if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_REQSTATUS)
	{
		DLT634_HMI_MASTER_SlaveFrame10Packet(pdrv, _DLT634_HMIMASTER_S_FUN11);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_REQSTATUS);
		return;
	}
	
	if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_RECONFIRM)
	{
		DLT634_HMI_MASTER_SlaveFrame10Packet(pdrv, _DLT634_HMIMASTER_S_FUN0);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_RECONFIRM);
		return;
	}
	
	if((DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_ASKSTATUS)&&(!(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND)))
	{
		DLT634_HMI_MASTER_MasterFrame10Packet(pdrv, _DLT634_HMIMASTER_M_FUN9);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_ASKSTATUS);
		DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta = DLT634_HMIMaster_Pad[pdrv].AskStaOutValue;
		return;
	}
	
	if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_ASKRELINK)
	{
		DLT634_HMI_MASTER_MasterFrame10Packet(pdrv, _DLT634_HMIMASTER_M_FUN0);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_ASKRELINK);
		return;
	}	
}
/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_Data1Process
** ��������: 1�����ݷ��ʹ�����
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_ReadAllDataProcess  ���ٺ���
**           DLT634_HMI_MASTER_ReadTime  ��ʱ
**           DLT634_HMI_SLAVE_R_SOE  SOE��ȡ����
**           DLT634_HMI_MASTER_LinkTestProcess  ��������
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_Data1Process(uint8_t pdrv)
{
    uint16_t length;
    
    length = *(uint16_t *)&DLT634_HMIMaster_App[pdrv].StoreDATABuf[0];
    
    if(length != 0)
    {
        memcpy((uint8_t *)&DLT634_HMIMaster_App[pdrv].TX_Temp, (uint8_t *)DLT634_HMIMaster_App[pdrv].StoreDATABuf, length);
        memset((uint8_t *)DLT634_HMIMaster_App[pdrv].StoreDATABuf,0,sizeof(DLT634_HMIMaster_App[pdrv].StoreDATABuf)); 
				DLT634_HMI_SLAVE_EMPTY(pdrv);
        return;        
    }
    
//    if((DLT634_HMI_MASTER_H_HMIYX(pdrv))||(DLT634_HMIMaster_App[pdrv].DataFlag&_DLT634_HMIMASTER_CALLALLDATA))
//    {
//        DLT634_HMI_MASTER_R_HMIYX(pdrv,(uint8_t *)&DLT634_HMIMaster_App[pdrv].TX_Temp);
//        DLT634_HMIMaster_App[pdrv].DataFlag &= ~_DLT634_HMIMASTER_CALLALLDATA;
//        return; 
//    }	
}
/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_MasterFrame68Packet
** ��������: �ɱ�֡����վ���ݴ��
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_CKS  ����CKS
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_MasterFrame68Packet(uint8_t pdrv, uint8_t Function)
{
    uint16_t length = 0;
    uint8_t num = 0;
    uint8_t *pBuf;
    
    pBuf = DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail;

    pBuf[0] = pBuf[3] = _DLT634_HMIMASTER_STARTCODE68;
    pBuf[1] = pBuf[2] = DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMIMaster_App[pdrv].TX_Temp.status)+ 1 + 2;
    pBuf[4] = Function&0x0f;
    if(DLT634_HMIMaster_App[pdrv].SendConCode&_DLT634_HMIMASTER_FCB)
    {DLT634_HMIMaster_App[pdrv].SendConCode = 0;}
    else
    {DLT634_HMIMaster_App[pdrv].SendConCode = _DLT634_HMIMASTER_FCB;}
    pBuf[4] |= _DLT634_HMIMASTER_FCV;
    pBuf[4] |= DLT634_HMIMaster_App[pdrv].SendConCode;
    pBuf[4] |= _DLT634_HMIMASTER_PRM;

    pBuf[5] = DLT634_HMIMaster_Pad[pdrv].SourceAddr&0xff;
    pBuf[6] = (DLT634_HMIMaster_Pad[pdrv].SourceAddr>>8)&0xff;
    
    num = 7;
    memcpy(&pBuf[num], &DLT634_HMIMaster_App[pdrv].TX_Temp.buff, DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMIMaster_App[pdrv].TX_Temp.status));
    num += DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMIMaster_App[pdrv].TX_Temp.status);
		
    length = num + 2; 
    pBuf[1] = (length - 6)&0xff;
    pBuf[2] = ((length - 6)>>8)&0xff;
    
    pBuf[length-2] = DLT634_HMI_MASTER_CKS(pdrv, pBuf)&0xFF;
    pBuf[length-1] = _DLT634_HMIMASTER_ENDCODE;
    DLT634_HMIMaster_App[pdrv].TxdTail += length;

    memcpy(DLT634_HMIMaster_App[pdrv].TxdBuf_Pri, pBuf, length);
    DLT634_HMIMaster_App[pdrv].TxdTail_Pri = length;
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_Link68Process
** ��������: �ɱ�֡���ͺ���
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_MasterFrame68Packet  �ɱ�֡����վ���
**           DLT634_HMI_MASTER_WriteDataToMISI  ���ݷ��ͺ���
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_Link68Process(uint8_t pdrv)
{
    if(DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length)
    {
        DLT634_HMI_MASTER_MasterFrame68Packet(pdrv, _DLT634_HMIMASTER_M_FUN3);
        DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
        DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = DLT634_HMIMaster_Pad[pdrv].TimeOutValue;
        DLT634_HMIMaster_App[pdrv].RetryCount  = _DLT634_HMIMASTER_NUMOF_MAXRETRY;
        DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_SENDABLE);
        memset((uint8_t *)&DLT634_HMIMaster_App[pdrv].TX_Temp,0,sizeof(DLT634_HMIMASTER_PASDU));
    }
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_SendProcess
** ��������: ���ʹ�����
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_Link10Process  �̶�֡�������ݺ���
**           DLT634_HMI_MASTER_Data1Process  1�����ݷ������ݺ���
**           DLT634_HMI_MASTER_Link68Process  �ɱ�֡���ͺ���
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_SendProcess(uint8_t pdrv)
{   	
    if(DLT634_HMIMaster_App[pdrv].LinkFlag&0x000000ff)
    {
        DLT634_HMI_MASTER_Link10Process(pdrv);
        return;
    }
    

    if((DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_SENDABLE)&&(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND))
    {
        DLT634_HMI_MASTER_Data1Process(pdrv);
        DLT634_HMI_MASTER_Link68Process(pdrv);
        return;
    }
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_LinkOnTimer
** ��������: ��·�㶨ʱ��������
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_WriteDataToMISI  �������ݺ���
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
void DLT634_HMI_MASTER_LinkOnTimer(uint8_t pdrv)
{		
    if (DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri) // ��λ����
    {
        DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri--;
        if (!DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri)
        {
            if (!DLT634_HMIMaster_App[pdrv].RetryCount)
            {
                    return;
            }

            DLT634_HMIMaster_App[pdrv].RetryCount--;
            if (DLT634_HMIMaster_App[pdrv].RetryCount)
            {
                memcpy(DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail, DLT634_HMIMaster_App[pdrv].TxdBuf_Pri, DLT634_HMIMaster_App[pdrv].TxdTail_Pri);
                DLT634_HMIMaster_App[pdrv].TxdTail += DLT634_HMIMaster_App[pdrv].TxdTail_Pri;

                DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
                DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = DLT634_HMIMaster_Pad[pdrv].TimeOutValue;
                return;
            }
            else // �����ط����������½�����·����֪ͨӦ�ò㡣
            {
                DLT634_HMIMaster_App[pdrv].LinkFlag = 0;
            }
        }
    }	
    
    if(!(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND))
    {
        if (!DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta)
        {
            DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_ASKSTATUS;
        }
        else
        {
            DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta--;
        }
    }
    
    if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND)
    {
        if (!DLT634_HMIMaster_App[pdrv].TimeOutTick_AllData)
        {
            DLT634_HMIMaster_App[pdrv].TimeOutTick_AllData = DLT634_HMIMaster_Pad[pdrv].AllDataOutValue;
            DLT634_HMIMaster_App[pdrv].DataFlag |= _DLT634_HMIMASTER_CALLALLDATA;
        }
        else
        {
            DLT634_HMIMaster_App[pdrv].TimeOutTick_AllData--;
        }
    }
}

/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_REPLY
** ��������: �����豸Ӧ����
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**         : DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_StoreIN  ���ͻ�������ջ
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
uint8_t DLT634_HMI_MASTER_REPLY(uint8_t pdrv, uint8_t *pbuf)//�ظ�
{ 
    uint16_t length;
    
    length = *(uint16_t *)&pbuf[0];
    
    memcpy(DLT634_HMIMaster_App[pdrv].StoreDATABuf,pbuf,length);
    
	return(TRUE);
}

/* -----------------------------------------------------------------------------
** ��������: DLT634_HMI_MASTER_Clock
** ��������: ��ʱ������
** �䡡  ��: pdrv���豸���
** �䡡  ��: ��
** ȫ�ֱ���: DLT634_HMIMaster_App[pdrv]�����в����ṹ��
**           DLT634_HMIMaster_Pad[pdrv]���趨�����ṹ��
** ����ģ��: DLT634_HMI_MASTER_ReadMISIData  ��ѯMISI���ջ�����
**           DLT634_HMI_MASTER_SendProcess  ���ʹ�����
**           DLT634_HMI_MASTER_LinkOnTimer  ��·�㶨ʱ��������
** ����  ��: Mr.J
** �ա�  ��: 2017.8.3
** -----------------------------------------------------------------------------
** �� �� ��: 
** �޸�����:
** �ա�  ��: 
** ---------------------------------------------------------------------------*/
uint8_t DLT634_HMI_MASTER_Clock(uint8_t pdrv)
{
	uint8_t sta = 0;
	
	DLT634_HMIMaster_App[pdrv].ClockCounter++;
	if (DLT634_HMIMaster_App[pdrv].ClockCounter >= DLT634_HMIMaster_Pad[pdrv].ClockTimers)
	{
        DLT634_HMIMaster_App[pdrv].ClockCounter = 0;
                                            
        // ��ѯMISI���ջ�����
        DLT634_HMI_MASTER_ReadMISIData(pdrv);
    
        DLT634_HMI_MASTER_SendProcess(pdrv);
            
        // ��·�㶨ʱ��
        DLT634_HMI_MASTER_LinkOnTimer(pdrv);
	}
	
	if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND)
	{sta = 1;}
	return(sta);
}



