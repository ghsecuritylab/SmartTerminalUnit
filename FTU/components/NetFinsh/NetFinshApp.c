/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.c
  * @brief:     ʹ������ӿڣ�UDPЭ��ʵ��Զ�̵�¼������finsh��Ӧ�ú���
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


/****************************ȫ�ֱ���***********************************/
struct netconn* g_NetFinshNetconn = NULL;			/* ����finsh��������Դ�ӡ��Ϣ��netconn�ӿھ�� */
bool NetFinshFlag = false;							/* ����finsh��������Դ�ӡ��Ϣ������UDPͨ����ͨ��־ */
struct netconn* g_UDP_ServeNetconn = NULL;			/* ����UDPͨ�ŷ����netconn�ӿھ�� */
bool UDP_ServeFlag = false;							/* ����UDPͨ�ŷ��������UDPͨ����ͨ��־ */
struct netconn* g_MaintenanceServeNetconn = NULL;	/* ����ά�������netconn�ӿھ�� */
bool MaintenanceServe = false;						/* ����ά�����������UDPͨ����ͨ��־ */

/* finsh����FIFO��� */
FifoHandle* FinshReceiveFifoHandle = NULL;
uint8_t* FinshBuffer = NULL;
PointUint8* FinshBufferPack = NULL;

/* ��ӡ���FIFO��� */
FifoHandle* PrintfFifoHandle = NULL;
uint8_t* PrintfBuffer = NULL;
PointUint8* PrintfBufferPack = NULL;

/* UDPͨ�ŷ���FIFO��� */
FifoHandle* UDP_ServeFifoHandle = NULL;
uint8_t* UDP_ServeBuffer = NULL;
PointUint8* UDP_ServeBufferPack = NULL;

/* ά������FIFO��� */
FifoHandle* MaintenanceServeFifoHandle = NULL;
uint8_t* MaintenanceServeBuffer = NULL;
PointUint8* MaintenanceServeBufferPack = NULL;

int32_t g_NetFinshSocket = 0;
int32_t g_UDP_ServeSocket = 0;
int32_t g_MaintenanceServeSocket = 0;


/*****************************Function**********************************/

/**
  * @brief : udp finsh thread IP address set
  * @param : lwip lwip��ַ���ýṹ��ָ��
  * @return: none
  * @update: [2018-09-10][��  ��][����]
  * 
  */
void DP83848_ServeIpSet(struct lwip_dev* lwip)
{
	uint8_t i = 0;
	
	/*����Զ��IP*/
	lwip->remoteip[0] = g_EthDP83848.remoteip[0];
	lwip->remoteip[1] = g_EthDP83848.remoteip[1];
	lwip->remoteip[2] = g_EthDP83848.remoteip[2];
	lwip->remoteip[3] = g_EthDP83848.remoteip[3];
	
	/*���ñ���IP*/
	lwip->ip[0] = g_EthDP83848.ip[0];
	lwip->ip[1] = g_EthDP83848.ip[1];
	lwip->ip[2] = g_EthDP83848.ip[2];
	lwip->ip[3] = g_EthDP83848.ip[3];
	
	/*������������*/
	lwip->netmask[0] = g_EthDP83848.netmask[0];
	lwip->netmask[1] = g_EthDP83848.netmask[1];
	lwip->netmask[2] = g_EthDP83848.netmask[2];
	lwip->netmask[3] = g_EthDP83848.netmask[3];
	
	/*��������*/
	lwip->gateway[0] = g_EthDP83848.gateway[0];
	lwip->gateway[1] = g_EthDP83848.gateway[1];
	lwip->gateway[2] = g_EthDP83848.gateway[2];
	lwip->gateway[3] = g_EthDP83848.gateway[3];
	
	/*����MAC��ַ*/
	lwip->mac[0] = g_EthDP83848.mac[0];
	lwip->mac[1] = g_EthDP83848.mac[1];
	lwip->mac[2] = g_EthDP83848.mac[2];
	lwip->mac[3] = g_EthDP83848.mac[3];
	lwip->mac[4] = g_EthDP83848.mac[4];
	lwip->mac[5] = g_EthDP83848.mac[5];
	
	lwip->dhcpstatus = g_EthDP83848.dhcpstatus;
	
}


/**
  * @brief : ʹ��UDPЭ�飬netconn�ӿ���PC��������Ϣ
  * @param : udpconn netconn�ӿڵĽṹ��ָ��
  * @param : sendBuf Ҫ���͵��ַ���
  * @return: 0���ɹ�  1�����δ���  2��netbuf����ʧ�ܻ�̬�ڴ�����ʧ��  ���������ʹ���
  * @update: [2018-09-10][��  ��][����]
  * 
  */
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString)
{
	struct netbuf* sendNetbuf = NULL;
	void* pointer = NULL;
	int8_t error = 0;

	sendNetbuf = netbuf_new();
	netbuf_alloc(sendNetbuf,strlen((char *)sendString));
	memcpy(sendNetbuf->p->payload,(void*)sendString,strlen((char*)sendString));
	error = netconn_send(udpNetconn, sendNetbuf);
	if(error != ERR_OK)
	{
		rt_kprintf("send error\r\n");
		netbuf_delete(sendNetbuf);
	}
	netbuf_delete(sendNetbuf);

	
	return 0;
}


/**
  * @brief : ʹ��UDPЭ�飬netconn�ӿڽ�������PC�����ַ���
  * @param : udpconn netconn�ӿڵĽṹ��ָ��
  * @param : fifoHandle Ҫ��Ķ���ָ��
  * @return: ���յ����ַ���
  * @update: [2018-09-10][��  ��][����]
  * 
  */
uint32_t UDP_NetconnReceiveString(struct netconn* udpNetConn, FifoHandle* fifoHandle)
{
	static struct netbuf* recvNetBuf = NULL;
	rt_base_t level;			/*�����жϵķ���ֵ*/
	struct pbuf* q = NULL;
	uint32_t dataLenth = 0;
	
	netconn_recv(udpNetConn, &recvNetBuf); //��������
	if(recvNetBuf != NULL)          //���յ�����
	{
		level = rt_hw_interrupt_disable(); //���ж�
		
		for(q=recvNetBuf->p; q!=NULL; q=q->next)  //����������pbuf����
		{
			//�ж�Ҫ������fifoHandle�е������Ƿ����NET_FINSH_BUFSIZE��ʣ��ռ�,�������
			//�Ļ���ֻ����fifoHandle��ʣ�೤�ȵ�����,����Ļ����������е�����
			if(q->len > (NET_FINSH_BUFSIZE-dataLenth))
			{
				FifoStringEnqueue(fifoHandle, q->payload, (NET_FINSH_BUFSIZE-dataLenth));
			}
			else
			{
				FifoStringEnqueue(fifoHandle, q->payload, q->len);
			}
			dataLenth += q->len;	
			if(dataLenth > NET_FINSH_BUFSIZE)
			{
				dataLenth = NET_FINSH_BUFSIZE;
				break; //����TCP�ͻ��˽�������,����
			}
		}
		rt_hw_interrupt_enable(level);  //���ж�
		netbuf_delete(recvNetBuf);      //ɾ��buf
	}
	return dataLenth;
}


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
  * @brief : ʹ��UDP�Ĵ�ӡ����
  * @param : ��ӡ����Ϣ
  * @return: none
  * @update: [2018-09-11][��  ��][����]
  */
void NetFinsh_kprintf(const char *fmt, ...)
{
	va_list args;
    rt_size_t length;
    char rt_log_buf[RT_CONSOLEBUF_SIZE] = {0};

    va_start(args, fmt);
	
	length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;

	if(true == NetFinshFlag)		/*UDP_FinshFlagΪ1˵��UDP��ӡ�Ѿ���ʼ��������ʹ����*/
	{
		FifoStringEnqueue(PrintfFifoHandle, (uint8_t*)rt_log_buf, length);
	}
	else
	{
		#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
			static rt_device_t _console_device = RT_NULL;
		#endif

		_console_device = rt_console_get_device();			/*��ȡ��ǰ����̨����Ĵ����豸��*/
		rt_uint16_t old_flag = _console_device->open_flag;

        _console_device->open_flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(_console_device, 0, rt_log_buf, length);
        _console_device->open_flag = old_flag;
		
	}
	
	va_end(args);
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

/*****************************File End**********************************/


