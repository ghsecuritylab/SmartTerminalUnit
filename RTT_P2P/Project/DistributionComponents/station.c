/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      log.c
* @brief:     记录日志信息等
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:       2018/7/24 创建
* @update 2018/7/14: 转移--增加状态信息内容
*/
#include "station.h"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "station.pb.h"


#include "distribution_enum.h"
#include "helper.h"

#include "log.h"

#include "RingQueue.h"




#include "distribution.h"
#include "extern_interface.h"




static ErrorCode AssignmentStationMessage_AreaID(StationMessage* pmessage, uint32_t* pdata, uint8_t count, bool flagComplted);
static ErrorCode AssignmentStationMessage_SwitchProperty(node_property* pdest, SwitchProperty* psrc);

static ErrorCode AssignmentStationMessage_ConnectSwitch(connect_switch* pdest, const ConnectSwitch* const pcs);
static ErrorCode AssignmentStationMessage_ConnectPath(connect_path* pdest, const ListDouble* listPath);

static ErrorCode AssignmentStationMessage_DistributionStation(distribution_station* pstation,
	DistributionStation* pdistribution);
static ErrorCode AssignmentStationMessage_PowerArea(distribution_power_area* ppower,
	DistributionStation* pdistribution);
static ErrorCode AssignmentStationMessage_FaultDealHandle(faultdeal_handle* pdest,
	const FaultDealHandle* const psrc);




/**
* @brief :解码站点信息
* @param ：
* @update: [2018-06-30][张宇飞][]
*/
ErrorCode PacketDecodeStationMessage(AreaID* area, uint8_t* data, uint16_t len)
{

    CHECK_POINT_RETURN_LOG(area, NULL, ERROR_OK_NULL, 0);
    CHECK_POINT_RETURN_LOG(data, NULL, ERROR_OK_NULL, 0);

    uint16_t nanolen = COMBINE_UINT16(data[2], data[1]);
    if (nanolen + 2 > len)
    {
        return ERROR_OVER_LIMIT;
    }
    StationMessage message = StationMessage_init_zero;
    pb_istream_t instreamLog = pb_istream_from_buffer(data + 3, nanolen);
    bool status = pb_decode(&instreamLog, StationMessage_fields, &message);
    if (!status)
    {
        return ERROR_ENCODE;
    }

    if (message.idCollect_count > 0)
    {
        if (area->count > 0)
        {
            area->idCollect = REALLOC(area->idCollect, message.idCollect_count * sizeof(uint32_t));
            CHECK_POINT_RETURN_LOG(area->idCollect, NULL, ERROR_MALLOC, 0);
        }
        else
        {
            area->idCollect = MALLOC(message.idCollect_count * sizeof(uint32_t));
            CHECK_POINT_RETURN_LOG(area->idCollect, NULL, ERROR_MALLOC, 0);
        }
        area->count = message.idCollect_count;
        for (uint8_t i = 0; i < message.idCollect_count; i++)
        {
            area->idCollect[i] = message.idCollect[i];
        }
    }


    
    return ERROR_OK_NULL;
}

/**
* @brief :编码站点信息,默认长度1024
* @param ：
* @update: [2018-06-30][张宇飞][]
*/
ErrorCode PacketEncodeStationMessage(uint32_t* pdata, uint8_t count, PointUint8* packet, uint16_t addLen, uint16_t offset)
{
    uint16_t defaultLen = 1024;

    CHECK_POINT_RETURN(pdata, NULL, ERROR_NULL_PTR);
    uint16_t len = defaultLen + addLen;
    packet->pData = (uint8_t*)CALLOC(len, sizeof(uint8_t));
    CHECK_POINT_RETURN(packet->pData, NULL, ERROR_NULL_PTR);
    CHECK_EQUAL_RETURN(len, 0, ERROR_OVER_LIMIT);

    StationMessage message = StationMessage_init_zero;

    for (uint8_t i = 0; i < count; i++)
    {
        message.idCollect[i] = pdata[i];

    }
    message.idCollect_count = count;

    pb_ostream_t stream = pb_ostream_from_buffer(packet->pData + offset + 4, defaultLen);
    bool status = pb_encode(&stream, StationMessage_fields, &message);
    if (!status)
    {
        SafeFree(packet->pData);
        return ERROR_ENCODE;
    }
    uint16_t realLen = stream.bytes_written + addLen + 4;
    if (realLen != len)
    {
        REALLOC(packet->pData, realLen);//调小到实际长度
    }
    packet->pData[offset++] = NANOPB_TYPE;
    packet->pData[offset++] = STATION_MESSAGE;
    packet->pData[offset++] = GET_N_BYTE(stream.bytes_written, 0);
    packet->pData[offset++] = GET_N_BYTE(stream.bytes_written, 1);
    packet->len = realLen;
    return ERROR_OK_NULL;
}


/**
* @brief :赋值 AreaID,不进行参数检查
* @param ：
* @update: [2018-07-14][张宇飞][]
*/
static ErrorCode AssignmentStationMessage_AreaID(StationMessage* pmessage, uint32_t* pdata, uint8_t count, bool flagComplted)
{
	
	for (uint8_t i = 0; i < count; i++)
	{
		pmessage->idCollect[i] = pdata[i];
	}
	pmessage->idCollect_count = count;
	pmessage->isGainComplted = flagComplted;
	return ERROR_OK_NULL;
}

/**
* @brief :赋值 connect_switch,不进行参数检查
* @param ：
* @update: [2018-07-14][张宇飞][]
*/
static ErrorCode AssignmentStationMessage_ConnectSwitch(connect_switch* pdest,const ConnectSwitch* const pcs)
{	
	const ListDouble* const list = pcs->path;
	pdest->count = pcs->count;
	pdest->has_count = true;
	pdest->transferCode = pcs->transferCode;
	pdest->has_transferCode = true;
	pdest->isConnect = pcs->isConnect;

	//路径1

	pdest->path1_count = list_size(list);
	if (pdest->path1_count > sizeof(pdest->path1)/sizeof(uint32_t))
	{
		perror("Over Max Len");
		return ERROR_OVER_LIMIT;
	}
	FOR_EARCH_LIST_START(list);

	pdest->path1[iii] = GET_SWITCH_ELEMENT(m_foreach)->id;

	FOR_EARCH_LIST_END();


	//路径2
	const ListDouble* const list2  = pcs->path + 1;
	pdest->path2_count = list_size(list2);
	if (pdest->path2_count > sizeof(pdest->path1) / sizeof(uint32_t))
	{
		perror("Over Max Len\n");
		return ERROR_OVER_LIMIT;
	}
	FOR_EARCH_LIST_START(list2);

	pdest->path2[iii] = GET_SWITCH_ELEMENT(m_foreach)->id;

	FOR_EARCH_LIST_END();

	return ERROR_OK_NULL;
}

/**
* @brief :赋值 connect_path,不进行参数检查
* @param ：connect_path* pdest
* @param ：ListDouble* listPath
* @update: [2018-07-14][张宇飞][]
*[2018-07-24][张宇飞][修改形参为列表形式]
*/
static ErrorCode AssignmentStationMessage_ConnectPath(connect_path* pdest,const ListDouble* listPath)
{
	uint8_t size = list_size(listPath);
	if (size > sizeof(pdest) / sizeof(connect_path))
	{
		perror("Over Max Len\n");
		return ERROR_OVER_LIMIT;
	}
	FOR_EARCH_LIST_START(listPath);
	{
		ConnectPath* ppath = (ConnectPath*)list_data(m_foreach);
		pdest->id = ppath->id;
		pdest->has_id = true;
		pdest->isUpdated = ppath->isUpdated;
		pdest->remainderCapacity = ppath->remainderCapacity;
		pdest->has_remainderCapacity = true;
		pdest++;
	}
	FOR_EARCH_LIST_END();
	
	
    return ERROR_OK_NULL;
}

/**
* @brief :赋值 distribution_station,不进行参数检查
* @param ：
* @update: [2018-07-14][张宇飞][]
*/
static ErrorCode AssignmentStationMessage_DistributionStation(distribution_station* pstation,
	DistributionStation* pdistribution)
{
	//配电管理点
	pstation->areaCount = pdistribution->areaCount;
	pstation->has_areaCount = true;
		
	pstation->isComplted = pdistribution->isComplted;
	pstation->isGatherCompleted = pdistribution->isGatherCompleted;
	pstation->isGatherCalculateCompleted = pdistribution->isGatherCalculateCompleted;
	
	pstation->switchRef = pdistribution->switchRef->id;
	pstation->has_switchRef = true;
	
	pstation->isAlreayExitedFault = pdistribution->isAlreayExitedFault;
	pstation->isExitedInsulateFailure = pdistribution->isExitedInsulateFailure;

	return ERROR_OK_NULL;

}

/**
* @brief :赋值 distribution_power_area,不进行参数检查
* @param ：
* @update: [2018-07-14][张宇飞][]
*/
static ErrorCode AssignmentStationMessage_PowerArea(distribution_power_area* ppower,
	DistributionStation* pdistribution)
{	
	//单个配电区域
	DistributionPowerArea* powerArea = pdistribution->powerArea;
	for (uint8_t i = 0; i < pdistribution->areaCount; i++, ppower++, powerArea++)
	{
		ppower->areaSwitch_count = powerArea->switchNum;
		if (ppower->areaSwitch_count > sizeof(ppower->areaSwitch)/sizeof(uint32_t))
		{
			perror("Over Max Len");
			return ERROR_OVER_LIMIT;
		}
		for (uint8_t i =0 ; i < ppower->areaSwitch_count; i++)
		{
			ppower->areaSwitch[i] = (powerArea->areaSwitch[i])->id;
		}
		ppower->upadetedFlag = powerArea->upadetedFlag;
		ppower->has_upadetedFlag = true;

		ppower->isUpadeted = powerArea->isUpadeted;
		ppower->isFaultArea = powerArea->isFaultArea;
		ppower->isExitFaultMessage = powerArea->isExitFaultMessage;

		ppower->upadetedFull = powerArea->upadetedFull;
		ppower->has_upadetedFull = true;
		ppower->removalType = powerArea->removalType;
		ppower->has_removalType = true;
		ppower->removalType = powerArea->removalType;
		ppower->has_removalType = true;

	}

	return ERROR_OK_NULL;
}


/**
* @brief :赋值 faultdeal_handle,不进行参数检查
* @param ：
* @update: [2018-07-14][张宇飞][]
*/
static ErrorCode AssignmentStationMessage_FaultDealHandle(faultdeal_handle* pdest ,
	const FaultDealHandle* const psrc)
{
	pdest->switchPropertyID = psrc->switchProperty->id;
	pdest->has_switchPropertyID = true;
	pdest->state = psrc->state;
	pdest->has_state = true;
	pdest->lastState = psrc->lastState;
	pdest->has_lastState = true;
	pdest->nextState = psrc->nextState;
	pdest->has_nextState = true;

	pdest->isRun = psrc->isRun;

	pdest->step = psrc->step;
	pdest->has_step = true;
	pdest->startTime = psrc->startTime;
	pdest->has_startTime = true;
	pdest->startTime = psrc->startTime;
	pdest->has_startTime = true;
	pdest->limitTime = psrc->limitTime;
	pdest->has_limitTime = true;
	pdest->t1 = psrc->t1;
	pdest->has_t1 = true;
	pdest->t2 = psrc->t2;
	pdest->has_t2 = true;
	pdest->t3 = psrc->t3;
	pdest->has_t3 = true;
	pdest->t4 = psrc->t4;
	pdest->has_t4 = true;
	pdest->t5 = psrc->t5;
	pdest->has_t5 = true;

	pdest->checkOpenTime = psrc->checkOpenTime;
	pdest->has_checkOpenTime = true;
	pdest->checkBackupTime = psrc->checkBackupTime;
	pdest->has_checkBackupTime = true;
	pdest->rejectTime = psrc->rejectTime;
	pdest->has_rejectTime = true;
	pdest->reciveRemovalSuccessTime = psrc->reciveRemovalSuccessTime;
	pdest->has_reciveRemovalSuccessTime = true;
	pdest->reciveConnectTime = psrc->reciveConnectTime;
	pdest->has_reciveConnectTime = true;

	pdest->isCheckPass = psrc->isCheckPass;


	return ERROR_OK_NULL;
}

/**
* @brief :赋值 node_property,不进行参数检查
* @param ：
* @update: [2018-07-14][张宇飞][]
*/
static ErrorCode AssignmentStationMessage_SwitchProperty(node_property* pdest,
	SwitchProperty* psrc)
{
	pdest->id = psrc->id;
	pdest->has_id = true;
	pdest->type = psrc->type;
	pdest->has_type = true;
	pdest->state = psrc->state;
	pdest->has_state = true;
	
	pdest->neighbourCollect_count = psrc->neighbourNum;

	if (pdest->neighbourCollect_count > sizeof(pdest->neighbourCollect)/ sizeof(uint32_t))
	{
		perror("Over Max Len");
		return ERROR_OVER_LIMIT;
	}

	MEMCPY(pdest->neighbourCollect, psrc->neighbourCollect, psrc->neighbourNum);


	pdest->operateType = psrc->operateType;
	pdest->has_operateType = true;
	pdest->overTimeType = psrc->overTimeType;
	pdest->has_overTimeType = true;
	pdest->removalType = psrc->removalType;
	pdest->has_removalType = true;
	pdest->insulateType = psrc->insulateType;
	pdest->has_insulateType = true;
	pdest->faultState = psrc->fault.state;
	pdest->has_faultState = true;

	pdest->isFaultEdgeConnected = psrc->fault.isFaultEdgeConnected;
	
	uint8_t count = sizeof(psrc->indexArea) / sizeof(uint8_t);
	if (sizeof(pdest->indexArea)/ sizeof(uint32_t) > count)
	{
		perror("Over Max Len");
		return ERROR_OVER_LIMIT;
	}

	for (uint8_t i = 0; i < count; i++)
	{
		pdest->indexArea[i] = psrc->indexArea[i];
		pdest->isExitArea[i] = psrc->isExitArea[i];
		pdest->isExitArea[i] = psrc->isExitArea[i];
	}
	pdest->indexArea_count = count;
	pdest->isExitArea_count = count;
	pdest->isExitArea_count = count;
	return ERROR_OK_NULL;
}
/**
* @brief :编码站点信息,编码所有信息
* @param ：
* @update: [2018-07-14][张宇飞][]
[2018-07-25][张宇飞][修正赋值长度错误]
*/
ErrorCode PacketEncodeStationMessage_All(const StationPoint* const point, PointUint8* packet, uint16_t addLen, uint16_t offset)
{
	uint16_t defaultLen = 1024;
	ErrorCode  error = ERROR_OK_NULL; 
	CHECK_POINT_RETURN(point, NULL, ERROR_NULL_PTR);
	uint16_t len = defaultLen + addLen;
	packet->pData = (uint8_t*)CALLOC(len, sizeof(uint8_t));
	CHECK_POINT_RETURN(packet->pData, NULL, ERROR_NULL_PTR);
	CHECK_EQUAL_RETURN(len, 0, ERROR_OVER_LIMIT);


	StationMessage message = StationMessage_init_zero;

	//AreaID
	const AreaID* const area = &(point->topology.areaID);
	const StationTopology* const pTopology = &(point->topology);


	error = AssignmentStationMessage_AreaID(&message, area->idCollect, area->count, area->isGainComplted);
	if (error)
	{
		return error;
	}

	if (pTopology->localSwitch != NULL)
	{
		error = AssignmentStationMessage_SwitchProperty(&(message.node), pTopology->localSwitch);
		if (error)
		{
			return error;
		}
		message.has_node = true;


		DistributionStation* pDistribution = pTopology->localSwitch->distributionArea;
		if (pDistribution != NULL)
		{
			error = AssignmentStationMessage_DistributionStation(&(message.distribution), pDistribution);
			if (error)
			{
				return error;
			}
			message.has_distribution = true;

			error = AssignmentStationMessage_PowerArea(message.power_area, pDistribution);
            message.power_area_count = pDistribution->areaCount;
			if (error)
			{
				return error;
			}
		}
		else
		{
			message.has_distribution = false;
			message.power_area_count = 0;
		}
	}
	else
	{
		message.has_node = false;
	}
	
	error = AssignmentStationMessage_ConnectSwitch(&(message.connect), &(pTopology->connect));
	if (error)
	{
		return error;
	}
	message.has_connect = true;

  
	error = AssignmentStationMessage_ConnectPath(message.con_path, &(pTopology->connectPath));
	if (error)
	{
		return error;
	}
	message.con_path_count = list_size(&(pTopology->connectPath));
	

	error = AssignmentStationMessage_FaultDealHandle(&(message.fault_handle), &(point->removalHandle));
	if (error)
	{
		return error;
	}
	message.has_fault_handle = true;
	

	pb_ostream_t stream = pb_ostream_from_buffer(packet->pData + offset + 4, defaultLen);
	bool status = pb_encode(&stream, StationMessage_fields, &message);
	if (!status)
	{
		SafeFree(packet->pData);
		return ERROR_ENCODE;
	}
	uint16_t realLen = stream.bytes_written + addLen + 3;
	if (realLen != len)
	{
		REALLOC(packet->pData, realLen);//调小到实际长度
	}
	packet->pData[offset++] = NANOPB_TYPE;
	packet->pData[offset++] = STATION_MESSAGE;
	packet->pData[offset++] = GET_N_BYTE(stream.bytes_written, 0);
	packet->pData[offset++] = GET_N_BYTE(stream.bytes_written, 1);
	packet->len = realLen;
	return ERROR_OK_NULL;
}

/**
* @brief :编码站点信息
* @param ：
* @update: [2018-07-14][张宇飞][]
*[2018-07-19][张宇飞][修改发送细节]
*/
ErrorCode  TransmitEncodeStationMessage_All(StationPoint* point, uint16_t destAddress)
{
	ErrorCode result = ERROR_OK_NULL;
	PointUint8 packet;
		
	CHECK_POINT_RETURN(point, NULL, ERROR_NULL_PTR);
	
	DatagramTransferNode*  pTransferNode = &(point->transferNode);


	result = PacketEncodeStationMessage_All(point, &packet, FRAME_MIN_LEN, FRAME_FUNCODE_INDEX);

	if (result)
	{
		return result;
	}

	result = MakePacketMessage(&packet, destAddress, GET_UINT16(pTransferNode->id));
	if (result)
	{
		return result;
	}
	result = pTransferNode->Send(pTransferNode, &packet);

	
	
	return result;
	
}

/**
* @brief :解析NanoPb Type
* @param ：
* @update: [2018-07-05][张宇飞][]
* [2018-07-14][张宇飞][修改接口，站点信息获取]
*/
ErrorCode ParseNanopb(StationPoint* point, uint16_t sourceAddress, uint8_t* pdata, uint8_t len)
{
	ErrorCode error = ERROR_OK_NULL;
	CHECK_POINT_RETURN_LOG(pdata, NULL, ERROR_NULL_PTR, 0);

	switch ((NanopbType)(pdata[0]))
	{
	case STATION_MESSAGE:
		error = PacketDecodeStationMessage(&(point->topology.areaID), pdata, len);
		break;
	case NANOPB_GET_STATION:
		error = TransmitEncodeStationMessage_All(point, sourceAddress);
		break;
	default:
	{
		error = ERROR_UNFIND;
		break;
	}
	}
	return error;

}

