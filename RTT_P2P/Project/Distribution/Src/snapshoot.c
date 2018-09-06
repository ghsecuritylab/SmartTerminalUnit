/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      snapshoot.c
* @brief:     判断自身开关信息
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-09-06
* @update:    
*/

#include "snapshoot.h"
#include "distribution_interface.h"   
#include "distribution.h"
#include "common_def.h"
#include "topology.h" 
#include "list.h"

static bool ConnectSwitch_Clone(ConnectSwitch* from, ConnectSwitch* to);
static bool ListCopy(ListDouble* from, ListDouble* to);
static bool ListDeepCopy_ConnectPath(ListDouble* from, ListDouble* to);
static void SwitchSnapshootPrint(SwitchSnapshoot* snap);

/**
* @brief : 快照打印输出
* @param : ListDouble* from
* @param : ListDouble* to
* @return:
* @update: [2018-09-06][张宇飞][创建]
*/
static void SwitchSnapshootPrint(SwitchSnapshoot* snap)
{
	uint8_t data[64];
	//rt_kpritnf("", snap->);


}


/**
* @brief : 链表拷贝
* @param : ListDouble* from
* @param : ListDouble* to
* @return:
* @update: [2018-09-06][张宇飞][创建]
*/
static bool ListCopy(ListDouble* from, ListDouble* to)
{
	FOR_EARCH_LIST_START(from)
	ListInsertNext(to, NULL, GET_SWITCH_ELEMENT(m_foreach));
	FOR_EARCH_LIST_END()
    return true;
}
/**
* @brief : 链表拷贝
* @param : ListDouble* from
* @param : ListDouble* to
* @return:
* @update: [2018-09-06][张宇飞][创建]
*/
static bool ListDeepCopy_ConnectPath(ListDouble* from, ListDouble* to)
{
	FOR_EARCH_LIST_START(from)

	ConnectPath* cp = (ConnectPath*)CALLOC(1, sizeof(ConnectPath));
	if (!cp)
	{
		perror("CALLOC is null\n");
		return false;
	}
	MEMCPY(list_data(m_foreach), cp, sizeof(ConnectPath));

	ListInsertNext(to, NULL, cp);
	FOR_EARCH_LIST_END();
    return true;
}
/**
* @brief : 复制ConnectSwitch
* @param :GetNeighboorHandle* handle
* @return:
* @update: [2018-09-06][张宇飞][创建]
*/
static bool ConnectSwitch_Copy(ConnectSwitch* from, ConnectSwitch* to)
{
	to->isLock = from->isLock;
	to->isConnect = from->isConnect;
	to->count = from->count;
	to->transferCode = from->transferCode;
	ListCopy(from->path, to->path);
	ListCopy(from->path +1 , to->path + 1);

    return true;
}

/**
* @brief : 保存开关状态信息
* @param :GetNeighboorHandle* handle
* @return:
* @update: [2018-09-06][张宇飞][创建]
*/
SwitchSnapshoot* SnapshootSwitchProperty(StationTopology* st)
{
	SwitchSnapshoot* ss = (SwitchSnapshoot*)CALLOC(1, sizeof(SwitchSnapshoot));
	if (!ss)
	{
		perror("CALLOC is NULL");
		return NULL;
	}

	ss->time = rt_tick_get();
	ss->isFaultEdgeConnected = st->localSwitch->fault.isFaultEdgeConnected;

	ListInit(ss->connect.path, NULL);
	ListInit(ss->connect.path + 1, FREE);

	bool state = ConnectSwitch_Copy(&st->connect, &ss->connect);
	if (!state)
	{
		FREE(ss);
		return NULL;
	}

	ListInit(&ss->connectPath, FREE);
	state = ListDeepCopy_ConnectPath(&st->connectPath, &ss->connectPath);
	if (!state)
	{
		FREE(ss);
		return NULL;
	}



	return ss;
}


/**
* @brief : 保存开关状态信息
* @param :GetNeighboorHandle* handle
* @return:
* @update: [2018-09-06][张宇飞][创建]
*/
void SwitchSnapshoot_Destory(SwitchSnapshoot** ss)
{
	if (! *ss)
	{
		return;
	}
	Listdestroy(&(*ss)->connectPath);
	Listdestroy((*ss)->connect.path);
	Listdestroy((*ss)->connect.path + 1);
	FREE(*ss);
	*ss = NULL;
}

/**
* @brief : 站点快照, 不进行覆盖
* @param :StationTopology* pTopology
* @return: void
* @update: [2018-09-06][张宇飞][创建]
*/
ErrorCode Station_Snapshoot(StationTopology* pTopology)
{
	CHECK_POINT_RETURN(pTopology, NULL, ERROR_MALLOC);
	if(!pTopology->snapshoot)
	{
		pTopology->snapshoot = SnapshootSwitchProperty(pTopology);
		if (pTopology->snapshoot)
		{
			return ERROR_OK_NULL;
		}
		else
		{
			perror("pTopology->snapshoot is null\n");
			return ERROR_MALLOC;
		}
	}
	return ERROR_EXIST;

}

/**
* @brief : 站点快照, 删除快照
* @param :StationTopology* pTopology
* @return: void
* @update: [2018-09-06][张宇飞][创建]
*/
ErrorCode Station_DeleteSnapshoot(StationTopology* pTopology)
{
	CHECK_POINT_RETURN(pTopology, NULL, ERROR_MALLOC);
	SwitchSnapshoot_Destory(&pTopology->snapshoot);
	return ERROR_OK_NULL;

}
