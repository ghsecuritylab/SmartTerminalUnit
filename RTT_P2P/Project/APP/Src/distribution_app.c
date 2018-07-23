/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_app.c
  * @brief:     分布式应用总入口
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018/7/21 
  * @update:    
  */

#include "distribution.h"

#include "extern_interface.h"

#ifdef UDP_SEND
extern ErrorCode ExternSend(PointUint8* pPacket);

/**
* @brief  : ，有动态分配
* @param  : 队列句柄
* @param  ：RingQueue* ring 环形队列
* @param  ：PointUint8* pPacket
* @return: ErrorCode
* @update: [2018-07-23][张宇飞][]
*/
ErrorCode Udp_SendPacketNode(DatagramTransferNode* node, PointUint8* pPacket)
{
    
    
	CHECK_POINT_RETURN(node, NULL, ERROR_NULL_PTR);
	CHECK_POINT_RETURN(pPacket, NULL, ERROR_NULL_PTR);

	
	//重新提取目的和源地址


	ErrorCode error = ExternSend(pPacket);
    
    //释放内存
	FREE(pPacket->pData);
	return error;
}


#endif


extern  void DistributionLogicalAppInit(void);
extern  void DistributionMutalAppInit(void);
extern  void LogicalSimulationAppInit(void);
extern void  UdpServerAppInit(void);


void DistributionAppInit(void)
{

	LogInit(&g_Loghandle);

	NodeFifo* nodefifo;
	StationMangerInit(&g_StationManger);
	rt_kprintf("DATA:%s,%s, %s, %d\n", __DATE__, __TIME__, __FUNCTION__, __LINE__);
	// TestListPrevCase();
	uint32_t id = 0x0; //虚拟管理成员，对应上位机



	ErrorCode error = RouterDatagram_NewTransferNode(LOCAL_ADDRESS, 100, &g_VirtualNode);
    g_VirtualNode.Send = Udp_SendPacketNode;
    
	if (error)
	{
		perror("RouterDatagram_NewTransferNode ： 0x%x", error);
	}

	// RingQueueTest();
	LogAddException(ERROR_OK_NULL, 0);

	LogicalSimulationAppInit();
	DistributionMutalAppInit();
	DistributionLogicalAppInit();
    UdpServerAppInit();
}

