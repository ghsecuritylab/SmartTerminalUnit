/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_transfer.c
* @brief:     用于转供电的各种操作
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-12
* @update:    
*/

#include "distribution_interface.h"   
#include "distribution.h"
#include "common_def.h"
#include "topology.h" 
#include "list.h"
#include "distribution_transfer.h"

static ErrorCode Transfer_ControlSend(ListDouble* listcp, FaultDealHandle* handle);
static ErrorCode Transfer_CapacitySend(FaultDealHandle* handle);
/**
* @brief :故障处理——转供电
* @param
* @return: 0-正常
* @update: [2018-07-12][张宇飞][BRIEF]
* [2018-07-16][张宇飞][补全判断漏洞]
*/
StateResult TransferPowerSupply_Master(FaultDealHandle* handle)
{
	SwitchProperty* switchProperty = handle->switchProperty;
	StationTopology* stationTopology = switchProperty->parent;
	ListDouble* Listcp = &(stationTopology->connectPath);
	ErrorCode error;
	if (handle->nextState != TRANSFER_MASTER)
	{
		return RESULT_ERROR_MATCH;
	}
	handle->state = TRANSFER_MASTER;

	switch (handle->step)
	{
	case 0: //初始化状态
	{		
		handle->limitTime = handle->reciveConnectTime;
		handle->GetNowTime(handle);
		
		error = ConnectPath_ResetUpdateFlag(Listcp);
		if (error)
		{
			perror("ConnectPath_ResetUpdateFlag Error: 0x%X", error);
			return RESULT_ERROR;
		}
		stationTopology->isConnectPathUpdatedComplted = false;

		handle->nextState = TRANSFER_MASTER;
		handle->step = 1;
		break;
	}
	case 1:
	{		
		//收集全面
		if (stationTopology->isConnectPathUpdatedComplted)
		{
			error = Transfer_ControlSend(Listcp, handle);
			if (error)
			{
				perror("Transfer_ControlSend Error: 0x%X", error);
				return RESULT_ERROR;
			}
			PrintIDTipsTick(handle->switchProperty->id, "Transfer Master Send Cmd.");
			handle->nextState = DISTRIBUTION_LOCK;
			break;
		}

		if (handle->IsOverTime(handle))
		{
			PrintIDTipsTick(handle->switchProperty->id, "Transfer Master Failure: receive overTime");
			handle->nextState = DISTRIBUTION_LOCK;
			break;
		}
		break;
	}
	}


	handle->lastState = handle->state;
	return RESULT_NULL;
}



/**
* @brief :故障处理——转供电
* @param
* @return: 0-正常
* @update: [2018-07-13][张宇飞][BRIEF]
*/
StateResult TransferPowerSupply_Connect(FaultDealHandle* handle)
{	
	SwitchProperty* switchProperty = handle->switchProperty;
	StationTopology* stationTopology = switchProperty->parent;
	ConnectSwitch* pconnect = &(stationTopology->connect);
	ErrorCode error;
	CHECK_UNEQUAL_RETURN_LOG(handle->nextState, TRANSFER_CONNECT, RESULT_ERROR_MATCH, switchProperty->id);	
	handle->state = TRANSFER_CONNECT;

	switch (handle->step)
	{
	case 0: //初始化状态
	{
		handle->limitTime = handle->reciveConnectTime;
		handle->GetNowTime(handle);		
		pconnect->transferCode = TRANSFER_NULL;
		error = Transfer_CapacitySend(handle);
		if (error)
		{
			perror("Transfer_CapacitySend Error: 0x%X", error);
			return RESULT_ERROR;
		}		
		handle->nextState = TRANSFER_CONNECT;
		handle->step = 1;
		break;
	}
	case 1:
	{
		if ( pconnect->transferCode)
		{
			//执行合闸转供命令
			if (pconnect->transferCode == TRANSFER_SET)
			{				
				handle->CloseOperate(handle);//TODO:简单执行合闸，需要完善动作逻辑
				handle->limitTime = handle->rejectTime;
				handle->GetNowTime(handle);				
				PrintIDTipsTick(handle->switchProperty->id, "Transfer Close Operate");
				handle->nextState = TRANSFER_CONNECT;
				handle->step = 2;
			}
			else
			{
				PrintIDTipsTick(handle->switchProperty->id, "Transfer Connect Cacer");
				handle->nextState = DISTRIBUTION_LOCK;
			}
			break;
		}

		if (handle->IsOverTime(handle))
		{
			PrintIDTipsTick(handle->switchProperty->id, "Transfer Over Time Cancer");
			handle->nextState = DISTRIBUTION_LOCK;
			break;
		}


		break;
	}
	case 2: //合闸超时检测
	{
		if (handle->IsClosePosition(handle))
		{
			PrintIDTipsTick(handle->switchProperty->id, "Transfer Success");
			handle->nextState = DISTRIBUTION_LOCK;
			break;
		}


		if (handle->IsOverTime(handle))
		{
			PrintIDTipsTick(handle->switchProperty->id, "Close reject");
			handle->nextState = DISTRIBUTION_LOCK;
			break;
		}

		break;
	}

	}


	handle->lastState = handle->state;
	return RESULT_NULL;
}
/**
* @brief :转供电——控制发送,不进行参数检查
* @param
* @return: 0-正常
* @update: [2018-07-12][张宇飞][BRIEF]
*/
static ErrorCode Transfer_ControlSend(ListDouble* listcp, FaultDealHandle* handle)
{
	ErrorCode error;
	ConnectPath* maxcp;
	ConnectPath* 	cp;
	error = ConnectPath_MeetCondition(listcp, &maxcp);
	if (error)
	{	
		return error;
	}

	FOR_EARCH_LIST_START(listcp);

	cp = ((ConnectPath*)list_data(m_foreach));

	if (cp == maxcp)
	{
		error = TransmitTransferMessageExtern(handle->pTransferNode, TRANSFER_SET, cp->id, GET_UINT16(cp->id));
	}
	else
	{
		error = TransmitTransferMessageExtern(handle->pTransferNode, TRANSFER_CANCER, cp->id, GET_UINT16(cp->id));
	}
	if (error)
	{
		return error;
	}
	FOR_EARCH_LIST_END();


	return ERROR_OK_NULL;	
}


/**
* @brief :转供电——容量发送,不进行参数检查
* @param
* @return: 0-正常
* @update: [2018-07-13][张宇飞][BRIEF]
*/
static ErrorCode Transfer_CapacitySend( FaultDealHandle* handle)
{
	ListDouble* list = &(handle->switchProperty->parent->globalSwitchList);
	SwitchProperty* node;

	FOR_EARCH_LIST_START(list);
	node = GET_SWITCH_ELEMENT(m_foreach);
	if (node->insulateType == RESULT_SUCCESS)
	{
		TransmitTransferMessageExtern(handle->pTransferNode, TRANSFER_REMAINDER_CAP, handle->switchProperty->id, GET_UINT16(node->id));
	}
	FOR_EARCH_LIST_END();
	return ERROR_OK_NULL;
}
	
/**
* @brief :转供电——解析处理
* @param
* @return: 0-正常
* @update: [2018-07-12][张宇飞][BRIEF]
*/
ErrorCode Transfer_ParseDeal(uint8_t* pdata, uint16_t len,  StationTopology* station)
{
	CHECK_POINT_RETURN_LOG(station, NULL, ERROR_NULL_PTR, 0);
	CHECK_POINT_RETURN_LOG(pdata, NULL, ERROR_NULL_PTR, station->id);
	CHECK_EQUAL_RETURN_LOG(len, 0, ERROR_LEN, station->id);
	ErrorCode error = ERROR_OK_NULL;
	uint32_t id = 0, data = 0;
	ConnectPath* cp;
	TransferCode code = pdata[0];
	switch (code)
	{
	case  TRANSFER_REMAINDER_CAP:
	{
		CHECK_UNEQUAL_RETURN_LOG(len, 9, ERROR_LEN, station->id);
		id = COMBINE_UINT32(pdata[4], pdata[3], pdata[2], pdata[1]);
		data = COMBINE_UINT32(pdata[8], pdata[7], pdata[6], pdata[5]);
		ListDouble* list = &(station->connectPath);

		FOR_EARCH_LIST_START(list);

		cp = ((ConnectPath*)list_data(m_foreach));

		if (cp->id == id)
		{
			cp->isUpdated = true;
			cp->remainderCapacity = data;
		}


		FOR_EARCH_LIST_END();

		//检测是否更新完毕
		error = ConnectPath_CheckIsUpdateComplete(list, &(station->isConnectPathUpdatedComplted));
		break;
	}


	case  TRANSFER_SET:
	case  TRANSFER_CANCER:
	{
		CHECK_UNEQUAL_RETURN_LOG(len, 5, ERROR_LEN, station->id);
		id = COMBINE_UINT32(pdata[4], pdata[3], pdata[2], pdata[1]);
		if ((station->id == id) && (station->connect.isConnect))
		{
			station->connect.transferCode = code;
		}

		break;
	}
	default:
		break;
	}

	return error;
}
