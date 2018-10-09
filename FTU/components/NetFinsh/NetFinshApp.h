/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.h
  * @brief:     ʹ������ӿڣ�UDPЭ��ʵ��Զ�̵�¼������finsh��Ӧ�ú���
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


#define NET_FINSH_BUFSIZE		512			//finsh��FIFO���ջ�������С
#define NET_PRINTF_BUFSIZE		2048		//��ӡ��FIFO�����������С
#define UDP_SERVE_BUFSIZE		1024		//UDPͨ�ŷ����FIFO��������С
#define MAINTENANCE_SERVE_BUFSIZE	1024	//ά�������FIFO��������С

extern struct netconn* g_NetFinshNetconn;
extern bool NetFinshFlag;
extern struct netconn* g_UDP_ServeNetconn;
extern bool UDP_ServeFlag;
extern struct netconn* g_MaintenanceServeNetconn;
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

extern uint32_t g_NetFinshSocket;
extern uint32_t g_UDP_ServeSocket;
extern uint32_t g_MaintenanceServeSocket;


void DP83848_ServeIpSet(struct lwip_dev* lwip);  //dp83848��ط���IP��ַ����
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString);  //UDP�����ַ���
uint32_t UDP_NetconnReceiveString(struct netconn* udpNetConn, FifoHandle* fifoHandle);   //UDP�����ַ���
void FifoFree(FifoHandle** fifoHandle, uint8_t** buffer, PointUint8** bufferPack); //dp83848��ط���Ķ��ж�̬�ڴ��ͷ�
uint8_t FifoMallocAndInit(FifoHandle** fifoHandle, uint8_t** buffer, uint32_t bufSize, PointUint8** bufferPack);  //���г�ʼ��
void FifoStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size);    //�ַ�����Ӳ���
char FifoCharDequeue(FifoHandle *handle);    //�ַ����Ӳ���
char NetGetchar(void);    //��ȡһ���ַ�,��finsh��ʹ��,����getchar����
void UDP_SocketSendString(uint8_t* remoteAddressString, uint32_t port, uint8_t* sendData);	//udpʹ��socket�ӿڽ��з������ݵĺ���
void IpAddressInit(struct sockaddr_in* localAddress, uint32_t localPort,
	struct sockaddr_in* remoteAddress, uint32_t remotePort, uint8_t* remoteAddressString);	//IP��ַ����
uint32_t UdpSocketInit(uint32_t* socketNum, struct sockaddr* socketAddress);	//ʹ��UDP��socket�����Ͱ�

//void NetFinsh_kprintf(const char *fmt, ...);


#endif

