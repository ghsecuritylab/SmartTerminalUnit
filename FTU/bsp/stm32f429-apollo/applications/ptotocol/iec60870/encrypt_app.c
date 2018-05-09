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
** File name:               encrypt_app.c
** Descriptions:            The application of encrypt
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
#include "encrypt_app.h"
#include "encrypt_disk.h"

/* PUBLIC VARIABLES ----------------------------------------------------------*/
ENCRYPT_APPINFO  *Encrypt_App = RT_NULL;

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/

static BYTE Encrypt_CheckFramValid(BYTE pdrv)
{
    BYTE *pBuf = 0;
    
    pBuf = (Encrypt_App[pdrv].RxdBuf + Encrypt_App[pdrv].RxdHead);

	  // ��ͷУ��
    if (pBuf[0] != pBuf[3])/// EB = EB
    {
        return(0);
    }
    
    return (1);
}

static WORD Encrypt_CKS(BYTE pdrv ,BYTE *pBuf)
{
    WORD sum = 0;
    BYTE *p = 0;
    BYTE i = 0;

    if(*(pBuf) == _ENCRYPT_STARTCODE)
    {
        p = pBuf+4;
        i = ((*(pBuf+1)<<8)|(*(pBuf+2)));

        while (i--)
        {
            sum += (*p++);
        }
    }

    return (WORD)(sum);
}

BYTE Encrypt_LRC(WORD num,BYTE *pBuf)
{
	WORD i;
	BYTE temp;
	
	for(temp=0x00,i=0;i<num;i++)
	{
		temp = (temp^pBuf[i]);
	}
    temp = ~temp;
	return(temp);
}

static BYTE Encrypt_StoreIN(BYTE pdrv,ENCRYPT_PASDU *buf)
{	
	ENCRYPT_PASDU *temp;
	
	temp = Encrypt_App[pdrv].StoreDATA1IN;///1��������ָ��
	if(++temp >= Encrypt_App[pdrv].StoreDATA1Buf + _ENCRYPT_STOREDATA1NUM)
	{
		temp = Encrypt_App[pdrv].StoreDATA1Buf;
	}
	
	if(temp == Encrypt_App[pdrv].StoreDATA1OUT)
	{
		return(FALSE);
	}
	
	memcpy(Encrypt_App[pdrv].StoreDATA1IN,buf,sizeof(ENCRYPT_PASDU));///�����ݷ��� һ������ָ��	Ϊ�˷���1������

	if(++Encrypt_App[pdrv].StoreDATA1IN >= Encrypt_App[pdrv].StoreDATA1Buf + _ENCRYPT_STOREDATA1NUM)
	{
		Encrypt_App[pdrv].StoreDATA1IN = Encrypt_App[pdrv].StoreDATA1Buf;
	}
		
	return(TRUE);
}

static void Encrypt_StatusReset(BYTE pdrv)
{///�����ͻ�����1����ָ�� = 1�����ݻ�����
    Encrypt_App[pdrv].StoreDATA1IN = Encrypt_App[pdrv].StoreDATA1Buf;
    Encrypt_App[pdrv].StoreDATA1OUT = Encrypt_App[pdrv].StoreDATA1Buf;///1����ָ��
    memset(Encrypt_App[pdrv].StoreDATA1Buf,0,sizeof(Encrypt_App[pdrv].StoreDATA1Buf));//������0
}

static BYTE Encrypt_WriteDataToMISI(BYTE pdrv)
{
    WORD sendlen = 0;
   
    sendlen = Encrypt_WriteData(Encrypt_App[pdrv].TxdBuf + Encrypt_App[pdrv].TxdHead , Encrypt_App[pdrv].TxdTail - Encrypt_App[pdrv].TxdHead , pdrv );
    
    if(sendlen < _ENCRYPT_LPDUSIZE*2)
    {
        Encrypt_App[pdrv].TxdHead += sendlen;
        if (Encrypt_App[pdrv].TxdHead >= Encrypt_App[pdrv].TxdTail) // �ô����������Ѿ�����
        {
            Encrypt_App[pdrv].TxdHead = 0;
            Encrypt_App[pdrv].TxdTail = 0;
            return (TRUE);
        }
        else
        {
            return (FALSE);
        }
    }
    else
    {
        return (FALSE);
    }
}

static void Encrypt_ReturnFaultInfo(BYTE apptype,BYTE faulttype_h,BYTE faulttype_l,BYTE pdrv)
{
    WORD num=0;

    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
    Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = apptype;//Ӧ������
    num=0;
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x02;//��Ϣ��ȫ��չ��len
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = faulttype_h;
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = faulttype_l;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
    Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
}

static void Encrypt_TakeRandomNum(BYTE *txbuff,BYTE *rxbuff)
{
    WORD num=0;
    
    txbuff[num++] = 0x55;
    txbuff[num++] = 0x00;
    txbuff[num++] = 0x84;
    txbuff[num++] = 0x00;
    txbuff[num++] = 0x08;
    txbuff[num++] = 0x00;
    txbuff[num++] = 0x00;
    txbuff[num] = Encrypt_LRC(num-1,&txbuff[1]);
    num++;

    Encrypt_ChipCOM(txbuff,rxbuff);//ȡ�����
}

static void Encrypt_VerifySign(BYTE *txbuff, BYTE *rxbuff ,BYTE pdrv)
{
    WORD num=0;
    WORD length=0;
    
    length = ((Encrypt_App[pdrv].RxTemp.status.Length_H<<8)|Encrypt_App[pdrv].RxTemp.status.Length_L);
    txbuff[num++] = 0x55;
    txbuff[num++] = 0x80;
    txbuff[num++] = 0x08;
    txbuff[num++] = 0x00;
    txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[(length-1)-2-1];//����-1�������һλ��-2�������ͣ�-1Ӧ������
    txbuff[num++] = ((length-2-1)>>8)&0xff;//-2�������ͣ�-1ASKID
    txbuff[num++] = ((length-2-1))&0xff;
    memcpy(&txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.Type,(length-2-1));
    num += length;
    txbuff[num] = Encrypt_LRC(num-1,&txbuff[1]);
    num++;

    Encrypt_ChipCOM(txbuff,rxbuff);
}

static BYTE Encrypt_Ciphertext(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    BYTE res=FALSE;

    memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
    if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_B)//���Ӷ�������վ
    {
        num=0;
        length = ((Encrypt_App[pdrv].RxdBuf[1]<<8)|(Encrypt_App[pdrv].RxdBuf[2]));
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;///оƬSPI��ʼ����0x55
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x2c;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x60;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
        Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 16 - 2)>>8)&0xff;///�������ݳ���  �����ݳ���+IVdata:��վ�������
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 16 - 2)&0xff;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
        num += 16;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxdBuf[6],(length - 2));///Encrypt_App[pdrv].RxdBuf[6]��װ������ʼλ
        num += (length - 1);
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);///MACУ��
        num++;
    }
    else if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_M)//���Ӷ�����֤�������
    {
        num=0;
        length = ((Encrypt_App[pdrv].RxdBuf[1]<<8)|(Encrypt_App[pdrv].RxdBuf[2]));
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x2c;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 8 + 8 + 16 - 2)>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 8 + 8 + 16 - 2)&0xff;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].M_ID,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
        num += 16;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxdBuf[6],(length - 2));
        num += (length - 2);
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        num++;
    }
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);///�����оƬд���� ��������
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].RxdBuf[1] = ((((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3])+2)>>8)&0xff;///�������ݳ���
        Encrypt_App[pdrv].RxdBuf[2] = (((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3])+2)&0xff;
        memcpy(&Encrypt_App[pdrv].RxdBuf[6],&Encrypt_App[pdrv].Enc_rxbuff[4],(((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3])+2));///����

        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);///����

        Encrypt_App[pdrv].RxTemp.status.Length_H = ((length+2)>>8)&0xff;
        Encrypt_App[pdrv].RxTemp.status.Length_L = (length+2)&0xff;
        memcpy(&Encrypt_App[pdrv].RxTemp.Data,&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        res = TRUE;
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        res = FALSE;
    }
    return(res);
}

static BYTE Encrypt_encrypttext(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    BYTE res=FALSE;

    memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
    if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_B)//���Ӷ�������վ
    {
        if(Encrypt_App[pdrv].TxTemp.Data.Type == 0x00)
        {
            num=0;
            length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L)-2;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
            Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 16)>>8)&0xff;
            Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 16)&0xff;
            memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
            num += 16;
            memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].TxTemp.Data,length);
            num += length;
            Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
            num++;
            Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
        }
        else
        {
            Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
            
            if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
            {
                num=0;
                length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L)-2;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x60;////0x62  change by chen
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
                Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 16 + 8)>>8)&0xff;
                Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 16 + 8)&0xff;
                memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
                num += 16;
                memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].TxTemp.Data,length);
                num += length;
                memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],8);
                num += 8;
                Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
                num++;
                Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
            }
            else
            {
                Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
                Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_SANDABLE;
                res = FALSE;
            }
        }
    }
    else if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_M)//���Ӷ�����֤�������
    {
        num=0;
        length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L)-2;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 8 + 8 + 16)>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 8 + 8 + 16)&0xff;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].M_ID,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
        num += 16;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].TxTemp.Data,length);
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        num++;
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    }
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        Encrypt_App[pdrv].TxTemp.status.Length_H = ((length+2)>>8)&0xff;
        Encrypt_App[pdrv].TxTemp.status.Length_L = ((length+2))&0xff;
        memcpy(&Encrypt_App[pdrv].TxTemp.Data,&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        res = TRUE;
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_SANDABLE;
        res = FALSE;
    }
    return(res);
}

static void Encrypt_PackSend(BYTE pdrv)
{
    BYTE *pBuf;
    WORD length;

    pBuf = Encrypt_App[pdrv].TxdBuf + Encrypt_App[pdrv].TxdTail;
    
    if(Encrypt_App[pdrv].TxTemp.Head.MesType_L&_ENCRYPT_ENC)///�жϼ��ܱ�ʶ
    {
        if(!Encrypt_encrypttext(pdrv))///�жϼ��� ���͸�����оƬ����ȡ��ֵ����return
        {return;}
    }

    length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L);

    pBuf[0] = pBuf[3] = _ENCRYPT_STARTCODE;
    pBuf[1] = (length>>8)&0xff;
    pBuf[2] = length&0xff;

    memcpy(&pBuf[4],&Encrypt_App[pdrv].TxTemp.Head,length);

    length += 6; // ȫ�����ĳ���	
    pBuf[length-2] = Encrypt_CKS(pdrv, pBuf)&0xFF;
    pBuf[length-1] = _ENCRYPT_ENDCODE;

    Encrypt_App[pdrv].TxdTail += length;

    Encrypt_WriteDataToMISI(pdrv);
}

static BYTE Encrypt_EncryptCheck(BYTE pdrv)
{
    BYTE res=TRUE;
    WORD length;
	
	length = (Encrypt_App[pdrv].RxdBuf[1]<<8)|Encrypt_App[pdrv].RxdBuf[2];///���ݳ���
	
	Encrypt_App[pdrv].RxTemp.status.Length_H = (length>>8)&0xff;
	Encrypt_App[pdrv].RxTemp.status.Length_L = length&0xff;
	memcpy(&Encrypt_App[pdrv].RxTemp.Head,&Encrypt_App[pdrv].RxdBuf[4],length);///�ӱ������Ϳ�ʼ
	///�Ѵӱ������Ϳ�ʼ���� �͵�Encrypt_App[pdrv].RxTemp.Head���������� ͷ��
    switch(Encrypt_App[pdrv].RxTemp.Data.Type)///�жϱ�������
    {
        case _ENCRYPT_I_FUN00:///0x00 ������ ֻ��0x00
            if(((Encrypt_App[pdrv].RxdBuf[1]<<8)|Encrypt_App[pdrv].RxdBuf[2])>20)///���ݳ��Ȳ��ܳ���10 /////change by chen
            {res = FALSE;}
            break;
        case _ENCRYPT_I_FUN01:case _ENCRYPT_I_FUN02:case _ENCRYPT_I_FUN03:case _ENCRYPT_I_FUN04:
        case _ENCRYPT_I_FUN05:case _ENCRYPT_I_FUN06:case _ENCRYPT_I_FUN07:case _ENCRYPT_I_FUN08:
            res = FALSE;
            break;
        //�����������
    }
    
    if(res == FALSE)///������ݳ��ȳ���10 ���߱������Ͳ�Ϊ0x00
    {
        Encrypt_ReturnFaultInfo(0x1f,0x91,0x06,pdrv);///���ش�������Ϣ
    }
    return(res);
}

//����/����/֤���ʶ/��֡��/��ǰ֡���/֤�������N/֤�鹤��ID
static void Encrypt_AppProcess_M_FUN30(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//��һ���建����
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
        Encrypt_StatusReset(pdrv);
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����δ�洢λ��
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//�����֡���Ѵ��ڣ�������
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
		
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-11);//�洢
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
        memcpy(Encrypt_App[pdrv].M_ID,&Encrypt_App[pdrv].RxTemp.Data.buff[5+length_n],8);//֤��ID��ֵ
    }
		
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//�Ѵ洢��������Ҫ��
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1e;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//���
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����λ��
            {
                length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                {
                    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                    break;
                }
                num_n = num_n+length_n+3;
                if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//���������֤֤��
        
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {       
            Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//ȡ�����
        
            if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
            {
                Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
                Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
                Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x31;//Ӧ������
                num=0;
                Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
                Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x08;//��Ϣ��ȫ��չ��len
                memcpy(Encrypt_App[pdrv].IVData,&Encrypt_App[pdrv].Enc_rxbuff[4],8);
                for(i=0;i<8;i++)
                {
                    Encrypt_App[pdrv].IVData[i+8] = ~Encrypt_App[pdrv].IVData[i];
                }
                memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],8);
                num += 8;
                Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
                Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
                Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
            }
            else
            {
                Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
            }
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x45,0x90,0x00,pdrv);
    }
}

//����/����/ǩ��
static void Encrypt_AppProcess_M_FUN32(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x20;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x40;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],0x40);
    num += 0x40;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_M_CERSUCCESS;
    }
    Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//����/����
static void Encrypt_AppProcess_M_FUN34(BYTE pdrv)
{
    BYTE keyversion;	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1A;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        keyversion = Encrypt_App[pdrv].Enc_txbuff[4];
    
        Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x35;//Ӧ������
            num=0;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x09;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = keyversion;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
            num += 0x08;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }		
}

//����/����
static void Encrypt_AppProcess_M_FUN36(BYTE pdrv)
{
    WORD num=0;
    BYTE id[24] ="F20xxxxxx201708250906";

    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
    Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x37;//Ӧ������
    num=0;
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x18;//��Ϣ��ȫ��չ��len
    memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],id,0x18);
    num += 0x18;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
    Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);	
}

//����/����
static void Encrypt_AppProcess_M_FUN38(BYTE pdrv)
{
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x99;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x39;//Ӧ������
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x08;//��Ϣ��ȫ��չ��len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
        num += 0x08;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//����/����
static void Encrypt_AppProcess_M_FUN3A(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x30;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        length = (Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3];
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x08&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x3B;//Ӧ������
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length)&0xff;
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        num += length;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = ((num+2+1))&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//����/����/��֡��/��ǰ֡���/֤����������
static void Encrypt_AppProcess_M_FUN3C(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[3] == 1)//��һ���建����
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����δ�洢λ��
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[3] == Encrypt_App[pdrv].CerContent[num_n])//�����֡���Ѵ��ڣ�������
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
    
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-2);//�洢
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[3];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[4],length_n);
    }
    
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[2])//�Ѵ洢��������Ҫ��
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x0a;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[2];i++)//���
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����λ��
            {
                    length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                    if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                    {
                        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                        break;
                    }
                    num_n = num_n+length_n+3;
                    if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                    {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//���������֤֤��

        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x3D;//Ӧ������
            num=0;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x40;//��Ϣ��ȫ��չ��len
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x40);
            num += 0x40;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x45,0x90,0x00,pdrv);
    }
}

//����/����/֤���ʶ/��֡��/��ǰ֡���/֤���ļ�N
static void Encrypt_AppProcess_M_FUN3E(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//��һ���建����
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����δ�洢λ��
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//�����֡���Ѵ��ڣ�������
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
    
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-3);//�洢
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
    }
    
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//�Ѵ洢��������Ҫ��
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        if(Encrypt_App[pdrv].RxTemp.Data.buff[2] == 0x06)
        {
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x24;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        }
        else
        {
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x22;
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[2];
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//���
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����λ��
            {
                length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                {
                    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                    break;
                }
                num_n = num_n+length_n+3;
                if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//����֤��
        
        Encrypt_ReturnFaultInfo(0x3f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x45,0x90,0x00,pdrv);
    }
}

//����/����
static void Encrypt_AppProcess_M_FUN40(BYTE pdrv)
{
    WORD num=0;
    WORD length;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x81;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x81;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[4];
        Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[5];
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        num++;
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);		
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            num=0;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x24;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[2];
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[3];
            length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
            memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],length);
            num += length;
            Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
            num++;
        
            Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
            Encrypt_ReturnFaultInfo(0x41,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }	
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//����/����/֤���ʶ/��֡��/��ǰ֡���/�ն�֤��N
static void Encrypt_AppProcess_M_FUN42(BYTE pdrv)
{	
    WORD eachnum=0xf0;
    WORD group=0;
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    BYTE i;
	
    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x30;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
        
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {		
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        group = (length%eachnum) ? (length/eachnum + 1)  : (length/eachnum);
        Encrypt_StatusReset(pdrv);
        for(i=0;i<group;i++)
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x43;//Ӧ������
            num=0;
            length_n = ((i+1) == group) ? (length%eachnum)  : eachnum;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = ((length_n+3)>>8)&0xff;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length_n+3)&0xff;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 6;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = group;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = i+1;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4 + i*eachnum],length_n);
            num += length_n;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
    }
    else
    {
            Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/��Կ��
static void Encrypt_AppProcess_M_FUN46(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1c;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    length = (Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1];
    Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
    Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],length);
    num += length;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;	
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
    Encrypt_ReturnFaultInfo(0x47,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//����/����/����
static void Encrypt_AppProcess_M_FUN48(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x08&_ENCRYPT_ENC));
    Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x49;//Ӧ������

    num=0;
    length = (Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1];
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((8+8+16+length)>>8)&0xff;
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((8+8+16+length))&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].M_ID,8);
    num += 8;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,8);
    num += 8;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
    num += 16;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],length);
    num += length;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data,&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((length+2)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (length+2)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����
static void Encrypt_AppProcess_I_FUN00(BYTE pdrv)
{
    Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC���� Ӧ��������
    memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);///101���� ->IEC
}

//����/����/����/����/ǩ�����/��Կ��ʶ
static void Encrypt_AppProcess_I_FUN01(BYTE pdrv)
{
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//ȡ�����
        
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            memcpy(Encrypt_App[pdrv].R_NUM,&Encrypt_App[pdrv].Enc_rxbuff[4],8);
            Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC����
            memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/����/����/ǩ�����/��Կ��ʶ
static void Encrypt_AppProcess_I_FUN03(BYTE pdrv)
{	    
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC����
        memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/����/����/ǩ�����/��Կ��ʶ
static void Encrypt_AppProcess_I_FUN05(BYTE pdrv)
{    
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);///800800 �����оƬ��������
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC����
        memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/����/����/ǩ�����/��Կ��ʶ
static void Encrypt_AppProcess_I_FUN07(BYTE pdrv)
{	    
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC����
        memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/�����
static void Encrypt_AppProcess_G_FUN20(BYTE pdrv)
{
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x16;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],8);
    num += 8;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x21;//Ӧ������
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x40+0x08+1;//��Ϣ��ȫ��չ��len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x40+0x08);
        num += (0x40+0x08);
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x01;//ǩ����Կ��ʶ
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/ǩ�����/��Կ��ʶ
static void Encrypt_AppProcess_G_FUN22(BYTE pdrv)
{
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x18;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x40;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],0x40);
    num += 0x40;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_G_CERSUCCESS;
    }

    Encrypt_ReturnFaultInfo(0x23,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//����/����/�����
static void Encrypt_AppProcess_B_FUN50(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x16;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],8);
    num += 8;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x51;//Ӧ������
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x40+0x08+1;//��Ϣ��ȫ��չ��len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x40+0x08);
        num += (0x40+0x08);
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x01;//ǩ����Կ��ʶ
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/ǩ�����/��Կ��ʶ
static void Encrypt_AppProcess_B_FUN52(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x18;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x40;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],0x40);
    num += 0x40;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_G_CERSUCCESS;
    }
    Encrypt_ReturnFaultInfo(0x53,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//����/����
static void Encrypt_AppProcess_B_FUN54(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x99;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x55;//Ӧ������
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x08;//��Ϣ��ȫ��չ��len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
        num += 0x08;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//����/����
static void Encrypt_AppProcess_B_FUN60(BYTE pdrv)
{
    BYTE keyversion;
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1A;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        keyversion = Encrypt_App[pdrv].Enc_txbuff[4];
    
        Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x61;//Ӧ������
            num=0;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x09;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = keyversion;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
            num += 0x08;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }		
}

//����/����/��Կ��
static void Encrypt_AppProcess_B_FUN62(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    length = ((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1]);
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1C;
    if(Encrypt_App[pdrv].RxTemp.Data.buff[2])
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;}
    else
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;}
    Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[(length-1)+2];//����-1�������һλ��-2���ĳ���
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1)>>8)&0xff;//-1ASKID
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1))&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],(length-1));
    num += (length-1);
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_B_CERSUCCESS;
    }
    
    Encrypt_ReturnFaultInfo(0x63,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//����/����/��Կ��
static void Encrypt_AppProcess_B_FUN64(BYTE pdrv)
{
    WORD num=0;
    WORD length=0;

    length = ((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1]);
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1C;
    if(Encrypt_App[pdrv].RxTemp.Data.buff[2])
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;}
    else
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;}
    Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[(length-1)+2];//����-1�������һλ��-2���ĳ���
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1)>>8)&0xff;//-1ASKID
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1))&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],(length-1));
    num += (length-1);
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
    Encrypt_ReturnFaultInfo(0x65,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//����/����/֤���ʶ/��֡��/��ǰ֡���/֤���ļ�N
static void Encrypt_AppProcess_B_FUN70(BYTE pdrv)
{
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {    
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//��һ���建����
        {
            memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
        }

        num_n = 0;
        while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����δ�洢λ��
        {
            if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//�����֡���Ѵ��ڣ�������
            {break;}
            length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
            num_n = num_n+length_n+3;
            num_max++;
        }
        
        length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-3)-6-64-1;//�洢
        if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
        {
            Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
            Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
            Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
            memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
        }
        
        if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//�Ѵ洢��������Ҫ��
        {			
            memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
            num=0;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[2];
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
            for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//���
            {
                num_n = 0;length_n = 0;
                while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����λ��
                {
                    length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                    if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                    {
                        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                        break;
                    }
                    num_n = num_n+length_n+3;
                    if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                    {break;}
                }
                length += length_n;
            }
            Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
            Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
            num += length;
            Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
            
            Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//����֤��
            
            Encrypt_ReturnFaultInfo(0x71,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/֤���ʶ/��֡��/��ǰ֡���/֤���ļ�N
static void Encrypt_AppProcess_B_FUN72(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//��һ���建����
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����δ�洢λ��
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//�����֡���Ѵ��ڣ�������
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
    
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-3);//�洢
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
    }
    
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//�Ѵ洢��������Ҫ��
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x24;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//���
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//����λ��
            {
                length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                {
                    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                    break;
                }
                num_n = num_n+length_n+3;
                if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//����֤��
        
        Encrypt_ReturnFaultInfo(0x73,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//����/����/֤���ʶ/��֡��/��ǰ֡���/�ն�֤��N
static void Encrypt_AppProcess_B_FUN74(BYTE pdrv)
{	
    WORD eachnum=0xf0;
    WORD group=0;
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    BYTE i;
	
    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x30;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
        
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {		
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        group = (length%eachnum) ? (length/eachnum + 1)  : (length/eachnum);
        Encrypt_StatusReset(pdrv);
        for(i=0;i<group;i++)
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x75;//Ӧ������
            num=0;
            length_n = ((i+1) == group) ? (length%eachnum)  : eachnum;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = ((length_n+3)>>8)&0xff;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length_n+3)&0xff;//��Ϣ��ȫ��չ��len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 6;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = group;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = i+1;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4 + i*eachnum],length_n);
            num += length_n;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
    }
    else
    {
            Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

void Encrypt_AppProcess(BYTE pdrv)
{		
	switch (Encrypt_App[pdrv].RxTemp.Data.Type)
	{
        //IEC    
        case _ENCRYPT_I_FUN00: // Ӧ������Ϊ0
            Encrypt_AppProcess_I_FUN00(pdrv);
        break;
        
        case _ENCRYPT_I_FUN01: // Ӧ������Ϊ1
            Encrypt_AppProcess_I_FUN01(pdrv);
        break;
                
        case _ENCRYPT_I_FUN03: // Ӧ������Ϊ3
            Encrypt_AppProcess_I_FUN03(pdrv);
        break;
        
        case _ENCRYPT_I_FUN05: // Ӧ������Ϊ5
            Encrypt_AppProcess_I_FUN05(pdrv);
        break;
                
        case _ENCRYPT_I_FUN07: // Ӧ������Ϊ7
            Encrypt_AppProcess_I_FUN07(pdrv);
        break;
        
        case _ENCRYPT_I_FUN08: // Ӧ������Ϊ8
//            Encrypt_AppProcess_I_FUN08(pdrv);
        break;
        
        //����  
        case _ENCRYPT_G_FUN20: // ˫�������֤����
            Encrypt_AppProcess_G_FUN20(pdrv);
            Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_G_CERSUCCESS);
        break;
        
        case _ENCRYPT_G_FUN22: // ���ն���֤������Ӧ
            Encrypt_AppProcess_G_FUN22(pdrv);
        break;

        //��վ  
        case _ENCRYPT_B_FUN50: // ˫�������֤����
            Encrypt_AppProcess_B_FUN50(pdrv);
            Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_B_CERSUCCESS);
        break;
        
        case _ENCRYPT_B_FUN52: // ˫�������֤����
            Encrypt_AppProcess_B_FUN52(pdrv);
        break;
          
        case _ENCRYPT_B_FUN54: // ��ȡ�ն�оƬ���к�
            Encrypt_AppProcess_B_FUN54(pdrv);
        break;
        
        case _ENCRYPT_B_FUN60: // ��ȡ�ն���Կ�汾
            Encrypt_AppProcess_B_FUN60(pdrv);
        break;
        
        case _ENCRYPT_B_FUN62: // ִ��Զ����Կ����
            Encrypt_AppProcess_B_FUN62(pdrv);
        break;
        
        case _ENCRYPT_B_FUN64: // ִ��Զ����Կ�ָ�
            Encrypt_AppProcess_B_FUN64(pdrv);
        break;
        
        case _ENCRYPT_B_FUN70: // ֤��Զ�̸���
            Encrypt_AppProcess_B_FUN70(pdrv);
        break;
  
        case _ENCRYPT_B_FUN72: // �ն�֤��Զ�̸���
            Encrypt_AppProcess_B_FUN72(pdrv);
        break;
        
        case _ENCRYPT_B_FUN74: // ��ȡ�ն�֤��
            Encrypt_AppProcess_B_FUN74(pdrv);
        break;
        
        //֤�������     
        case _ENCRYPT_M_FUN30: // �·���֤����
            Encrypt_AppProcess_M_FUN30(pdrv);
            Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_B_CERSUCCESS);
        break;

        case _ENCRYPT_M_FUN32: // ���ն���֤������Ӧ
            Encrypt_AppProcess_M_FUN32(pdrv);
        break;
        
        case _ENCRYPT_M_FUN34: // ��ȡ�ն���Կ�汾
            Encrypt_AppProcess_M_FUN34(pdrv);
        break;

        case _ENCRYPT_M_FUN36: // ��ȡ�ն���Կ�汾
            Encrypt_AppProcess_M_FUN36(pdrv);
        break;
        
        case _ENCRYPT_M_FUN38: // ��ȡ�ն����к�
            Encrypt_AppProcess_M_FUN38(pdrv);
        break;
        
        case _ENCRYPT_M_FUN3A: // ��ȡ��ȫоƬ��Կ
            Encrypt_AppProcess_M_FUN3A(pdrv);
        break;
        
        case _ENCRYPT_M_FUN3C: // ��֤�������ļ�����ǩ��
            Encrypt_AppProcess_M_FUN3C(pdrv);
        break;
                    
        case _ENCRYPT_M_FUN3E: // ��֤�鵼���ն�
            Encrypt_AppProcess_M_FUN3E(pdrv);
        break;
        
        case _ENCRYPT_M_FUN40: // �ն˳�ʼ֤���д
            Encrypt_AppProcess_M_FUN40(pdrv);
        break;

        case _ENCRYPT_M_FUN42: // �����ն�֤��
            Encrypt_AppProcess_M_FUN42(pdrv);
        break;

        case _ENCRYPT_M_FUN46: // �ָ��ն���Կ
            Encrypt_AppProcess_M_FUN46(pdrv);
        break;

        case _ENCRYPT_M_FUN48: // ��ȡ�����ն����ݴ���
            Encrypt_AppProcess_M_FUN48(pdrv);
        break;
        
        default: // ���ͱ�ʶ�д����֧��
        break;
	}
}

static void Encrypt_DecodeFrame(BYTE pdrv)
{
    Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_SANDABLE;///0x00010000//���ܳ���ɷ�������
    Encrypt_App[pdrv].Object = Encrypt_App[pdrv].RxdBuf[5]&_ENCRYPT_OBJ;///���Ӷ�����վ00����ά����01������10��11���ã�
    
    Encrypt_App[pdrv].RxTemp.Head.MesType_H = Encrypt_App[pdrv].RxdBuf[4];///��������
    Encrypt_App[pdrv].RxTemp.Head.MesType_L = Encrypt_App[pdrv].RxdBuf[5];

    if (Encrypt_App[pdrv].RxTemp.Head.MesType_L & _ENCRYPT_ENC) // ����
    {
        if(!Encrypt_Ciphertext(pdrv))
        {return;}
    }
    else  // ����
    {
        if(!Encrypt_EncryptCheck(pdrv))
        {return;}
    }
    Encrypt_AppProcess(pdrv);///�Ӽ���оƬȡ�������ݽ��д��� �ٷ��͸�����оƬ����--
}

void Encrypt_SearchValidFrame(BYTE pdrv)
{
    BYTE stop = 0;
    BYTE length = 0;
    
    if(!(Encrypt_App[pdrv].LinkFlag&_ENCRYPT_INITIAL))///���δ��ʼ�� ��ʼ��
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_INITIAL;///0x00800000���ܳ����ʼ��
        Encrypt_StatusReset(pdrv);
    }	
		
    while ((Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail) && (!stop))
    {
        if (Encrypt_App[pdrv].RxdStatus == RXDSTART)
        {
            while ((Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead] != _ENCRYPT_STARTCODE)
                    && (Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail))
            {
                Encrypt_App[pdrv].RxdHead++;
            }
				///�ҵ�����0xEB
            if (Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail) // �ҵ������ַ������������뻺�������롣
            {
                Encrypt_App[pdrv].RxdStatus = RXDHEAD;
                if (Encrypt_App[pdrv].RxdHead != 0)
                {
                    memcpy(Encrypt_App[pdrv].RxdBuf,Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead,Encrypt_App[pdrv].RxdTail-Encrypt_App[pdrv].RxdHead);
                    Encrypt_App[pdrv].RxdTail -= Encrypt_App[pdrv].RxdHead;
                    Encrypt_App[pdrv].RxdHead = 0;
                }
            }
        }
        
        Encrypt_App[pdrv].RxdLength = Encrypt_App[pdrv].RxdTail - Encrypt_App[pdrv].RxdHead;///�����ݳ���
        if (Encrypt_App[pdrv].RxdStatus == RXDHEAD) // ���Ĺ�һ֡����
        {
            switch (Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead])///����ͷ
            {
                case _ENCRYPT_STARTCODE://0xEB
                    if (Encrypt_App[pdrv].RxdLength >= (3+6)) // ���ĳ�������9���ֽڡ�
                    {
                        if (!Encrypt_CheckFramValid(pdrv)) // �ж��Ƿ���ͷ
                        {
                            Encrypt_App[pdrv].RxdHead++;//���Ǳ���ͷ���������»�ȡ
                            Encrypt_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            Encrypt_App[pdrv].RxdStatus = RXDCONTINUE;///�Ǳ���ͷ ���һ������
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
            }
        }
				
        if (Encrypt_App[pdrv].RxdStatus == RXDCONTINUE)
        {
            switch (Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead])
            {
              case _ENCRYPT_STARTCODE:/// EB ���� ����L EB  ....... CS 16
                length = ((Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+1]<<8)|(Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+2]));
                if (Encrypt_App[pdrv].RxdLength >= length+6) // ������ȫ�����д���
                {
                    if ((Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+length+4] == (Encrypt_CKS(pdrv, Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead)&0xFF))//У����ȷ
                            && (Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+length+4+1] == _ENCRYPT_ENDCODE)) // ��������ȷ
 					          {
                        if ( Encrypt_App[pdrv].RxdHead > 0 )
                        {
                            memcpy(Encrypt_App[pdrv].RxdBuf,Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead,Encrypt_App[pdrv].RxdTail-Encrypt_App[pdrv].RxdHead);//�����뻺��������
                            Encrypt_App[pdrv].RxdTail -= Encrypt_App[pdrv].RxdHead;
                            Encrypt_App[pdrv].RxdHead = 0;
                        }
												/* ͷָ����Ƶ����ĺ� */
                        Encrypt_App[pdrv].RxdHead += (Encrypt_App[pdrv].RxdBuf[1]+6);
                        Encrypt_App[pdrv].RxdStatus = RXDSTART;

                        Encrypt_DecodeFrame(pdrv);///���ݽ���  ��������оƬ���ݲ��õ�����
                    }
                    else
                    {
                        Encrypt_App[pdrv].RxdHead += 6;
                        Encrypt_App[pdrv].RxdStatus = RXDSTART;
                    }
                }
                else // ����δ��ȫ�������������ȴ����ݡ�
                {
                    stop = 1;
                }
                break;
            }
        }
    }
}

WORD Encrypt_ReadMISIData(BYTE *pbuf, BYTE pdrv)
{
    WORD count = 0;
    WORD len = 0;
    
    if (Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail) // ���ջ�����������δ���������
    {
        if (Encrypt_App[pdrv].RxdHead != 0)
        {
            memcpy(Encrypt_App[pdrv].RxdBuf, Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead, Encrypt_App[pdrv].RxdTail-Encrypt_App[pdrv].RxdHead);//��δ����������Ƶ�������ͷ
            Encrypt_App[pdrv].RxdTail -= Encrypt_App[pdrv].RxdHead;
            Encrypt_App[pdrv].RxdHead = 0;
        }
    }
    else
    {
        Encrypt_App[pdrv].RxdHead = 0;
		Encrypt_App[pdrv].RxdTail = 0;
    }
    ///ע�� ������ ֱ�ӴӴ��ڶ�������
    count = Encrypt_ReadData((BYTE *)&Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdTail], _ENCRYPT_LPDUSIZE, pdrv);
    if (count > 0)
    {
        Encrypt_App[pdrv].RxdTail += count;///βָ�����
        Encrypt_App[pdrv].IECLength = 0;///IEC���ݳ�����0
        memset(Encrypt_App[pdrv].IECBuf,0,sizeof(Encrypt_App[pdrv].IECBuf));///IEC���ջ�������0
			
        Encrypt_SearchValidFrame(pdrv);///�����жϴ��� ��������оƬ���ݲ���ȡ
			
        len = Encrypt_App[pdrv].IECLength;//IEC���� 101����
        memcpy(pbuf,Encrypt_App[pdrv].IECBuf,len);///101���ݷ��� pbuf��
    }
		
		return(len);
}

BYTE Encrypt_ENCSendData(BYTE pdrv)
{
    BYTE res = FALSE;
    WORD length;
		///��ʼ���͸�����оƬ��������ʱ��1������ָ��++ �����������ݽ���
    if((Encrypt_App[pdrv].StoreDATA1OUT != Encrypt_App[pdrv].StoreDATA1IN)&&(Encrypt_App[pdrv].LinkFlag&_ENCRYPT_SANDABLE))
    {
        Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_SANDABLE);
        length = ((Encrypt_App[pdrv].StoreDATA1OUT->status.Length_H<<8)|Encrypt_App[pdrv].StoreDATA1OUT->status.Length_L);
        memcpy(&Encrypt_App[pdrv].TxTemp,Encrypt_App[pdrv].StoreDATA1OUT,length+2);
        Encrypt_PackSend(pdrv);
        
        memset((BYTE *)Encrypt_App[pdrv].StoreDATA1OUT,0,sizeof(_ENCRYPT_LPDUSIZE));
        if(++Encrypt_App[pdrv].StoreDATA1OUT >= Encrypt_App[pdrv].StoreDATA1Buf + _ENCRYPT_STOREDATA1NUM)
        {
            Encrypt_App[pdrv].StoreDATA1OUT = Encrypt_App[pdrv].StoreDATA1Buf;
        }
        res = TRUE;
    }

    return(res);
}

BYTE Encrypt_AppInit(BYTE pdrv)
{   
    if (Encrypt_App == RT_NULL)
	{
		Encrypt_App = rt_malloc(sizeof(ENCRYPT_APPINFO) * _ENCRYPT_VOLUMES);
	}

    memset (&Encrypt_App[pdrv], 0, sizeof (ENCRYPT_APPINFO));
	  
    return(TRUE);
}

BYTE Encrypt_IECSendData(BYTE *pbuf, WORD count, BYTE type, BYTE pdrv)
{
    BYTE res = FALSE;
    
    if(Encrypt_App[pdrv].LinkFlag&0x00810000)///��̨��֤()_ENCRYPT_B_CERSUCCESS 0x40000000  change by chen
    {
        Encrypt_App[pdrv].TxTemp.status.Length_H = ((count+1+1+2)>>8)&0xff;///���ݳ���    û����Ϣ��ȫ��չ����
        Encrypt_App[pdrv].TxTemp.status.Length_L = ((count+1+1+2))&0xff;
        Encrypt_App[pdrv].TxTemp.Head.MesType_H = 0x00;///��������
        Encrypt_App[pdrv].TxTemp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|((type>>4)&_ENCRYPT_ENC));
        Encrypt_App[pdrv].TxTemp.Data.Type = type&0x7f;
        Encrypt_App[pdrv].TxTemp.Data.buff[0] = count;
        memcpy(&Encrypt_App[pdrv].TxTemp.Data.buff[1],pbuf,count);
        Encrypt_PackSend(pdrv);
        res = TRUE;
    }

    return(res);
}


