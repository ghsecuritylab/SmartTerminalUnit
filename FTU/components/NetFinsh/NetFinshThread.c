/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     ʹ������ӿڣ�UDPʵ��finsh����λ���·�����������������ļ����������
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */


/*****************************include***********************************/
#include "lwip/api.h"
#include "NetFinshApp.h"
#include "NetFinshThread.h"
#include "tcp_server.h"
#include "rtconfig.h"
#include ".\MultiThread\multi_thread.h"
#include "station_manager.h"
#include "w5500_server.h"

#include "lwip/sockets.h"
#include "lwip/ip.h"
#include <lwip/netdb.h>






/*******************************Function********************************/

/**
  * @brief : ʹ��dp83848��UDPЭ�����finshԶ�̵�¼�ʹ�ӡ����Ĺ���
  * @param : none
  * @return: none
  * @update: [2018-09-10][��  ��][����]
  * 		 [2018-10-09][��  ��][��Ϊsocket�ӿ�ʵ��]
  * 
  */  
#if RT_USING_NET_FINSH
static void rt_net_finsh_thread_entry(void *param)
{
	int32_t receiveNum;		//���յ����ֽ���
	uint8_t buffer[PRINT_BUFFER_SIZE] = {0};		//���պͷ��ͻ�����
	uint32_t addressLenth;		//��ַ����
	struct sockaddr_in localAddress, remoteAddress;		//����IP��ַ��Զ��IP��ַ
	uint8_t ret = 0;			//����ִ�еķ���ֵ,�����ж��Ƿ�ִ�гɹ�
	uint32_t i = 0;

	/* ���غ�Զ��IP���˿ڵ�����,socket�Ľ����Ͱ� */
	IpAddressInit(&localAddress, FINSH_LOCAL_PORT, &remoteAddress, FINSH_REMOTE_PORT, REMOTE_ADDRESS);
	ret += UdpSocketInit(&g_NetFinshSocket, (struct sockaddr*)&localAddress);
	addressLenth = sizeof(struct sockaddr);
	
	/* ����FIFO�ͷ���FIFO���붯̬�ڴ��Լ���ʼ�� */
	ret += FifoMallocAndInit(&FinshReceiveFifoHandle, &FinshBuffer, NET_FINSH_BUFSIZE, &FinshBufferPack);
	ret += FifoMallocAndInit(&PrintfFifoHandle, &PrintfBuffer, NET_PRINTF_BUFSIZE, &PrintfBufferPack);
	if(ret)		//�ϱ���������ִ�гɹ�����0,ִ��ʧ�ܷ���ֵ����0,ֻҪ��һ��ʧ��,���ʾʧ��
	{
		FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/* fifo����ʧ��,������ͷ� */
		FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
		rt_kprintf("Net Finsh Thread: Fifo Malloc And Init Faliure\r\n");
		return;
	}
	
	/*UDP�����Ѿ�������֮�����ʹ�����ڵĴ�ӡ������*/
	NetFinshFlag = true;
	rt_kprintf("Net finsh Init Success\r\n");			
	
	while (1)
	{
		/* �������� */
		memset(buffer, 0, PRINT_BUFFER_SIZE);
		for(i=0; (i<PRINT_BUFFER_SIZE) && (PrintfFifoHandle->fifo.count); i++)
		{
			buffer[i] = FifoCharDequeue(PrintfFifoHandle);
		}
		if(0 != i)
		{
			/* ��rt_kprintf������ʵ�ʷ��ͳ�ȥ */
			lwip_sendto(g_NetFinshSocket, buffer, i, 0, (struct sockaddr*)&remoteAddress, sizeof(struct sockaddr));
		}
		
		/* �������� */
		memset(buffer, 0, PRINT_BUFFER_SIZE);
		receiveNum = lwip_recvfrom(g_NetFinshSocket, buffer, PRINT_BUFFER_SIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		if((receiveNum > 0) && (receiveNum < PRINT_BUFFER_SIZE))
		{
			/* �����յ���������ӣ��ȴ�finsh��ȡ */
			FifoStringEnqueue(FinshReceiveFifoHandle, buffer, receiveNum);
		}
		rt_thread_delay(10);		/* �ͷ�cpu��Դ���õ����ȼ������������ */
	}
	
	lwip_close(g_NetFinshSocket);
	FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
	FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
	NetFinshFlag = false;
	return;
}
#endif	/* RT_USING_NET_FINSH */


/**
  * @brief : ʹ��dp83848��UDPЭ�������λ���·����õĹ���
  * @param : none
  * @return: none
  * @update: [2018-09-12][��  ��][����]
  * 		 [2018-10-09][��  ��][��Ϊsocket�ӿ�ʵ��]
  * 
  */
#if RT_USING_UDP_SERVE
static void rt_udp_serve_thread_entry(void *param)
{
	int32_t receiveNum;		//���յ����ֽ���
	uint8_t buffer[UDP_SERVE_BUFSIZE] = {0};		//���պͷ��ͻ�����
	uint32_t addressLenth;		//��ַ����
	struct sockaddr_in localAddress, remoteAddress;		//����IP��ַ��Զ��IP��ַ
	uint8_t ret = 0;			//����ִ�еķ���ֵ,�����ж��Ƿ�ִ�гɹ�
	uint32_t i = 0;

	/* ���غ�Զ��IP���˿ڵ�����,socket�Ľ����Ͱ� */
	IpAddressInit(&localAddress, UDP_SERVE_LOCAL_PORT, &remoteAddress, UDP_SERVE_REMOTE_PORT, REMOTE_ADDRESS);
	ret += UdpSocketInit(&g_UDP_ServeSocket, (struct sockaddr*)&localAddress);
	addressLenth = sizeof(struct sockaddr);

	/* ��λ���·����õ�FIFO���붯̬�ڴ��Լ���ʼ�� */
	ret += FifoMallocAndInit(&UDP_ServeFifoHandle, &UDP_ServeBuffer, UDP_SERVE_BUFSIZE, &UDP_ServeBufferPack);
	if(ret)		//�ϱ���������ִ�гɹ�����0,ִ��ʧ�ܷ���ֵ����0,ֻҪ��һ��ʧ��,���ʾʧ��
	{
		FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);		/* fifo����ʧ��,������ͷ� */
		rt_kprintf("Udp Serve Thread: Fifo Malloc And Init Faliure\r\n");
		return;
	}

	/*��λ���·����õĶ˿��Ѿ�����*/
	UDP_ServeFlag = true;
	rt_kprintf("UDP Communbicate Serve Init Success\r\n");

	while (1)
	{
		/* �������� */
		memset(buffer, 0, UDP_SERVE_BUFSIZE);
		receiveNum = lwip_recvfrom(g_UDP_ServeSocket, buffer, UDP_SERVE_BUFSIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		if((receiveNum > 0) && (receiveNum < UDP_SERVE_BUFSIZE))
		{
			StationPointFrameDeal(buffer, receiveNum);
		}
		rt_thread_delay(10);		/* �ͷ�cpu��Դ���õ����ȼ������������ */
	}

	lwip_close(g_UDP_ServeSocket);
	FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);	/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
	UDP_ServeFlag = false;
	return;
}
#endif	/* RT_USING_UDP_SERVE */

/**
  * @brief : dp82848ʵ�ֵ�ά��������������
  * @param : none
  * @return: none
  * @update: [2018-09-17][��  ��][����]
  * 		 [2018-10-09][��  ��][��Ϊsocket�ӿ�ʵ��]
  * 
  */
static void rt_maintenance_serve_thread_entry(void *param)
{
	int32_t receiveNum;		//���յ����ֽ���
	uint8_t buffer[MAINTENANCE_SERVE_BUFSIZE] = {0};		//���պͷ��ͻ�����
	uint32_t addressLenth;		//��ַ����
	struct sockaddr_in localAddress, remoteAddress;		//����IP��ַ��Զ��IP��ַ
	uint8_t ret = 0;			//����ִ�еķ���ֵ,�����ж��Ƿ�ִ�гɹ�
	uint32_t i = 0;

	/* ���غ�Զ��IP���˿ڵ�����,socket�Ľ����Ͱ� */
	IpAddressInit(&localAddress, MAINTACE_SERVE_LOCAL_PORT, &remoteAddress, MAINTACE_SERVE_REMOTE_PORT, REMOTE_ADDRESS);
	ret += UdpSocketInit(&g_MaintenanceServeSocket, (struct sockaddr*)&localAddress);
	addressLenth = sizeof(struct sockaddr);

	/* ��λ���·����õ�FIFO���붯̬�ڴ��Լ���ʼ�� */
	ret += FifoMallocAndInit(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer,
							MAINTENANCE_SERVE_BUFSIZE, &MaintenanceServeBufferPack);	/*��ʼ��fifo*/
	if(ret)		//�ϱ���������ִ�гɹ�����0,ִ��ʧ�ܷ���ֵ����0,ֻҪ��һ��ʧ��,���ʾʧ��
	{
		FifoFree(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, &MaintenanceServeBufferPack);//fifo����ʧ��,������ͷ�
		rt_kprintf("Maintenance Serve Thread: Fifo Malloc And Init Faliure\r\n");
		return;
	}

	/*��λ���·������ļ���ά���˿��Ѿ�����*/
	MaintenanceServe = true;
	g_StationManger.isMaintanceRun = true;
	rt_kprintf("UDP Maintenance Serve Init Success\r\n");

	while (1)
	{
		/* �������� */
		memset(buffer, 0, MAINTENANCE_SERVE_BUFSIZE);
		receiveNum = lwip_recvfrom(g_MaintenanceServeSocket, buffer, MAINTENANCE_SERVE_BUFSIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		if((receiveNum > 0) && (receiveNum < MAINTENANCE_SERVE_BUFSIZE))
		{
			MantaiceFrameDeal(buffer, receiveNum);
		}
		rt_thread_delay(10);		/* �ͷ�cpu��Դ���õ����ȼ������������ */
	}

	lwip_close(g_MaintenanceServeSocket);
	FifoFree(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, &MaintenanceServeBufferPack);//fifo����ʧ��,������ͷ�
	MaintenanceServe = false;
	g_StationManger.isMaintanceRun = false;
	return;
}

/**
  * @brief : Start udp finsh thread
  * @param : none
  * @return: none
  * @update: [2018-09-10][��  ��][����]
  * 
  */
#if RT_USING_NET_FINSH
static void NetFinsh_thread_start(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(NET_FINSH_THREAD_NAME, 
                           rt_net_finsh_thread_entry, 
                           param, 
                           NET_FINSH_THREAD_STACK_SIZE, 
                           NET_FINSH_THREAD_PRIORITY, 
                           NET_FINSH_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("Net finsh thread start \r\n"); 
    }   
}
#endif


/**
  * @brief : Start udp finsh thread
  * @param : none
  * @return: none
  * @update: [2018-09-12][��  ��][����]
  * 
  */
#if RT_USING_UDP_SERVE
static void UDP_CommunicateServe_thread_start(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(UDP_SERVE_THREAD_NAME, 
                           rt_udp_serve_thread_entry, 
                           param, 
                           UDP_SERVE_THREAD_STACK_SIZE, 
                           UDP_SERVE_THREAD_PRIORITY, 
                           UDP_SERVE_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("UDP serve thread start \r\n"); 
    }   
}
#endif


/**
  * @brief : dp82848ʵ��ά������
  * @param : none
  * @return: none
  * @update: [2018-09-17][��  ��][����]
  * 
  */
void DP83848_MaintenanceServiceInit(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(MAINTENANCE_SERVE_THREAD_NAME, 
                           rt_maintenance_serve_thread_entry, 
                           param, 
                           MAINTENANCE_SERVE_THREAD_STACK_SIZE, 
                           MAINTENANCE_SERVE_THREAD_PRIORITY, 
                           MAINTENANCE_SERVE_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("Maintenance Serve thread start \r\n"); 
    }   
}


/**
  * @brief : ʹ��dp83848��UDPЭ�����finshԶ�̵�¼�ʹ�ӡ����Ĺ���
  * @param : none
  * @return: 0:�ɹ�; 1:ʧ��
  * @update: [2018-09-10][��  ��][����]
  * 
  */
#if RT_USING_NET_FINSH
uint8_t rt_NetFinsh_thread_start(void)
{
    NetFinsh_thread_start(RT_NULL);
	
	return RT_EOK;
}
INIT_APP_EXPORT(rt_NetFinsh_thread_start);
#endif


/**
  * @brief : ʹ��dp83848��UDPЭ�������λ���·����õĹ���
  * @param : none
  * @return: 0:�ɹ�; 1:ʧ��
  * @update: [2018-09-12][��  ��][����]
  * 
  */
#if RT_USING_UDP_SERVE
uint8_t rt_UDP_CommunicateServe_thread_start(void)
{
	UDP_CommunicateServe_thread_start(RT_NULL);

	return RT_EOK;
}
INIT_APP_EXPORT(rt_UDP_CommunicateServe_thread_start);
#endif



/*****************************File End**********************************/

