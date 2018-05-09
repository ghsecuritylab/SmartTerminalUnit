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
** File name:               encrypt_config.h
** Descriptions:            The config of encrypt
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
#ifndef	_ENCRYPT_CONFIG_H_
#define _ENCRYPT_CONFIG_H_


/* PUBLIC VARIABLE -----------------------------------------------------------*/
typedef enum
{
    ENCRYPT_D0 = 0,
    ENCRYPT_D1,
    _ENCRYPT_VOLUMES
}ENCRYPT_DISK;

#define _ENCRYPT_LPDUSIZE  512
#define _ENCRYPT_STOREDATA1NUM	10

/* ��·��֡���δ�***************************************************************/
#define  _ENCRYPT_STARTCODE  	                     0xEB	// ��ʼ��
#define  _ENCRYPT_ENDCODE                          0xD7	// ��ֹ��

#define  _ENCRYPT_OBJ                              0xc0 // ���Ӷ���
#define  _ENCRYPT_ENC                              0x08 // ���ܱ�ʶ

#define  _ENCRYPT_OBJ_B                              0x00 // ��վ
#define  _ENCRYPT_OBJ_M                              0x40 // �ֳ���ά����
#define  _ENCRYPT_OBJ_G                              0x80 // ����

//LINK_FLAG
#define  _ENCRYPT_INITIAL                	           0x00800000//���ܳ����ʼ��
#define  _ENCRYPT_SANDABLE                	           0x00010000//���ܳ���ɷ�������

#define  _ENCRYPT_G_CERSUCCESS                  	   0x80000000//������֤�ɹ�
#define  _ENCRYPT_B_CERSUCCESS                  	   0x40000000//��̨��֤�ɹ�
#define  _ENCRYPT_M_CERSUCCESS                  	   0x20000000//֤���������֤�ɹ�

//Ӧ������IEC/Gateway/Backstage/Maintain
//ҵ��ȫ����������	
#define   _ENCRYPT_S_FUN1F             	0x1F	         // ҵ��ȫ����������

//IEC																									 // Ӧ������/ʱ��(6)/�����(8)/ǩ�����(65)
#define   _ENCRYPT_I_FUN00             	0x00           // ��      /����   /����     /����
#define   _ENCRYPT_I_FUN01              0x01           // ��      /����   /����     /��
#define   _ENCRYPT_I_FUN02              0x02	         // ��      /����   /��       /����
#define   _ENCRYPT_I_FUN03             	0x03	         // ��      /����   /��       /��
#define   _ENCRYPT_I_FUN04             	0x04           // ��      /��     /����     /����
#define   _ENCRYPT_I_FUN05              0x05           // ��      /��     /����     /��
#define   _ENCRYPT_I_FUN06              0x06	         // ��      /��     /��       /����
#define   _ENCRYPT_I_FUN07             	0x07	         // ��      /��     /��       /��
#define   _ENCRYPT_I_FUN08              0x08	         // ����    /��     /��       /��

//����
#define   _ENCRYPT_G_FUN20             	0x20           // ��֤���ض��ն˵���֤����
#define   _ENCRYPT_G_FUN21              0x21           // �ն���֤ȷ�ϲ�����������֤
#define   _ENCRYPT_G_FUN22              0x22	         // ���ض��ն���֤�������Ӧ
#define   _ENCRYPT_G_FUN23             	0x23	         // �ն������ط�����֤���

//��̨
#define   _ENCRYPT_B_FUN50             	0x50           // ��վ���ն˵���֤����
#define   _ENCRYPT_B_FUN51              0x51           // �ն���֤ȷ�ϲ�������վ��֤
#define   _ENCRYPT_B_FUN52              0x52	         // ��վ���ն���֤�������Ӧ
#define   _ENCRYPT_B_FUN53             	0x53	         // �ն�����վ������֤���
#define   _ENCRYPT_B_FUN54              0x54	         // ��վ��ȡ�ն�оƬ���к�
#define   _ENCRYPT_B_FUN55             	0x55	         // �ն˷�������оƬ���к�

#define   _ENCRYPT_B_FUN60             	0x60           // ��վ��ȡ�ն���Կ�汾
#define   _ENCRYPT_B_FUN61             	0x61           // �ն�����վ������Կ�汾
#define   _ENCRYPT_B_FUN62             	0x62           // ��վִ��Զ����Կ����
#define   _ENCRYPT_B_FUN63             	0x63           // �ն�����վ������Կ���½��
#define   _ENCRYPT_B_FUN64             	0x64           // ��վִ��Զ����Կ�ָ�
#define   _ENCRYPT_B_FUN65             	0x65           // �ն�����վ������Կ�ָ����

#define   _ENCRYPT_B_FUN70             	0x70           // ��վ���ն˷���֤��Զ�̸���
#define   _ENCRYPT_B_FUN71             	0x71           // �ն�����վ����֤��Զ�̸��½��
#define   _ENCRYPT_B_FUN72             	0x72           // ��վ���ն˷���Զ�������ն�֤��
#define   _ENCRYPT_B_FUN73             	0x73           // �ն�����վ����֤��Զ�����ؽ��
#define   _ENCRYPT_B_FUN74             	0x74           // ��վ��ȡ�ն�֤��
#define   _ENCRYPT_B_FUN75             	0x75           // �ն˷���֤��
#define   _ENCRYPT_B_FUN76             	0x76           // ��վ���ն˷���֤����ȡ���

//֤�������
#define   _ENCRYPT_M_FUN30             	0x30           // ֤������߷�����֤����
#define   _ENCRYPT_M_FUN31              0x31           // �ն���Ӧ֤������ߵ���֤����
#define   _ENCRYPT_M_FUN32              0x32	         // ֤������߶��ն���֤�������Ӧ
#define   _ENCRYPT_M_FUN33             	0x33	         // �ն���֤������߷�����֤���

#define   _ENCRYPT_M_FUN34              0x34	         // ��ȡ�ն���Կ�汾
#define   _ENCRYPT_M_FUN35             	0x35	         // �ն˷�����Կ�汾

#define   _ENCRYPT_M_FUN36              0x36	         // ��ȡ�ն����к�
#define   _ENCRYPT_M_FUN37              0x37           // �ն˷����ն����к�

#define   _ENCRYPT_M_FUN38              0x38	         // ��ȡ�ն����ð�ȫоƬ���к�
#define   _ENCRYPT_M_FUN39              0x39           // �ն˷������ð�ȫоƬ���к�

#define   _ENCRYPT_M_FUN3A              0x3A	         // ��ȡ�ն˰�ȫоƬ��Կ
#define   _ENCRYPT_M_FUN3B              0x3B           // �ն˷����ն˰�ȫоƬ��Կ���ܱ���

#define   _ENCRYPT_M_FUN3C              0x3C	         // ֤������߽����ɵ�֤�������ļ������ն�
#define   _ENCRYPT_M_FUN3D              0x3D           // ��֤������߷���ǩ�����

#define   _ENCRYPT_M_FUN3E              0x3E	         // ֤������߽����Ӧ��֤�鵼���ն�
#define   _ENCRYPT_M_FUN3F              0x3F           // �ն˷���֤�鵼����

#define   _ENCRYPT_M_FUN40              0x40	         // �ն˳�ʼ֤���д
#define   _ENCRYPT_M_FUN41              0x41           // �ն˷��س�ʼ֤���д���

#define   _ENCRYPT_M_FUN42              0x42	         // �����ն�֤��
#define   _ENCRYPT_M_FUN43              0x43           // �ն���֤������߷����ն�֤��
#define   _ENCRYPT_M_FUN44              0x44           // ֤����������ն˷���֤�鵼�����

#define   _ENCRYPT_M_FUN45              0x45           // ��֡���ݴ���������

#define   _ENCRYPT_M_FUN46              0x46           // ֤������߻ָ��ն���Կ
#define   _ENCRYPT_M_FUN47              0x47           // ��Կ�ָ����

#define   _ENCRYPT_M_FUN48              0x48           // ֤����������ֳ���ά������Կ��������ݼ��ܲ�����
#define   _ENCRYPT_M_FUN49              0x49           // �ն����ֳ���ά������Կ��������ݼ��ܲ�����

/* �����붨�� ********************************************************************************/ 
//ҵ����չ������
#define   _ENCRYPT_ERROR_CODE9101      9101					// ҵ��Ӧ�����ʹ���
#define   _ENCRYPT_ERROR_CODE9102      9102					// ������ǩʧ��
#define   _ENCRYPT_ERROR_CODE9103      9103					// ���Ľ���ʧ��
#define   _ENCRYPT_ERROR_CODE9104      9104					// �������֤ʧ��
#define   _ENCRYPT_ERROR_CODE9105      9105					// ʱ��У��ʧ��
#define   _ENCRYPT_ERROR_CODE9106      9106					// ��Ҫ���ܱ���δ����
#define   _ENCRYPT_ERROR_CODE9107      9107					// ҵ��ȫ���̷Ƿ�
#define   _ENCRYPT_ERROR_CODE9108      9108					// Ȩ�޲���
#define   _ENCRYPT_ERROR_CODE9109      9109					// δ֪����
#define   _ENCRYPT_ERROR_CODE9110      9110					// ���ĳ�����������ʧ��

//��ȫ��չ������
#define   _ENCRYPT_ERROR_CODE9000      9000					// �ɹ�
#define   _ENCRYPT_ERROR_CODE9090      9090					// ��֤ʧ��
#define   _ENCRYPT_ERROR_CODE9091      9091					// ��Կ����ʧ��
#define   _ENCRYPT_ERROR_CODE9092      9092					// ��Կ�ָ�ʧ��
#define   _ENCRYPT_ERROR_CODE9093      9093					// ֤�鵼��ʧ��
#define   _ENCRYPT_ERROR_CODE9094      9094					// ֤�鵼��ʧ��
#define   _ENCRYPT_ERROR_CODE9095      9095					// ֤����ȡʧ��
#define   _ENCRYPT_ERROR_CODE9096      9096					// ��֡���ݽ���ʧ��
#define   _ENCRYPT_ERROR_CODE9097      9099					// ֤��Զ�̸�������ʧ��

/* TI���ͱ�ʶ ********************************************************************************/ 
//���ӷ��������Ϣ
#define   _ENCRYPT_M_SP_NA_1              1            // ������Ϣ
#define   _ENCRYPT_M_DP_NA_1              3            // ˫����Ϣ
#define   _ENCRYPT_M_ME_NA_1              9            // ����ֵ����һ��ֵ
#define   _ENCRYPT_M_ME_NB_1              11           // ����ֵ����Ȼ�ֵ
#define   _ENCRYPT_M_ME_NC_1              13           // ����ֵ���̸�����
#define   _ENCRYPT_M_SP_TB_1              30           // ��ʱ��ĵ�����Ϣ
#define   _ENCRYPT_M_DP_TB_1              31           // ��ʱ���˫����Ϣ
#define   _ENCRYPT_M_FT_NA_1              42           // �����¼���Ϣ
#define   _ENCRYPT_M_IT_NB_1              206          // �ۼ������̸�����
#define   _ENCRYPT_M_IT_TC_1              207          // ��ʱ���ۼ������̸�����

//���Ʒ��������Ϣ
#define   _ENCRYPT_C_SC_NA_1              45            // ��������
#define   _ENCRYPT_C_SC_NB_1              46            // ˫������

//���ӷ����ϵͳ����
#define   _ENCRYPT_M_EI_NA_1              70            // ��ʼ������

//���Ʒ����ϵͳ����
#define   _ENCRYPT_C_IC_NA_1              100            // վ���ٻ�����
#define   _ENCRYPT_C_CI_NA_1              101            // �������ٻ�����
#define   _ENCRYPT_C_CS_NA_1              103            // ʱ��ͬ������
#define   _ENCRYPT_C_TS_NA_1              104            // ��������
#define   _ENCRYPT_C_RP_NA_1              105            // ��λ��������
#define   _ENCRYPT_C_SR_NA_1              200            // �л���ֵ��
#define   _ENCRYPT_C_RR_NA_1              201            // ����ֵ����
#define   _ENCRYPT_C_RS_NA_1              202            // �������Ͷ�ֵ
#define   _ENCRYPT_C_WS_NA_1              203            // д�����Ͷ�ֵ
#define   _ENCRYPT_F_FR_NA_1              210            // �ļ�����
#define   _ENCRYPT_F_SR_NA_1              211            // �������



#endif /* END _ENCRYPT_CONFIG_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/
