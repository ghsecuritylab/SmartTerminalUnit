/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      logical_app.c
  * @brief:     分布式保护逻辑
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-21
  * @update:     
  */

#include "distribution.h"
#include "distribution_app.h"


#include "extern_interface.h"

static struct rt_thread distribution_thread;
static struct rt_thread connected_thread;

static void DistributionLogicalApp(StationManger* manager);
static void distribution_thread_entry(void* parameter);

ALIGN(RT_ALIGN_SIZE);
static rt_uint8_t rt_distribution_thread_stack[THREAD_DISTRIBUTION_STACK_SIZE];//线程栈
static rt_uint8_t rt_connected_thread_stack[THREAD_CONNECT_STACK_SIZE];//线程栈



static void distribution_thread_entry(void* parameter)
{
    rt_kprintf("thread distribution_thread_entry start.\r\n");
    DistributionLogicalApp(&g_StationManger);
}

/**
* @brief :分布式应用
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-06-16][张宇飞][创建]
*/
static void DistributionLogicalApp(StationManger* manager)
{
    ListElment* element;
    ListDouble* list;
    uint8_t size;
    StationPoint* station;
    FaultDealHandle* handle;
    ErrorCode code;
    if (manager == NULL)
    {
        rt_kprintf("DistributionLogicalApp ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    StationServer* server = &(manager->stationServer);
    if (server == NULL)
    {
        rt_kprintf("DistributionLogicalApp ERROR :server = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    //FaultRemovalInit()
    list = &(server->stationPointList);

    //循环更新模拟开关状态
    do
    {
        element = list_head(list);
        size = list_size(list);
        for (uint8_t i = 0; i < size; i++)
        {
            station = (StationPoint*)(element->data);
            if (station != NULL)
            {
                handle = &(((StationPoint*)(element->data))->removalHandle);
                code = RemovalHandleCheckself(handle);
                if (code == ERROR_OK_NULL)
                {
                    FaultDealStateCenter(handle); 

					if ( (!handle->isRun) && handle->switchProperty->isChanged)
					{
						handle->switchProperty->isChanged = false;
						handle->TransmitMessage(handle, STATUS_MESSAGE);
						handle->TransmitMessage(handle, REMOVAL_MESSAGE);
						handle->TransmitMessage(handle, INSULATE_MESSAGE);
					}
                }  
				else
				{
					code = code;
				}

            }
            else
            {
                rt_kprintf("StationPoint* station = NULL.\n");
                LogAddException(ERROR_NULL_PTR, 0);
                break;
            }           
            element = element->next;
        }
        
    
        rt_thread_delay(3);
    } while (1);

}


/**
* @brief :联络功能判别
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-07-16][张宇飞][创建]
*/
static void connected_thread_entry(void* parameter)
{
	rt_kprintf("thread connected_thread_entry start.\r\n");

	StationManger* manager = (StationManger*)parameter;
	if (manager == NULL)
	{
		rt_kprintf("connect ERROR :manager = NULL.\n");
		LogAddException(ERROR_NULL_PTR, 0);
	}
	StationServer*   stationServer = &(manager->stationServer);
	if (stationServer == NULL)
	{
		rt_kprintf("MutalCommunicationServer ERROR :router = NULL.\n");
		LogAddException(ERROR_NULL_PTR, 0);
	}

	ListDouble* list = &(stationServer->stationPointList);
	ListElment* element;
	uint8_t size;
	do
	{
		element = list_head(list);
		size = list_size(list);
		for (uint8_t i = 0; i < size; i++)
		{
			StationPoint* station = (StationPoint*)(element->data);
			if (station != NULL)
			{
				ConnectedSwitchJuadgeAPP(station);//获取所有开关
				if (station->topology.areaID.isGainComplted)
				{
					GetNeighboorRunState(station); //获取邻居
				}

			}
			else
			{
				rt_kprintf("stationServer* station = NULL.\n");
				LogAddException(ERROR_NULL_PTR, 0);
				break;
			}
			element = element->next;


		}
		rt_thread_delay(1000);
	} while (1);

}


/**
* @brief :分布式逻辑功能引用初始化：分布式保护，联络判别
* @param  
* @return: 0--正常
* @update: [2018-07-21][张宇飞][创建]
*/
void DistributionLogicalAppInit(void)
{
	rt_thread_init(&distribution_thread,                 //线程控制块
		THREAD_DISTRIBUTION_NAME,                       //线程名字，在shell里面可以看到
		distribution_thread_entry,            //线程入口函数
		RT_NULL,                      //线程入口函数参数
		&rt_distribution_thread_stack,     //线程栈起始地址
		sizeof(rt_distribution_thread_stack), //线程栈大小
		5,                            //线程的优先级
		20);                          //线程时间片

	rt_thread_startup(&distribution_thread);

	rt_thread_init(&connected_thread,                 //线程控制块
		THREAD_CONNECT_NAME,                       //线程名字，在shell里面可以看到
		connected_thread_entry,            //线程入口函数
		&g_StationManger,                      //线程入口函数参数
		&rt_connected_thread_stack,     //线程栈起始地址
		sizeof(rt_connected_thread_stack), //线程栈大小
		THREAD_CONNECT_PRIORITY,                            //线程的优先级
		THREAD_CONNECT_TIMESLICE);                          //线程时间片

	rt_thread_startup(&connected_thread);
}