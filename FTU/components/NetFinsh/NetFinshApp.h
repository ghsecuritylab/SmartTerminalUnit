/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.h
  * @brief:     ʹ������ӿڣ�UDPʵ��finsh����λ���·�����������������ļ������Ӧ�ú���
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
  
#ifndef __NET_FINSH_APP_H__
#define __NET_FINSH_APP_H__

#include "common_data.h"
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "buffer.h"
#include <stdbool.h>
#include <sys/types.h>
#include <lwip/netdb.h>

/*****************************�궨��************************************/
#define REMOTE_ADDRESS 					"192.168.10.111"		/*Զ��IP�ĵ�ַ*/
#define FINSH_LOCAL_PORT				5533					//finsh�����ڴ�ӡ����ı��ض˿�
#define FINSH_REMOTE_PORT				FINSH_LOCAL_PORT		//finsh���������Զ�˶˿�
#define UDP_SERVE_LOCAL_PORT			5555					//UDPͨ�ŷ��񱾵ض˿ں�
#define UDP_SERVE_REMOTE_PORT			UDP_SERVE_LOCAL_PORT	//UDPͨ�ŷ���Զ�˶˿ں�
#define MAINTACE_SERVE_LOCAL_PORT		5500					//ά�����񱾵ض˿ں�
#define MAINTACE_SERVE_REMOTE_PORT		5555					//ά������Զ�˶˿ں�

#define PRINT_BUFFER_SIZE				512						//��ӡ����Ļ�������С
#define NET_FINSH_BUFSIZE				512						//finsh��FIFO���ջ�������С
#define NET_PRINTF_BUFSIZE				2048					//��ӡ��FIFO�����������С
#define UDP_SERVE_BUFSIZE				1024					//UDPͨ�ŷ����FIFO��������С
#define MAINTENANCE_SERVE_BUFSIZE		1024					//ά�������FIFO��������С

extern int32_t g_NetFinshSocket;
extern int32_t g_UDP_ServeSocket;
extern int32_t g_MaintenanceServeSocket;

extern bool NetFinshFlag;
extern bool UDP_ServeFlag;
extern bool MaintenanceServe;

extern FifoHandle* FinshReceiveFifoHandle;
extern uint8_t* FinshBuffer;
extern PointUint8* FinshBufferPack;
extern FifoHandle* PrintfFifoHandle;
extern uint8_t* PrintfBuffer;
extern PointUint8* PrintfBufferPack;
extern FifoHandle* UDP_ServeFifoHandle;
extern uint8_t* UDP_ServeBuffer;
extern PointUint8* UDP_ServeBufferPack;
extern FifoHandle* MaintenanceServeFifoHandle;
extern uint8_t* MaintenanceServeBuffer;
extern PointUint8* MaintenanceServeBufferPack;



void FifoFree(FifoHandle** fifoHandle, uint8_t** buffer, PointUint8** bufferPack); //dp83848��ط���Ķ��ж�̬�ڴ��ͷ�
uint8_t FifoMallocAndInit(FifoHandle** fifoHandle, uint8_t** buffer, uint32_t bufSize, PointUint8** bufferPack);  //���г�ʼ��
void FifoStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size);    //�ַ�����Ӳ���
char FifoCharDequeue(FifoHandle *handle);    //�ַ����Ӳ���
char NetGetchar(void);    //��ȡһ���ַ�,��finsh��ʹ��,����getchar����
int32_t UDP_SocketSendString(int32_t socket, uint8_t* data, uint32_t lenth,
	uint8_t* remoteAddressString, uint32_t remotePort);		//udpʹ��socket�ӿڽ��з������ݵĺ���
void IpAddressInit(struct sockaddr_in* localAddress, uint32_t localPort,
	struct sockaddr_in* remoteAddress, uint32_t remotePort, uint8_t* remoteAddressString);	//IP��ַ����
uint32_t UdpSocketInit(uint32_t* socketNum, struct sockaddr* socketAddress);	//ʹ��UDP��socket�����Ͱ�



#endif

