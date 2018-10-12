/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.c
  * @brief:     ʹ������ӿڣ�UDPʵ��finsh����λ���·�����������������ļ������Ӧ�ú���
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
 

/*****************************include***********************************/
#include "NetFinshApp.h"
#include <string.h>
#include "board.h"
#include "rtdef.h"
#include "stm32f429xx.h"
#include <lwip/sockets.h>
#include <finsh.h>


/****************************ȫ�ֱ���***********************************/
int32_t g_NetFinshSocket = 0;				/* finsh��socket�� */
int32_t g_UDP_ServeSocket = 0;				/* ��λ���·����������socket�� */
int32_t g_MaintenanceServeSocket = 0;		/* ��λ���·������ļ���socket�� */

bool NetFinshFlag = false;							/* ����finsh��������Դ�ӡ��Ϣ������UDPͨ����ͨ��־ */
bool UDP_ServeFlag = false;							/* ����UDPͨ�ŷ��������UDPͨ����ͨ��־ */
bool MaintenanceServe = false;						/* ����ά�����������UDPͨ����ͨ��־ */

/* finsh����FIFO��� */
FifoHandle* FinshReceiveFifoHandle = NULL;
uint8_t* FinshBuffer = NULL;
PointUint8* FinshBufferPack = NULL;

/* ��ӡ���FIFO��� */
FifoHandle* PrintfFifoHandle = NULL;
uint8_t* PrintfBuffer = NULL;
PointUint8* PrintfBufferPack = NULL;

/* ��λ���·�������������FIFO��� */
FifoHandle* UDP_ServeFifoHandle = NULL;
uint8_t* UDP_ServeBuffer = NULL;
PointUint8* UDP_ServeBufferPack = NULL;

/* ��λ���·������ļ���ά������FIFO��� */
FifoHandle* MaintenanceServeFifoHandle = NULL;
uint8_t* MaintenanceServeBuffer = NULL;
PointUint8* MaintenanceServeBufferPack = NULL;



/*****************************Function**********************************/

/**
  * @brief : ʹ��dp83848ͨ�ŷ����еĶ��ж�̬�ڴ��ͷ�
  * @param : fifoHandle ��Ҫ�ͷ��ڴ��FIFO�ľ��
  * @param : buffer ��Ӧ�Ķ��еĻ�����
  * @param : bufferPack ��Ӧ�Ļ������ṹ��
  * @return: none
  * @update: [2018-09-17][��  ��][����]
  * 
  */
void FifoFree(FifoHandle** fifoHandle, uint8_t** buffer, PointUint8** bufferPack)
{
	if(NULL != *fifoHandle)
	{
		rt_free(*fifoHandle);
		*fifoHandle = NULL;
	}
	if(NULL != *buffer)
	{
		rt_free(*buffer);
		*buffer = NULL;
	}
	if(NULL != *bufferPack)
	{
		rt_free(*bufferPack);
		*bufferPack = NULL;
	}
}


/**
  * @brief : ʹ��dp83848ͨ�ŵ�FIFO��ʼ��
  * @param : fifoHandle FIFO�ľ��
  * @param : buffer ���еĻ�����
  * @param : bufSize ��������С
  * @param : bufferPack �������ṹ��,����ʼ��FIFOʹ��
  * @return: 0:�ɹ�    1:ʧ��
  * @update: [2018-09-17][��  ��][����]
  * 
  */
uint8_t FifoMallocAndInit(FifoHandle** fifoHandle, uint8_t** buffer, uint32_t bufSize, PointUint8** bufferPack)
{
	uint8_t err = 0;
	*fifoHandle = rt_malloc(sizeof(FifoHandle));
	if(NULL == fifoHandle)
	{
		rt_kprintf("%s:[%s] #-%d: fifoHandle Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	*buffer = rt_malloc(sizeof(uint8_t) * bufSize);
	if(NULL == buffer)
	{
		rt_kprintf("%s:[%s] #-%d: buffer Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	*bufferPack = rt_malloc(sizeof(PointUint8));
	if(NULL == bufferPack)
	{
		rt_kprintf("%s:[%s] #-%d: bufferPack Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}

	if(0 == err)
	{
		(*bufferPack)->len = bufSize;
		(*bufferPack)->pData = *buffer;
		FifoInit(*fifoHandle, *bufferPack);
	}
	else
	{
		FifoFree(fifoHandle, buffer, bufferPack);
	}

	return err;
}


/**
  * @brief : finsh���յ��Ķ���ַ����
  * @param : handle FIFO���
  * @param : indata Ҫ��ӵ��ַ���
  * @param : size Ҫ��ӵ��ֽڴ�С
  * @return: none
  * @update: [2018-09-11][��  ��][����]
  */
void FifoStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size)
{
	uint32_t i = 0;
	for(i=0; i<size; i++)
	{
		handle->Enqueue(handle, indata[i]);
	}
}


/**
  * @brief : finsh��FIFO����
  * @param : handle FIFO���
  * @return: ���ӵ��ַ�
  * @update: [2018-09-11][��  ��][����]
  */
char FifoCharDequeue(FifoHandle *handle)
{
	uint8_t dedata = 0;
	uint8_t ret = 0;
	ret = handle->Dequeue(handle, &dedata);
	if(false == ret)
	{
		return 0;
	}
	else if(true == ret)
	{
		return dedata;
	}
}


/**
  * @brief : ʹ��UDP��ȡ�ַ�
  * @param : none
  * @return: ���ӵ��ַ�
  * @update: [2018-09-10][��  ��][����]
  */
char NetGetchar(void)
{
	return FifoCharDequeue(FinshReceiveFifoHandle);
}


/**
  * @brief : ʹ��UDP�ķ��ͺ���
  * @param : socket ����ʱʹ�õ�socket��
  * @param : data ���͵�����
  * @param : lenth ���͵����ݳ���
  * @param : remoteAddressString Զ��IP��ַ���ַ�����ʽ,���硰192.168.10.111��
  * @param : remotePort Զ�̶˿ں�
  * @return: -1�����ʹ���	������������ɵ��ֽ���
  * @update: [2018-10-09][��  ��][����]
  */
int32_t UDP_SocketSendString(int32_t socket, uint8_t* data, uint32_t lenth, uint8_t* remoteAddressString, uint32_t remotePort)
{
	int32_t sendNum = 0;
	struct hostent* host;
	struct sockaddr_in remoteAddress;

	if(socket <= 0)
	{
		rt_kprintf("UDP Send Error: Socket is Invalid\r\n");
		return -1;
	}

	/* ͨ��������ڲ������host��ַ��������������������������� */
	host = (struct hostent*)gethostbyname(remoteAddressString);

	/* ��ʼ��Ԥ���ӵķ���˵�ַ */
	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_port = htons(remotePort);
	remoteAddress.sin_addr = *((struct in_addr *) host->h_addr);
	memset(&(remoteAddress.sin_zero), 0, sizeof(remoteAddress.sin_zero));

	/* �������ݵ�����Զ�� */
	sendNum = lwip_sendto(socket, data, lenth, 0, (struct sockaddr*)&remoteAddress, sizeof(struct sockaddr));
	
	return sendNum;
}


/**
  * @brief : ʹ��UDP��IP��ַ����
  * @param : localAddress ����IP��ַ��socket�ṹ��ָ��,���Ը�ֵΪNULL
  * @param : localPort ���ض˿�,��localAddressΪNULL�����ֵ��������
  * @param : remoteAddress Զ��IP��ַ��socket�ṹ��ָ��,���Ը�ֵΪNULL
  * @param : remotePort Զ�̶˿�,��remoteAddressΪNULL�����ֵ��������
  * @param : remoteAddressString Զ��IP��ַ���ַ�����ʽ,���硰192.168.10.111��,��remoteAddressΪNULL�����ֵ��������
  * @return: none
  * @update: [2018-10-09][��  ��][����]
  */
void IpAddressInit(struct sockaddr_in* localAddress, uint32_t localPort,
	struct sockaddr_in* remoteAddress, uint32_t remotePort, uint8_t* remoteAddressString)
{
	struct hostent* host;
	
	if(NULL != localAddress)
	{
		/* ��ʼ�����ص�ַ */
		localAddress->sin_family = AF_INET;
		localAddress->sin_port = htons(localPort);
		localAddress->sin_addr.s_addr = INADDR_ANY;
		memset(&(localAddress->sin_zero), 0, sizeof(localAddress->sin_zero));
	}
	
	if(NULL != remoteAddress)
	{
		/* ��ʼ��Զ�̵�ַ */
		host = (struct hostent*)gethostbyname(remoteAddressString);
		remoteAddress->sin_family = AF_INET;
		remoteAddress->sin_port = htons(remotePort);
		remoteAddress->sin_addr = *((struct in_addr *) host->h_addr);
		memset(&(remoteAddress->sin_zero), 0, sizeof(remoteAddress->sin_zero));
	}
	
}


/**
  * @brief : ʹ��UDP��socket�����Ͱ�
  * @param : socketNum socket�ŵ�ָ��,��Ϊ�������
  * @param : socketAddress ���ص�ַ��ָ��
  * @return: 0���ɹ�	1��socket����ʧ��	2����ʧ��
  * @update: [2018-10-09][��  ��][����]
  */
uint32_t UdpSocketInit(uint32_t* socketNum, struct sockaddr* socketAddress)
{
	/* ����һ��socket��������SOCK_DGRAM��UDP���� */
	if ((*socketNum = lwip_socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		rt_kprintf("g_NetFinshSocket lwip_socket error\n");
		return 1;
	}
	
	/* ��socket������˵�ַ */
	if (lwip_bind(*socketNum, socketAddress, sizeof(struct sockaddr))== -1)
	{
		/* �󶨵�ַʧ�� */
		rt_kprintf("Bind error\n");
		return 2;
	}
	
	return 0;
}


/**
  * @brief : ��ӡip��ַ����Ϣ��finsh����
  * @return: none
  * @update: [2018-10-11][��  ��][����]
  */
void ifconfig(void)
{
	rt_kprintf("DP83848:\r\n");
	rt_kprintf("  IPv4 Address: %d.%d.%d.%d\r\n", g_EthDP83848.ip[0], g_EthDP83848.ip[1],
												  g_EthDP83848.ip[2], g_EthDP83848.ip[3]);
	rt_kprintf("  MAC: %X:%X:%X:%X:%X:%X\r\n", g_EthDP83848.mac[0], g_EthDP83848.mac[1], g_EthDP83848.mac[2],
											   g_EthDP83848.mac[3], g_EthDP83848.mac[4], g_EthDP83848.mac[5]);
	rt_kprintf("  Netmask: %d.%d.%d.%d\r\n", g_EthDP83848.netmask[0], g_EthDP83848.netmask[1],
											 g_EthDP83848.netmask[2], g_EthDP83848.netmask[3]);
	rt_kprintf("  Gateway: %d.%d.%d.%d\r\n", g_EthDP83848.gateway[0], g_EthDP83848.gateway[1],
											 g_EthDP83848.gateway[2], g_EthDP83848.gateway[3]);
	rt_kprintf("  DNS: %d.%d.%d.%d\r\n", g_EthDP83848.dns[0], g_EthDP83848.dns[1],
									   	 g_EthDP83848.dns[2], g_EthDP83848.dns[3]);
	rt_kprintf("  Remote IP: %s\r\n", REMOTE_ADDRESS);

	rt_kprintf("W5500:\r\n");
	rt_kprintf("  IPv4 Address: %d.%d.%d.%d\r\n", g_EthW5500.ip[0], g_EthW5500.ip[1],
												  g_EthW5500.ip[2], g_EthW5500.ip[3]);
	rt_kprintf("  MAC: %X:%X:%X:%X:%X:%X\r\n", g_EthW5500.mac[0], g_EthW5500.mac[1], g_EthW5500.mac[2],
											   g_EthW5500.mac[3], g_EthW5500.mac[4], g_EthW5500.mac[5]);
	rt_kprintf("  Netmask: %d.%d.%d.%d\r\n", g_EthW5500.netmask[0], g_EthW5500.netmask[1],
										   	 g_EthW5500.netmask[2], g_EthW5500.netmask[3]);
	rt_kprintf("  Gateway: %d.%d.%d.%d\r\n", g_EthW5500.gateway[0], g_EthW5500.gateway[1],
										   	 g_EthW5500.gateway[2], g_EthW5500.gateway[3]);
	rt_kprintf("  DNS: %d.%d.%d.%d\r\n", g_EthW5500.dns[0], g_EthW5500.dns[1],
									   	 g_EthW5500.dns[2], g_EthW5500.dns[3]);

}
FINSH_FUNCTION_EXPORT(ifconfig, net massage)


/**
  * @brief : �޸Ĵ�ӡ���Ϊ���ڻ򴮿�
  * @param : argv "-s"��ʾ����Ϊ���ڴ�ӡ;"-n"��ʾ����Ϊ���ڴ�ӡ
  * @return: none
  * @update: [2018-10-11][��  ��][����]
  */
void printfset(uint8_t* argv)
{
	if(argv == NULL)
	{
		if(true == NetFinshFlag)
		{
			rt_kprintf("The rt_kprintf function of the current device is network output\r\n");
			rt_kprintf("You can use the command 'printfset(\"-s\")' to set device to serial output\r\n");
		}
		else
		{
			rt_kprintf("The rt_kprintf function of the current device is serial output\r\n");
			rt_kprintf("You can use the command 'printfset(\"-n\")' to set device to network output\r\n");
		}
	}
	else
	{
		if(!strcmp(argv, "-s"))
		{
			rt_kprintf("SET OK!\r\n");
			NetFinshFlag = false;
			rt_kprintf("\r\nThe rt_kprintf function of the current device is serial output\r\n");
		}
		else if(!strcmp(argv, "-n"))
		{
			rt_kprintf("SET OK!\r\n");
			NetFinshFlag = true;
			rt_kprintf("\r\nThe rt_kprintf function of the current device is network output\r\n");
		}
		else
		{
			rt_kprintf("usage:\r\n");
			rt_kprintf("\tprintfset(\"-s\") : set serial output\r\n");
			rt_kprintf("\tprintfset(\"-n\") : set network output\r\n");
		}
	}

	return;
}
FINSH_FUNCTION_EXPORT(printfset, printf output set)

/*****************************File End**********************************/


