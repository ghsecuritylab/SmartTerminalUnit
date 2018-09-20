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


/*****************************�궨��************************************/
#define FINSH_LOCAL_PORT			5533						//finsh�����ڴ�ӡ����ı��ض˿�
#define FINSH_REMOTE_PORT			FINSH_LOCAL_PORT			//finsh���������Զ�˶˿�
#define UDP_SERVE_LOCAL_PORT		5555						//UDPͨ�ŷ��񱾵ض˿ں�
#define UDP_SERVE_REMOTE_PORT		UDP_SERVE_LOCAL_PORT		//UDPͨ�ŷ���Զ�˶˿ں�
#define MAINTACE_SERVE_LOCAL_PORT	5500						//ά�����񱾵ض˿ں�
#define MAINTACE_SERVE_REMOTE_PORT	MAINTACE_SERVE_LOCAL_PORT	//ά������Զ�˶˿ں�
#define PRINT_BUFFER_SIZE			512		//��ӡ����Ļ�������С





/*****************************Function**********************************/

/**
  * @brief : net finsh thread entry
  * @param : none
  * @return: none
  * @update: [2018-09-10][��  ��][����]
  * 
  */  
#if RT_USING_NET_FINSH
static void rt_net_finsh_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint8_t printBuffer[PRINT_BUFFER_SIZE] = {0};
	
	ret = FifoMallocAndInit(&FinshReceiveFifoHandle, &FinshBuffer, NET_FINSH_BUFSIZE, &FinshBufferPack);  /*��ʼ��fifo*/
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
					UDP_NetconnReceiveString(g_NetFinshNetconn, FinshReceiveFifoHandle);
					
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
#endif


/**
  * @brief : udp serve thread entry
  * @param : none
  * @return: none
  * @update: [2018-09-12][��  ��][����]
  * 
  */
#if RT_USING_UDP_SERVE
static void rt_udp_serve_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint32_t receviceNum = 0;
	uint8_t printBuffer[PRINT_BUFFER_SIZE] = {0};

	NVIC_Configuration();
	
	ret = FifoMallocAndInit(&UDP_ServeFifoHandle, &UDP_ServeBuffer, UDP_SERVE_BUFSIZE, &UDP_ServeBufferPack);  /*��ʼ��fifo*/
	
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
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //����Ŀ��IP��ַ
			netconn_connect(g_UDP_ServeNetconn, &destipAddr, UDP_SERVE_REMOTE_PORT); 	//���ӵ�Զ������
			if(err == ERR_OK)//�����
			{
				/*UDP�����Ѿ�������֮�����ʹ�����ڵĴ�ӡ������*/
				UDP_ServeFlag = true;
				rt_kprintf("UDP Communbicate Serve Init Success\r\n");
				
				while(1)
				{
					receviceNum = UDP_NetconnReceiveString(g_UDP_ServeNetconn, UDP_ServeFifoHandle);
					if(receviceNum > 0)
                    {
                        MantaiceFrameDeal(UDP_ServeFifoHandle->fifo.pData, receviceNum);
                    }
					
					/*����*/
//					if(true == UDP_ServeFlag)
//					{
//						memset(printBuffer, 0, PRINT_BUFFER_SIZE);
//						for(i=0; (i<PRINT_BUFFER_SIZE) && (UDP_ServeFifoHandle->fifo.count); i++)
//						{
//							printBuffer[i] = FifoCharDequeue(UDP_ServeFifoHandle);
//						}
//						if(0 != i)
//						{
//							UDP_NetconnSendString(g_UDP_ServeNetconn, printBuffer);
//						}
//						
//					}
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
	
	FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);  /*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
}
#endif

/**
  * @brief : dp82848ʵ�ֵ�ά��������������
  * @param : none
  * @return: none
  * @update: [2018-09-17][��  ��][����]
  * 
  */
static void rt_maintenance_serve_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint32_t receviceNum = 0;
	uint8_t printBuffer[PRINT_BUFFER_SIZE] = {0};
	
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
                        MantaiceFrameDeal(MaintenanceServeFifoHandle->fifo.pData, receviceNum);
                    }
					
//					if(true == MaintenanceServe)
//					{
//						memset(printBuffer, 0, PRINT_BUFFER_SIZE);
//						for(i=0; (i<PRINT_BUFFER_SIZE) && (UDP_ServeFifoHandle->fifo.count); i++)
//						{
//							printBuffer[i] = FifoCharDequeue(MaintenanceServeFifoHandle);
//						}
//						if(0 != i)
//						{
//							UDP_NetconnSendString(g_MaintenanceServeNetconn, printBuffer);
//						}
						
//					}
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
	
	FifoFree(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, &MaintenanceServeBufferPack);		/*�ͷŽ��պͷ��͵Ķ������õ��Ķ�̬������ڴ�*/
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
  * @brief : ʹ��dp83848��UDPЭ�����finshԶ�̵�¼�Ĺ���
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
  * @brief : ʹ��dp83848��UDPЭ���������Զ�ͨ�ŵĹ���
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

