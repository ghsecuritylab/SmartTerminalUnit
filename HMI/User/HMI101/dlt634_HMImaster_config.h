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
** File name:               dlt634_HMImaster_config.h
** Descriptions:            The application layer of DL/T634.HMI_2002
**
** -----------------------------------------------------------------------------
** Created by:              Mr.j
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
#ifndef	_DLT634_HMIMASTER_CONFIG_H_
#define _DLT634_HMIMASTER_CONFIG_H_


/* PUBLIC VARIABLE -----------------------------------------------------------*/
enum
{
    DLT634_HMIMASTER_DISK0 = 0,
    _DLT634_HMIMASTER_VOLUMES
};
/* Number of volumes (logical drives) to be used.*/
#define _DLT634_HMIMASTER_LPDUSIZE  1024

#define  _DLT634_HMIMASTER_NUMOF_MAXRETRY                0x03 // Ĭ��DL/T634.HMI_2002��Լ��·����������Ӵ���

//LINK_FLAG
#define  _DLT634_HMIMASTER_REQSTATUS                   0x00000001
#define  _DLT634_HMIMASTER_RECONFIRM                   0x00000002
#define  _DLT634_HMIMASTER_ASKSTATUS               		0x00000004
#define  _DLT634_HMIMASTER_ASKRELINK               		0x00000008
#define  _DLT634_HMIMASTER_INITEND                 		0x00001000

#define  _DLT634_HMIMASTER_SENDABLE                   0x10000000// ƽ��

//APP_FLAG
#define  _DLT634_HMIMASTER_CALLALLDATA                0x00000001

/*************************************************************************************************************************************/
//DL/T634.HMI_2002��Լ

/* ��·��֡���δ�***************************************************************/
#define  _DLT634_HMIMASTER_STARTCODE10	                     0x10	// ��ʼ��
#define  _DLT634_HMIMASTER_STARTCODE68	                     0x68	// ��ʼ��
#define  _DLT634_HMIMASTER_ENDCODE                           0x16	// ��ֹ��

/* ��·���������************************************************************/
//ǰ4λ
#define  _DLT634_HMIMASTER_FUNCODE                       0x0F // ������
#define  _DLT634_HMIMASTER_FCV                           0x10 // ֡������Чλ 
#define  _DLT634_HMIMASTER_DFC                           0x10 // ����������
#define  _DLT634_HMIMASTER_FCB                           0x20 // ֡����λ
#define  _DLT634_HMIMASTER_ACD                           0x20 // Ҫ�����λ 
#define  _DLT634_HMIMASTER_PRM                           0x40 // ��������λ
#define  _DLT634_HMIMASTER_DIR                           0x80 // ���䷽��λ

//��·����վ����վ���书���붨��
#define   _DLT634_HMIMASTER_M_FUN0             	0           // ��λԶ����·
#define   _DLT634_HMIMASTER_M_FUN2              2           // ������·���� 
#define   _DLT634_HMIMASTER_M_FUN3              3	          // ����ȷ������
#define   _DLT634_HMIMASTER_M_FUN4             	4	          // ���Ͳ�ȷ������
#define   _DLT634_HMIMASTER_M_FUN8              8	          // ������Ӧȷ������״̬
#define   _DLT634_HMIMASTER_M_FUN9             	9	          // �ٻ���·״̬
#define   _DLT634_HMIMASTER_M_FUN10             10	        // �ٻ�1���û����� 
#define   _DLT634_HMIMASTER_M_FUN11             11          // �ٻ�2���û�����

//*��·����վ����վ���书���붨�� 
#define   _DLT634_HMIMASTER_S_FUN0              0            // ȷ��
#define   _DLT634_HMIMASTER_S_FUN1              1            // ȷ����·æδ���ձ���
#define   _DLT634_HMIMASTER_S_FUN8              8            // ��������Ӧ����֡
#define   _DLT634_HMIMASTER_S_FUN9              9            // �����ٻ�������
#define   _DLT634_HMIMASTER_S_FUN11             11           // ��Ӧ��·״̬��ش�����֡
 
/* TI���ͱ�ʶ ********************************************************************************/ 
//���ӷ��������Ϣ
#define   _DLT634_HMIMASTER_M_SP_NA_1              1            // ������Ϣ
#define   _DLT634_HMIMASTER_M_DP_NA_1              3            // ˫����Ϣ
#define   _DLT634_HMIMASTER_M_ME_NA_1              9            // ����ֵ����һ��ֵ
#define   _DLT634_HMIMASTER_M_ME_NB_1              11           // ����ֵ����Ȼ�ֵ
#define   _DLT634_HMIMASTER_M_ME_NC_1              13           // ����ֵ���̸�����
#define   _DLT634_HMIMASTER_M_SP_TB_1              30           // ��ʱ��ĵ�����Ϣ
#define   _DLT634_HMIMASTER_M_DP_TB_1              31           // ��ʱ���˫����Ϣ
#define   _DLT634_HMIMASTER_M_FT_NA_1              42           // �����¼���Ϣ
#define   _DLT634_HMIMASTER_M_IT_NB_1              206          // �ۼ������̸�����
#define   _DLT634_HMIMASTER_M_IT_TC_1              207          // ��ʱ���ۼ������̸�����

//���Ʒ��������Ϣ
#define   _DLT634_HMIMASTER_C_SC_NA_1              45            // ��������
#define   _DLT634_HMIMASTER_C_SC_NB_1              46            // ˫������

//���ӷ����ϵͳ����
#define   _DLT634_HMIMASTER_M_EI_NA_1              70            // ��ʼ������

//���Ʒ����ϵͳ����
#define   _DLT634_HMIMASTER_C_IC_NA_1              100            // վ���ٻ�����
#define   _DLT634_HMIMASTER_C_CI_NA_1              101            // �������ٻ�����
#define   _DLT634_HMIMASTER_C_CS_NA_1              103            // ʱ��ͬ������
#define   _DLT634_HMIMASTER_C_TS_NA_1              104            // ��������
#define   _DLT634_HMIMASTER_C_RP_NA_1              105            // ��λ��������
#define   _DLT634_HMIMASTER_C_SR_NA_1              200            // �л���ֵ��
#define   _DLT634_HMIMASTER_C_RR_NA_1              201            // ����ֵ����
#define   _DLT634_HMIMASTER_C_RS_NA_1              202            // �������Ͷ�ֵ
#define   _DLT634_HMIMASTER_C_WS_NA_1              203            // д�����Ͷ�ֵ
#define   _DLT634_HMIMASTER_F_FR_NA_1              210            // �ļ�����
#define   _DLT634_HMIMASTER_F_SR_NA_1              211            // �������

/* COT����ԭ�� ********************************************************************************/ 
#define   _DLT634_HMIMASTER_COT_CYC              1            // ����ѭ��
#define   _DLT634_HMIMASTER_COT_BACK             2            // ����ɨ��
#define   _DLT634_HMIMASTER_COT_SPONT            3            // ͻ��
#define   _DLT634_HMIMASTER_COT_INIT             4            // ��ʼ��
#define   _DLT634_HMIMASTER_COT_REQ              5            // ���������
#define   _DLT634_HMIMASTER_COT_ACT              6            // ����
#define   _DLT634_HMIMASTER_COT_ACTCON           7            // ����ȷ��
#define   _DLT634_HMIMASTER_COT_DEACT            8            // ֹͣ����
#define   _DLT634_HMIMASTER_COT_DEACTCON         9            // ֹͣ����ȷ��
#define   _DLT634_HMIMASTER_COT_ACCTTERM         10            // ������ֹ
#define   _DLT634_HMIMASTER_COT_FILE             13            // �ļ�����
#define   _DLT634_HMIMASTER_COT_INTROGEN         20            // ��Ӧվ�ٻ�
#define   _DLT634_HMIMASTER_COT_REQCOGEN         37            // ��Ӧ�����ٻ�

#endif /* END _DLT634_HMI_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/
