/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     ʹ������ӿڣ�UDPЭ��ʵ��Զ�̵�¼������finsh���������
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
/*****************************�궨��************************************/
#define REMOTE_ADDRESS 				"192.168.10.111"			/*Զ��IP�ĵ�ַ*/
#define FINSH_LOCAL_PORT			5533						//finsh�����ڴ�ӡ����ı��ض˿�
#define FINSH_REMOTE_PORT			FINSH_LOCAL_PORT			//finsh���������Զ�˶˿�
#define UDP_SERVE_LOCAL_PORT		5555						//UDPͨ�ŷ��񱾵ض˿ں�
#define UDP_SERVE_REMOTE_PORT		UDP_SERVE_LOCAL_PORT		//UDPͨ�ŷ���Զ�˶˿ں�
#define MAINTACE_SERVE_LOCAL_PORT	5500						//ά�����񱾵ض˿ں�
#define MAINTACE_SERVE_REMOTE_PORT	5555						//ά������Զ�˶˿ں�






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
#if RT_USING_NETCONN
static void rt_net_finsh_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint8_t printBuffer[PRINT_BUFFER_SIZE] = {0};
	uint32_t receviceNum = 0;
	
	/* ����FIFO�ͷ���FIFO���붯̬�ڴ��Լ���ʼ�� */
	ret = FifoMallocAndInit(&FinshReceiveFifoHandle, &FinshBuffer, NET_FINSH_BUFSIZE, &FinshBufferPack);
	if(!ret)
	{
		ret = FifoMallocAndInit(&PrintfFifoHandle, &PrintfBuffer, NET_PRINTF_BUFSIZE, &PrintfBufferPack);
	}
	
	while(!ret)
	{
		rt_thread_delay(1000);
		DP83848_ServeIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);	

		g_NetFinshNetconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
		
		
		if(g_NetFinshNetconn != NULL)  //����UDP���ӳɹ�
		{
			g_NetFinshNetconn->recv_timeout = 100;
			err = netconn_bind(g_NetFinshNetconn, IP_ADDR_ANY, FINSH_LOCAL_PORT); 
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //����Ŀ��IP��ַ
			netconn_connect(g_NetFinshNetconn, &destipAddr, FINSH_REMOTE_PORT); 	//���ӵ�Զ������
			if(err == ERR_OK)//�����
			{
				/*UDP�����Ѿ�������֮�����ʹ�����ڵĴ�ӡ������*/
				NetFinshFlag = true;
				rt_kprintf("Net finsh Init Success\r\n");
				
				while(1)
				{
					/*�ȴ����գ������յ����ַ����*/
					receviceNum = UDP_NetconnReceiveString(g_NetFinshNetconn, FinshReceiveFifoHandle);
					
					/*�������е����ݷ��ͳ�ȥ*/
					if(true == NetFinshFlag)
					{
						memset(printBuffer, 0, PRINT_BUFFER_SIZE);
						for(i=0; (i<PRINT_BUFFER_SIZE) && (PrintfFifoHandle->fifo.count); i++)
						{
							printBuffer[i] = FifoCharDequeue(PrintfFifoHandle);
						}
						if(0 != i)
						{
							UDP_NetconnSendString(g_NetFinshNetconn, printBuffer);
						}
						
					}
				}
			}
			else
			{
				rt_kprintf("UDP bind faliure\r\n");
			}
		}
		else
		{
			rt_kprintf("UDP connect failure\r\n");
		}
		NetFinshFlag = false;
	}
	
	FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
	FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
}

#elif RT_USING_SOCKET	/* RT_USING_NETCONN */
static void rt_net_finsh_thread_entry(void *param)
{
	uint32_t receiveNum;		//���յ����ֽ���
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
		lwip_recvfrom(g_NetFinshSocket, buffer, PRINT_BUFFER_SIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		receiveNum = strlen((char*)buffer);
		if((receiveNum > 0) && (receiveNum < PRINT_BUFFER_SIZE))
		{
			/* �����յ���������ӣ��ȴ�finsh��ȡ */
			FifoStringEnqueue(FinshReceiveFifoHandle, buffer, receiveNum);
		}
		rt_thread_delay(10);
	}
	
	lwip_close(g_NetFinshSocket);
	FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
	FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
	NetFinshFlag = false;
	return;
}
#endif	/* RT_USING_SOCKET */
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
#if RT_USING_NETCONN
static void rt_udp_serve_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint32_t receviceNum = 0;
	uint8_t printBuffer[UDP_SERVE_BUFSIZE] = {0};

	// NVIC_Configuration();
	
	ret = FifoMallocAndInit(&UDP_ServeFifoHandle, &UDP_ServeBuffer, UDP_SERVE_BUFSIZE, &UDP_ServeBufferPack);  /*��ʼ��fifo*/
	ip_addr_t iplocal;
	IP4_ADDR(&iplocal, 192, 168, 10, 129);
	while(!ret)
	{
		rt_thread_delay(1000);
		DP83848_ServeIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);

		g_UDP_ServeNetconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
		
		if(g_UDP_ServeNetconn != NULL)  //����UDP���ӳɹ�
		{
			g_UDP_ServeNetconn->recv_timeout = 100;
			err = netconn_bind(g_UDP_ServeNetconn, IP_ADDR_ANY, UDP_SERVE_LOCAL_PORT);
		//	err = netconn_bind(g_UDP_ServeNetconn, &iplocal, UDP_SERVE_LOCAL_PORT);
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //����Ŀ��IP��ַ
			netconn_connect(g_UDP_ServeNetconn, &destipAddr, UDP_SERVE_REMOTE_PORT); 	//���ӵ�Զ������
			ip_set_option(g_UDP_ServeNetconn->pcb.ip, SO_BROADCAST);		//���ýӿ�Ϊ���չ㲥
			if(err == ERR_OK)//�����
			{
				/*��λ���·����õĶ˿��Ѿ�����*/
				UDP_ServeFlag = true;
				rt_kprintf("UDP Communbicate Serve Init Success\r\n");
				
				while(1)
				{
					receviceNum = UDP_NetconnReceiveString(g_UDP_ServeNetconn, UDP_ServeFifoHandle);
				
					if(receviceNum > 0)
                   {
                       memset(printBuffer, 0, UDP_SERVE_BUFSIZE);
						for(i=0; (i<UDP_SERVE_BUFSIZE) && (UDP_ServeFifoHandle->fifo.count); i++)
						{
							printBuffer[i] = FifoCharDequeue(UDP_ServeFifoHandle);
						}
						if(0 != i)
						{
							StationPointFrameDeal(printBuffer, i);
							rt_kprintf("StationPointFrameDeal success!\r\n");
						}
                   }
					
				}
			}
			else
			{
				rt_kprintf("UDP bind faliure\r\n");
			}
		}
		else
		{
			rt_kprintf("UDP connect failure\r\n");
		}
		UDP_ServeFlag = false;
	}
	/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
	FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);
}

#elif RT_USING_SOCKET	/* RT_USING_NETCONN */
static void rt_udp_serve_thread_entry(void *param)
{
	uint32_t receiveNum;		//���յ����ֽ���
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
		lwip_recvfrom(g_UDP_ServeSocket, buffer, UDP_SERVE_BUFSIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		receiveNum = strlen((char*)buffer);
		if((receiveNum > 0) && (receiveNum < UDP_SERVE_BUFSIZE))
		{
			StationPointFrameDeal(buffer, receiveNum);
		}
		rt_thread_delay(10);
	}

	lwip_close(g_UDP_ServeSocket);
	FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);	/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
	UDP_ServeFlag = false;
	return;
}
#endif	/* RT_USING_SOCKET */
#endif	/* RT_USING_NET_FINSH */

/**
  * @brief : dp82848ʵ�ֵ�ά��������������
  * @param : none
  * @return: none
  * @update: [2018-09-17][��  ��][����]
  * 
  */
 #if RT_USING_NETCONN
static void rt_maintenance_serve_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint32_t receviceNum = 0;
	uint8_t printBuffer[MAINTENANCE_SERVE_BUFSIZE] = {0};
	
	ret = FifoMallocAndInit(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, \
							MAINTENANCE_SERVE_BUFSIZE, &MaintenanceServeBufferPack);/*��ʼ��fifo*/

	g_StationManger.isMaintanceRun = false;

	while(!ret)
	{
		rt_thread_delay(1000);
		DP83848_ServeIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);

		g_MaintenanceServeNetconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
		
		if(g_MaintenanceServeNetconn != NULL)  //����UDP���ӳɹ�
		{
			g_MaintenanceServeNetconn->recv_timeout = 100;
			err = netconn_bind(g_MaintenanceServeNetconn, IP_ADDR_ANY, MAINTACE_SERVE_LOCAL_PORT); 
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //����Ŀ��IP��ַ
			netconn_connect(g_MaintenanceServeNetconn, &destipAddr, MAINTACE_SERVE_REMOTE_PORT); 	//���ӵ�Զ������
			if(err == ERR_OK)//�����
			{
				MaintenanceServe = true;
				g_StationManger.isMaintanceRun = true;

				/*UDP�����Ѿ�������֮�����ʹ�����ڵĴ�ӡ������*/
				rt_kprintf("UDP Maintenance Serve Init Success\r\n");
				
				while(1)
				{
					/*�ȴ����գ������յ����ַ����*/
					receviceNum = UDP_NetconnReceiveString(g_MaintenanceServeNetconn, MaintenanceServeFifoHandle);
					
					if(receviceNum > 0)
                    {
                        memset(printBuffer, 0, MAINTENANCE_SERVE_BUFSIZE);
						for(i=0; (i<MAINTENANCE_SERVE_BUFSIZE) && (MaintenanceServeFifoHandle->fifo.count); i++)
						{
							printBuffer[i] = FifoCharDequeue(MaintenanceServeFifoHandle);
						}
						if(0 != i)
						{
							MantaiceFrameDeal(printBuffer, i);
						}
						
                    }
				}
			}
			else
			{
				rt_kprintf("UDP bind faliure\r\n");
			}
		}
		else
		{
			rt_kprintf("UDP connect failure\r\n");
		}
		MaintenanceServe = false;
		g_StationManger.isMaintanceRun = false;
	}
	/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
	FifoFree(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, &MaintenanceServeBufferPack);
}

#elif RT_USING_SOCKET	/* RT_USING_NETCONN */
static void rt_maintenance_serve_thread_entry(void *param)
{

}
#endif	/* RT_USING_SOCKET */

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

